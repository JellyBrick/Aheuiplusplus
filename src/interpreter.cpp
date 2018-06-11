#include <Aheuiplusplus/interpreter.hpp>

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <string>

namespace app
{
	namespace
	{
		std::wstring char32_to_wchar(char32_t character)
		{
			if (character <= 65'535)
			{
				return std::wstring(1, static_cast<wchar_t>(character));
			}
			
			std::wstring result;
			result.resize(2);

			char32_t temp = character - 0x10000;

			wchar_t high_surrogate = (temp / 0x400) + 0xD800;
			wchar_t low_surrogate = (temp % 0x400) + 0xDC00;

			result[0] = low_surrogate;
			result[1] = high_surrogate;

			return result;
		}
		char32_t wchar_to_char32(wchar_t high_surrogate, wchar_t low_surrogate)
		{
			char32_t temp_high = (high_surrogate - 0xD800) * 0x400;
			char32_t temp_low = low_surrogate - 0xDC00;

			return temp_high + temp_low + 0x10000;
		}
	}

	interpreter::interpreter(std::FILE* input_stream, std::FILE* output_stream)
		: input_stream_(input_stream), output_stream_(output_stream)
	{
		initialize_();
	}
	interpreter::interpreter(app::version version, std::FILE* input_stream, std::FILE* output_stream)
		: version_(version), input_stream_(input_stream), output_stream_(output_stream)
	{
		initialize_();
	}

	void interpreter::run(const raw_code& code)
	{
		app::code splited_code = code;

		std::size_t x = 0;
		std::size_t y = 0;

		std::size_t direction = 0; // 0: ����, 1: ������, 2: ��, 3: �Ʒ�

		char32_t last_jungsung = 0;

		while (true)
		{
			char32_t command = splited_code.command(x, y);
			
			if (is_complete_hangul(command))
			{
				char32_t chosung = get_chosung(command);
				char32_t jungsung = get_jungsung(command);
				char32_t jongsung = get_jongsung(command);
				
				char32_t jungsung_org = get_jungsung_original(jungsung);
				bool is_added_additional_data = app::is_added_additional_data(jungsung);

				last_jungsung = jungsung;

				switch (chosung)
				{
				case U'��':
					pop_(jongsung, is_added_additional_data);
					break;

				case U'��':
					push_(jongsung, is_added_additional_data);
					break;


				case U'��':
					break;

				case U'��':
					return;
				}

				switch (jungsung_org)
				{
				case U'��':
					go_right_(x, y, 1, direction, splited_code);
					break;

				case U'��':
					go_right_(x, y, 2, direction, splited_code);
					break;

				case U'��':
					go_left_(x, y, 1, direction, splited_code);
					break;

				case U'��':
					go_left_(x, y, 2, direction, splited_code);
					break;

				case U'��':
					go_up_(x, y, 1, direction, splited_code);
					break;

				case U'��':
					go_up_(x, y, 2, direction, splited_code);
					break;

				case U'��':
					go_down_(x, y, 1, direction, splited_code);
					break;

				case U'��':
					go_down_(x, y, 2, direction, splited_code);
					break;

				case U'��':
				{
					if (direction == 0)
					{
						go_right_(x, y, 1, direction, splited_code);
					}
					else if (direction == 1)
					{
						go_left_(x, y, 1, direction, splited_code);
					}
					else if (direction == 2)
					{
						go_down_(x, y, 1, direction, splited_code);
					}
					else
					{
						go_up_(x, y, 1, direction, splited_code);
					}

					break;
				}

				case U'��':
				{
					if (direction == 0)
					{
						go_left_(x, y, 1, direction, splited_code);
					}
					else if (direction == 1)
					{
						go_right_(x, y, 1, direction, splited_code);
					}
					else if (direction == 2)
					{
						go_up_(x, y, 1, direction, splited_code);
					}
					else
					{
						go_down_(x, y, 1, direction, splited_code);
					}

					break;
				}
				
				case U'��':
				{
					if (direction == 0)
					{
						go_left_(x, y, 1, direction, splited_code);
					}
					else if (direction == 1)
					{
						go_right_(x, y, 1, direction, splited_code);
					}
					else if (direction == 2)
					{
						go_down_(x, y, 1, direction, splited_code);
					}
					else
					{
						go_up_(x, y, 1, direction, splited_code);
					}

					break;
				}
				}
			}
		}
	}

	const app::storage* interpreter::storage(std::size_t index) const
	{
		return storages_[index][storage_indexs_[index]];
	}
	std::size_t interpreter::storage_index(std::size_t index) const
	{
		return storage_indexs_[index];
	}

