#include <Aheuiplusplus/debugger.hpp>

#include <algorithm>
#include <exception>
#include <functional>
#include <stdexcept>
#include <string>

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#	include <fcntl.h>
#	include <io.h>
#endif

namespace app
{
	debugger::debugger(std::FILE* output_stream, app::interpreter& interpreter) noexcept
		: output_stream_(output_stream), interpreter_(interpreter)
	{}
	debugger::~debugger()
	{
		disconnect_debugger();
	}

	void debugger::connect_debugger()
	{
		if (interpreter_.debugger_ != this && interpreter_.debugger_ != nullptr)
			throw std::bad_function_call();
		else if (interpreter_.debugger_ == nullptr)
		{
			interpreter_.debugger_ = this;
		}
	}
	void debugger::disconnect_debugger() noexcept
	{
		if (interpreter_.debugger_ == this)
		{
			interpreter_.debugger_ = nullptr;
		}
	}
	bool debugger::is_connceted_debugger() const noexcept
	{
		return interpreter_.debugger_ == this;
	}

	void debugger::dump_storages() const
	{
		dump_storages(0);
	}
	void debugger::dump_storages(std::size_t depth) const
	{
		if (!is_connceted_debugger())
			throw std::bad_function_call();

		for (std::size_t i = 0; i < interpreter_.storages_.size() - 1; ++i) // ��δ� ����
		{
			std::string tab(depth * 4, ' ');

			static constexpr char32_t jongsungs[] = {
				0, U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��',
				U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��'
			};

			std::fprintf(output_stream_, "%s%lc(���õ� ��ȣ: %lld, ��ȣ ����: %lld):\n", tab.c_str(),
				static_cast<wchar_t>(get_complete_hangul(U'��', U'��', jongsungs[i])),
				static_cast<long long>(interpreter_.storage_indexs_[i]),
				static_cast<long long>(interpreter_.storages_[i].size()));
		
			++depth;

			for (std::size_t j = 0; j < interpreter_.storages_[i].size(); ++j)
			{
				tab = std::string(depth * 4, ' ');

				app::storage* storage = interpreter_.storages_[i][j];
				std::size_t virtual_length = storage->length();

				if (storage->type() == storage_type::list)
				{
					virtual_length = reinterpret_cast<list*>(storage)->virtual_length();
				}

				std::fprintf(output_stream_, "%s[%lld](���� ����: %lld, ���� ����: %lld):\n", tab.c_str(), static_cast<long long>(j),
					static_cast<long long>(virtual_length), static_cast<long long>(storage->length()));

				app::storage* storage_backup;
				
				if (storage->type() == storage_type::list)
				{
					storage_backup = new list();
				}
				else
				{
					storage_backup = new queue();
				}

				for (std::size_t k = 0; k < storage->length(); ++k)
				{
					storage_backup->push(storage->pop());
				}

				++depth;

				for (std::size_t k = 0; k < storage_backup->length(); ++k)
				{
					tab = std::string(depth * 4, ' ');

					std::fprintf(output_stream_, "%s[%lld] = ", tab.c_str(), static_cast<long long>(k));

					element* value = storage_backup->pop();

					switch (value->index())
					{
					case 0:
					{
						if (std::get<0>(*value).is_integer())
						{
							std::fprintf(output_stream_, "number(%lld)\n", std::get<0>(*value).integer());
						}
						else
						{
							std::fprintf(output_stream_, "number(%f)\n", std::get<0>(*value).decimal());
						}
						break;
					}

					case 1:
					{
						if constexpr (sizeof(char32_t) == sizeof(wchar_t))
						{
							std::fprintf(output_stream_, "character(%lc)\n", std::get<1>(*value));
						}
						else
						{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
							_setmode(_fileno(output_stream_), _O_U16TEXT);
#endif
							std::wstring converted = char32_to_wchar(std::get<1>(*value));
							std::fwprintf(output_stream_, L"character(%ls)\n", converted.c_str());
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
							_setmode(_fileno(output_stream_), _O_TEXT);
#endif
						}
						break;
					}

					case 2:
					{
						if constexpr (sizeof(wchar_t) == sizeof(char32_t))
						{
							raw_code print_value = std::get<2>(*value);
							std::fwprintf(output_stream_, L"string(%ls)\n", print_value.c_str());
						}
						else
						{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
							_setmode(_fileno(output_stream_), _O_U16TEXT);
#endif
							raw_code convert_value = std::get<2>(*value);
							std::wstring converted;

							for (char32_t c : convert_value)
							{
								converted += char32_to_wchar(c);
							}

							std::fwprintf(output_stream_, L"string(%ls)\n", converted.c_str());
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
							_setmode(_fileno(output_stream_), _O_TEXT);
#endif
						}
						break;
					}
					}
				}
			
				--depth;
			}
		
			--depth;
		}
	}

	void debugger::add_breakpoint(std::size_t x, std::size_t y)
	{
		if (std::find_if(breakpoints_.begin(), breakpoints_.end(), [&x, &y](const std::pair<std::size_t, std::size_t>& breakpoint)
		{
			return breakpoint.first == x && breakpoint.second == y;
		}) == breakpoints_.end())
		{
			breakpoints_.push_back({ x, y });
		}
		else
			throw std::invalid_argument("�μ� {x, y}�� �̹� ��ϵ� �ߴ����Դϴ�.");
	}
	void debugger::remove_breakpoint(std::size_t x, std::size_t y)
	{
		std::vector<std::pair<std::size_t, std::size_t>>::iterator iterator = std::find_if(breakpoints_.begin(), breakpoints_.end(),
			[&x, &y](const std::pair<std::size_t, std::size_t>& breakpoint)
		{
			return breakpoint.first == x && breakpoint.second == y;
		});

		if (iterator != breakpoints_.end())
		{
			breakpoints_.erase(iterator);
		}
		else
			throw std::invalid_argument("�μ� {x, y}�� ��ϵ��� ���� �ߴ����Դϴ�.");
	}
	bool debugger::is_added_breakpoint(std::size_t x, std::size_t y)
	{
		return std::find_if(breakpoints_.begin(), breakpoints_.end(),
			[&x, &y](const std::pair<std::size_t, std::size_t>& breakpoint)
		{
			return breakpoint.first == x && breakpoint.second == y;
		}) != breakpoints_.end();
	}

	void debugger::run_with_debugging(const raw_code& code)
	{
		std::size_t x = 0;
		std::size_t y = 0;

		std::size_t direction = 0;

		char32_t last_jungsung = 0;
		bool is_ignored = false;
		bool is_reflection = false;

		try
		{
			if (is_connceted_debugger())
			{
				interpreter_.run_(code, x, y, direction, last_jungsung, is_ignored, is_reflection);
			}
			else
			{
				goto throw_bad_function_call;
			}
		}
		catch (const std::exception& exception)
		{
			std::fprintf(output_stream_, "[Debugger] %lld�� %lld�࿡�� ó������ ���� ����ü ���ܰ� �߻��Ͽ����ϴ�. �����ڿ��� ������ �ֽʽÿ�.\n"
										 "[Debugger] ���� �޼���: %s\n", static_cast<long long>(y), static_cast<long long>(x), exception.what());
		}

		return;

	throw_bad_function_call:
		throw std::bad_function_call();
	}

	const std::vector<std::pair<std::size_t, std::size_t>>& debugger::breakpoints() const noexcept
	{
		return breakpoints_;
	}

	std::FILE* debugger::output_stream() const noexcept
	{
		return output_stream_;
	}
}