	void interpreter::initialize_()
	{
		for (std::size_t i = 0; i < 26; ++i)
		{
			std::vector<app::storage*> storages;
			storages.push_back(new list());

			storages_.push_back(storages);
		}

		std::vector<app::storage*> queues;
		queues.push_back(new queue());
		storages_.insert(storages_.begin() + 21, queues);

		std::vector<app::storage*> passages;
		storages_.push_back(passages);

		for (std::size_t i = 0; i < 28; ++i)
		{
			storage_indexs_.push_back(0);
		}
	}

	void interpreter::pop_(char32_t jongsung, bool is_added_additional_data)
	{
		element* value = storage_()->pop();

		if (value != nullptr)
		{
			if (jongsung == U'��' && !is_added_additional_data) // ����(����) ���
			{
				if (value->index() == 0) // ������ ���
				{
					std::fprintf(output_stream_, "%lld", static_cast<std::int64_t>(std::get<0>(*value)));
				}
				else if (value->index() == 1) // ������ ���
				{
					std::fprintf(output_stream_, "%lld", static_cast<std::int64_t>(std::get<1>(*value)));
				}
			}
			else if (jongsung == U'��' && is_added_additional_data) // ����(�Ҽ�) ���
			{
				if (value->index() == 0) // ������ ���
				{
					std::fprintf(output_stream_, "%f", std::get<0>(*value));
				}
				else if (value->index() == 1) // ������ ���
				{
					std::fprintf(output_stream_, "%f", static_cast<double>(std::get<1>(*value)));
				}
			}
			else if (jongsung == U'��' && !is_added_additional_data) // ���� ���
			{
				if (value->index() == 0) // ������ ���
				{
					if constexpr (sizeof(wchar_t) == sizeof(char32_t))
					{
						std::fwprintf(output_stream_, L"%c", static_cast<wchar_t>(std::get<0>(*value)));
					}
					else
					{
						std::wstring converted = char32_to_wchar(static_cast<char32_t>(std::get<0>(*value)));
						std::fwprintf(output_stream_, L"%ls", converted.c_str());
					}
				}
				else if (value->index() == 1) // ������ ���
				{
					if constexpr (sizeof(wchar_t) == sizeof(char32_t))
					{
						std::fwprintf(output_stream_, L"%c", std::get<1>(*value));
					}
					else
					{
						std::wstring converted = char32_to_wchar(std::get<1>(*value));
						std::fwprintf(output_stream_, L"%ls", converted.c_str());
					}
				}
			}
			else if (jongsung == U'��' && is_added_additional_data) // ���ڿ� ���
			{
				if (value->index() == 1) // ������ ���
				{
					if constexpr (sizeof(wchar_t) == sizeof(char32_t))
					{
						std::fwprintf(output_stream_, L"%c", std::get<1>(*value));
					}
					else
					{
						std::wstring converted = char32_to_wchar(std::get<1>(*value));
						std::fwprintf(output_stream_, L"%ls", converted.c_str());
					}
				}
				else if (value->index() == 2) // ���ڿ��� ���
				{
					if constexpr (sizeof(wchar_t) == sizeof(char32_t))
					{
						raw_code print_value = std::get<2>(*value);
						std::fwprintf(output_stream_, L"%ls", print_value.c_str());
					}
					else
					{
						raw_code convert_value = std::get<2>(*value);
						std::wstring converted;

						for (char32_t c : convert_value)
						{
							converted += char32_to_wchar(c);
						}

						std::fwprintf(output_stream_, L"%ls", converted.c_str());
					}
				}
			}
			else if (jongsung == 0 && !is_added_additional_data)
			{
				storage_()->push(value);
				value = nullptr;

				storage_()->push(new element(static_cast<double>(storage_()->length())));
			}
			else if (is_added_additional_data)
			{
				storage_()->push(value);
				value = nullptr;
			}

			delete value;
		}
	}
	void interpreter::push_(char32_t jongsung, bool is_added_additional_data)
	{
		if (jongsung == U'��' && !is_added_additional_data) // ����(����) �Է�
		{
			long long temp;
			std::fscanf(input_stream_, "%lld", &temp);

			storage_()->push(new element(static_cast<double>(temp)));
		}
		else if (jongsung == U'��' && is_added_additional_data) // ����(�Ҽ�) �Է�
		{
			double temp;
			std::fscanf(input_stream_, "%lf", &temp);

			storage_()->push(new element(temp));
		}
		else if (jongsung == U'��' && !is_added_additional_data) // ���� �Է�
		{
			if constexpr (sizeof(wchar_t) == sizeof(char32_t))
			{
				char32_t temp;
				std::fwscanf(input_stream_, L"%c", &temp);

				storage_()->push(new element(temp));
			}
			else
			{
				wchar_t high_surrogate = std::fgetwc(input_stream_);

				if (high_surrogate >= 0xD800 && high_surrogate <= 0xDBFF)
				{
					wchar_t low_surrogate = std::fgetwc(input_stream_);
					
					storage_()->push(new element(wchar_to_char32(high_surrogate, low_surrogate)));
				}
				else
				{
					storage_()->push(new element(static_cast<char32_t>(high_surrogate)));
				}
			}
		}
		else if (jongsung == U'��' && is_added_additional_data) // ���ڿ� �Է�
		{
			if constexpr (sizeof(wchar_t) == sizeof(char32_t))
			{
				raw_code temp;

				while (!std::feof(input_stream_))
				{
					temp += std::fgetwc(input_stream_);
				}

				storage_()->push(new element(temp));
			}
			else
			{
				raw_code temp;

				while (!std::feof(input_stream_))
				{
					wchar_t high_surrogate = std::fgetwc(input_stream_);

					if (high_surrogate >= 0xD800 && high_surrogate <= 0xDBFF)
					{
						wchar_t low_surrogate = std::fgetwc(input_stream_);

						temp += wchar_to_char32(high_surrogate, low_surrogate);
					}
					else
					{
						temp += high_surrogate;
					}
				}

				storage_()->push(new element(temp));
			}
		}
		else if (jongsung == 0 && !is_added_additional_data)
		{
			storage_()->push(new element(0.0));
		}
		else if (jongsung == 0 && is_added_additional_data)
		{
			// TODO: Ŭ����, ����ü, �Լ�
		}
		else
		{
			storage_()->push(new element(get_integer_(jongsung, is_added_additional_data)));
		}
	}

	void interpreter::go_left_(std::size_t& x, std::size_t& y, std::size_t move, int direction, app::code& splited_code)
	{
		x -= move;

		if (x == static_cast<std::size_t>(-1))
		{
			x = splited_code.line(y).size() - 2;
		}

		direction = 0;
	}
	void interpreter::go_right_(std::size_t& x, std::size_t& y, std::size_t move, int direction, app::code& splited_code)
	{
		x += move;

		if (x >= splited_code.line(y).size() - 1)
		{
			x = 0;
		}

		direction = 1;
	}
	void interpreter::go_up_(std::size_t&, std::size_t& y, std::size_t move, int direction, app::code& splited_code)
	{
		y -= move;

		if (y == static_cast<std::size_t>(-1))
		{
			y = splited_code.codes().size() - 2;
		}

		direction = 2;
	}
	void interpreter::go_down_(std::size_t&, std::size_t& y, std::size_t move, int direction, app::code& splited_code)
	{
		y += move;

		if (y >= splited_code.codes().size() - 1)
		{
			y = 0;
		}

		direction = 3;
	}
	double app::interpreter::get_integer_(char32_t jongsung, bool is_added_additional_data)
	{
		double value;

		switch (jongsung)
		{
		case U'��':
		case U'��':
		case U'��':
			value = 2.0;
			break;

		case U'��':
		case U'��':
		case U'��':
			value = 3.0;
			break;

		case U'��':
		case U'��':
		case U'��':
		case U'��':
		case U'��':
		case U'��':
		case U'��':
		case U'��':
			value = 4.0;
			break;

		case U'��':
		case U'��':
		case U'��':
			value = 5.0;
			break;

		case U'��':
			value = 6.0;
			break;

		case U'��':
		case U'��':
			value = 7.0;
			break;

		case U'��':
			value = 8.0;
			break;

		case U'��':
		case U'��':
		case U'��':
		case U'��':
			value = 9.0;
			break;
		}

		if (is_added_additional_data)
		{
			value = -value;
		}

		return value;
	}

	app::storage* interpreter::storage_()
	{
		return storages_[selected_index_][storage_indexs_[selected_index_]];
	}

	app::version interpreter::version() const noexcept
	{
		return version_;
	}
	const std::vector<std::vector<app::storage*>>& interpreter::storages() const noexcept
	{
		return storages_;
	}
	
	const std::vector<function*>& interpreter::functions() const noexcept
	{
		return functions_;
	}
	const std::vector<std::size_t>& interpreter::storage_indexs() const noexcept
	{
		return storage_indexs_;
	}
	std::size_t interpreter::selected_index() const noexcept
	{
		return selected_index_;
	}

	std::FILE* interpreter::input_stream()
	{
		return input_stream_;
	}
	std::FILE* interpreter::output_stream()
	{
		return output_stream_;
	}
}