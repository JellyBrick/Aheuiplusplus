﻿#include <Aheuiplusplus/interpreter.hpp>

#include <cmath>
#include <cctype>
#include <cstdio>
#include <cwchar>
#include <cwctype>

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#	include <fcntl.h>
#	include <io.h>
#endif

namespace app
{
	bool interpreter::pop_(char32_t jongsung, bool is_added_additional_data)
	{
		element* value = storage_()->pop();

		if (value != nullptr)
		{
			if (jongsung == U'ㅇ' && !is_added_additional_data) // 숫자(정수) 출력
			{
				if (value->index() == 0) // 숫자일 경우
				{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
					_setmode(_fileno(output_stream_), _O_TEXT);
#endif
					std::fprintf(output_stream_, "%lld", std::get<0>(*value).integer());
				}
				else if (value->index() == 1) // 문자일 경우
				{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
					_setmode(_fileno(output_stream_), _O_TEXT);
#endif
					std::fprintf(output_stream_, "%lld", static_cast<long long>(std::get<1>(*value)));
				}
				else if (value->index() == 2) // 문자열일 경우
				{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
					_setmode(_fileno(output_stream_), _O_TEXT);
#endif

					long long value_integer = 0;

					if (!std::get<2>(*value).empty())
					{
						value_integer = static_cast<long long>(std::get<2>(*value)[0]);
					}

					std::fprintf(output_stream_, "%lld", value_integer);
				}
			}
			else if (jongsung == U'ㅇ' && is_added_additional_data) // 숫자(소수) 출력
			{
				if (value->index() == 0) // 숫자일 경우
				{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
					_setmode(_fileno(output_stream_), _O_TEXT);
#endif
					std::fprintf(output_stream_, "%f", std::get<0>(*value).decimal());
				}
				else if (value->index() == 1) // 문자일 경우
				{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
					_setmode(_fileno(output_stream_), _O_TEXT);
#endif
					std::fprintf(output_stream_, "%f", static_cast<double>(std::get<1>(*value)));
				}
				else if (value->index() == 2) // 문자열일 경우
				{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
					_setmode(_fileno(output_stream_), _O_TEXT);
#endif

					double value_integer = 0.0;

					if (!std::get<2>(*value).empty())
					{
						value_integer = static_cast<double>(std::get<2>(*value)[0]);
					}

					std::fprintf(output_stream_, "%f", value_integer);
				}
			}
			else if (jongsung == U'ㅎ' && !is_added_additional_data) // 문자 출력
			{
				if (value->index() == 0) // 숫자일 경우
				{
					if constexpr (sizeof(wchar_t) == sizeof(char32_t))
					{
						std::fwprintf(output_stream_, L"%lc", static_cast<wchar_t>(std::get<0>(*value).integer()));
					}
					else
					{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
						_setmode(_fileno(output_stream_), _O_U16TEXT);
#endif
						std::wstring converted = char32_to_wchar(static_cast<char32_t>(std::get<0>(*value).integer()));
						std::fwprintf(output_stream_, L"%ls", converted.c_str());
					}
				}
				else if (value->index() == 1) // 문자일 경우
				{
					if constexpr (sizeof(wchar_t) == sizeof(char32_t))
					{
						std::fwprintf(output_stream_, L"%lc", std::get<1>(*value));
					}
					else
					{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
						_setmode(_fileno(output_stream_), _O_U16TEXT);
#endif
						std::wstring converted = char32_to_wchar(std::get<1>(*value));
						std::fwprintf(output_stream_, L"%ls", converted.c_str());
					}
				}
				else if (value->index() == 2) // 문자열일 경우
				{
					if constexpr (sizeof(wchar_t) == sizeof(char32_t))
					{
						std::fwprintf(output_stream_, L"%lc", std::get<2>(*value).empty() ? 0 : std::get<2>(*value)[0]);
					}
					else
					{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
						_setmode(_fileno(output_stream_), _O_U16TEXT);
#endif
						std::wstring converted = char32_to_wchar(std::get<2>(*value).empty() ? 0 : std::get<2>(*value)[0]);
						std::fwprintf(output_stream_, L"%ls", converted.c_str());
					}
				}
			}
			else if (jongsung == U'ㅎ' && is_added_additional_data) // 문자열 출력
			{
				if (value->index() == 0)
				{
					if constexpr (sizeof(wchar_t) == sizeof(char32_t)) // 숫자일 경우
					{
						std::fwprintf(output_stream_, L"%lc", static_cast<wchar_t>(std::get<0>(*value).integer()));
					}
					else
					{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
						_setmode(_fileno(output_stream_), _O_U16TEXT);
#endif
						std::wstring converted = char32_to_wchar(static_cast<char32_t>(std::get<0>(*value).integer()));
						std::fwprintf(output_stream_, L"%ls", converted.c_str());
					}
				}
				else if (value->index() == 1) // 문자일 경우
				{
					if constexpr (sizeof(wchar_t) == sizeof(char32_t))
					{
						std::fwprintf(output_stream_, L"%lc", std::get<1>(*value));
					}
					else
					{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
						_setmode(_fileno(output_stream_), _O_U16TEXT);
#endif
						std::wstring converted = char32_to_wchar(std::get<1>(*value));
						std::fwprintf(output_stream_, L"%ls", converted.c_str());
					}
				}
				else if (value->index() == 2) // 문자열일 경우
				{
					if constexpr (sizeof(wchar_t) == sizeof(char32_t))
					{
						raw_code print_value = std::get<2>(*value);
						std::fwprintf(output_stream_, L"%ls", print_value.c_str());
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

						std::fwprintf(output_stream_, L"%ls", converted.c_str());
					}
				}
			}
			else if (is_added_additional_data)
			{
				storage_()->push(value);
				value = nullptr;

				return true;
			}
			
			delete value;
			return false;
		}

		return true;
	}
	bool interpreter::push_(char32_t jongsung, bool is_added_additional_data)
	{
		if (jongsung == U'ㅇ' && !is_added_additional_data) // 숫자(정수) 입력
		{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
			_setmode(_fileno(input_stream_), _O_TEXT);
#endif
			if (std::feof(input_stream_))
			{
				storage_()->push(new element(number(0ll)));
				return false;
			}

			long long temp;
			std::fscanf(input_stream_, " %lld", &temp);

			storage_()->push(new element(number(temp)));

			return false;
		}
		else if (jongsung == U'ㅇ' && is_added_additional_data) // 숫자(소수) 입력
		{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
			_setmode(_fileno(input_stream_), _O_TEXT);
#endif
			if (std::feof(input_stream_))
			{
				storage_()->push(new element(number(0.0)));
				return false;
			}

			double temp;
			std::fscanf(input_stream_, " %lf", &temp);

			storage_()->push(new element(number(temp)));

			return false;
		}
		else if (jongsung == U'ㅎ' && !is_added_additional_data) // 문자 입력
		{
		input_char:
			if (std::feof(input_stream_))
			{
				storage_()->push(new element(0));
				return false;
			}

			if constexpr (sizeof(wchar_t) == sizeof(char32_t))
			{
				char32_t temp;

				std::fwscanf(input_stream_, L" %lc", &temp);

				if (std::iswspace(temp))
				{
					goto input_char;
				}

				storage_()->push(new element(temp));
			}
			else
			{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
				_setmode(_fileno(input_stream_), _O_U16TEXT);
#endif
				wchar_t high_surrogate = std::fgetwc(input_stream_);

				if (!is_processed_space_char_ && std::isspace(*reinterpret_cast<char*>(&high_surrogate)))
				{
					if (*(reinterpret_cast<char*>(&high_surrogate) + 1) == 0)
					{
						is_processed_space_char_ = true;
						goto input_char;
					}

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
					_setmode(_fileno(input_stream_), _O_TEXT);
#endif
					char high_surrogate_low = std::fgetc(input_stream_);

					if (high_surrogate_low == 0)
					{
						is_processed_space_char_ = true;
						goto input_char;
					}
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
					_setmode(_fileno(input_stream_), _O_U16TEXT);
#endif

					char temp = *(reinterpret_cast<char*>(&high_surrogate) + 1);
					*reinterpret_cast<char*>(&high_surrogate) = temp;
					*(reinterpret_cast<char*>(&high_surrogate) + 1) = high_surrogate_low;
				}

				if (high_surrogate >= 0xD800 && high_surrogate <= 0xDBFF)
				{
					wchar_t low_surrogate = std::fgetwc(input_stream_);

					is_processed_space_char_ = false;
					storage_()->push(new element(wchar_to_char32(high_surrogate, low_surrogate)));
				}
				else
				{
					is_processed_space_char_ = false;
					storage_()->push(new element(static_cast<char32_t>(high_surrogate)));
				}
			}

			return false;
		}
		else if (jongsung == U'ㅎ' && is_added_additional_data) // 문자열 입력
		{
			if (std::feof(input_stream_))
			{
				storage_()->push(new element(0));
				return false;
			}

			if constexpr (sizeof(wchar_t) == sizeof(char32_t))
			{
				raw_code temp;

				do
				{
					char32_t input = std::fgetwc(input_stream_);

					if (std::iswspace(input))
					{
						break;
					}

					temp += input;
				} while (!std::feof(input_stream_));

				storage_()->push(new element(temp));
			}
			else
			{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
				_setmode(_fileno(input_stream_), _O_U16TEXT);
#endif
				raw_code temp;

				bool is_first = true;

				while (!std::feof(input_stream_))
				{
					wchar_t high_surrogate = std::fgetwc(input_stream_);

					if (!is_processed_space_char_ && std::isspace(*reinterpret_cast<char*>(&high_surrogate)))
					{
						if (*(reinterpret_cast<char*>(&high_surrogate) + 1) == 0)
						{
							is_processed_space_char_ = true;

							if (is_first)
							{
								is_first = false;
								continue;
							}
							else
							{
								break;
							}
						}

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
						_setmode(_fileno(input_stream_), _O_TEXT);
#endif
						char high_surrogate_low = std::fgetc(input_stream_);

						if (high_surrogate_low == 0)
						{
							is_processed_space_char_ = true;
						}
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
						_setmode(_fileno(input_stream_), _O_U16TEXT);
#endif

						char temp2 = *(reinterpret_cast<char*>(&high_surrogate) + 1);
						*reinterpret_cast<char*>(&high_surrogate) = temp2;
						*(reinterpret_cast<char*>(&high_surrogate) + 1) = high_surrogate_low;
					}

					if (high_surrogate >= 0xD800 && high_surrogate <= 0xDBFF)
					{
						wchar_t low_surrogate = std::fgetwc(input_stream_);

						temp += wchar_to_char32(high_surrogate, low_surrogate);
					}
					else
					{
						if (std::iswspace(high_surrogate))
						{
							is_processed_space_char_ = true;
							break;
						}

						temp += high_surrogate;
					}
				}

				storage_()->push(new element(temp));
			}

			return false;
		}
		else
		{
			storage_()->push(new element(number(get_integer_(jongsung, is_added_additional_data))));

			return false;
		}
	}
	bool interpreter::copy_(char32_t jongsung, bool is_added_additional_data)
	{
		if (jongsung == 0)
		{
			element* copyed = storage_()->copy();

			if (copyed == nullptr)
			{
				return true;
			}

			if (is_added_additional_data)
			{
				if (copyed->index() == 0)
				{
					long long floor = std::get<0>(*copyed).integer();
					*copyed = element(number(floor));
				}
			}

			return false;
		}
		else if (jongsung == U'ㄱ')
		{
			element* copyed = storage_()->copy();

			if (copyed == nullptr)
			{
				return true;
			}

			if (copyed->index() == 0)
			{
				if (is_added_additional_data)
				{
					long long abs = static_cast<long long>(std::abs(std::get<0>(*copyed).decimal()));
					*copyed = element(number(abs));
				}
				else
				{
					double abs = std::abs(std::get<0>(*copyed).decimal());
					*copyed = element(number(abs));
				}
			}

			return false;
		}
		else if (jongsung == U'ㄴ')
		{
			element* copyed = storage_()->copy();

			if (copyed == nullptr)
			{
				return true;
			}

			if (copyed->index() == 0)
			{
				long long ceil = static_cast<long long>(std::ceil(std::get<0>(*copyed).decimal()));
				*copyed = element(number(ceil));
			}

			return false;
		}
		else if (jongsung == U'ㄵ')
		{
			element* copyed = storage_()->copy();

			if (copyed == nullptr)
			{
				return true;
			}

			if (copyed->index() == 0)
			{
				if (is_added_additional_data)
				{
					long long pow = static_cast<long long>(std::pow(std::get<0>(*copyed).decimal(), 2));
					*copyed = element(number(pow));
				}
				else
				{
					double pow = std::pow(std::get<0>(*copyed).decimal(), 2);
					*copyed = element(number(pow));
				}
			}

			return false;
		}
		else if (jongsung == U'ㄶ')
		{
			element* copyed = storage_()->copy();

			if (copyed == nullptr)
			{
				return true;
			}

			if (copyed->index() == 0)
			{
				if (is_added_additional_data)
				{
					long long sqrt = static_cast<long long>(std::sqrt(std::get<0>(*copyed).decimal()));
					*copyed = element(number(sqrt));
				}
				else
				{
					double sqrt = std::sqrt(std::get<0>(*copyed).decimal());
					*copyed = element(number(sqrt));
				}
			}

			return false;
		}
		else if (jongsung == U'ㄷ')
		{
			element* copyed = storage_()->copy();

			if (copyed == nullptr)
			{
				return true;
			}

			if (copyed->index() == 0)
			{
				if (is_added_additional_data)
				{
					long long exp = static_cast<long long>(std::exp(std::get<0>(*copyed).decimal()));
					*copyed = element(number(exp));
				}
				else
				{
					double exp = std::exp(std::get<0>(*copyed).decimal());
					*copyed = element(number(exp));
				}
			}

			return false;
		}

		return true;
	}
	bool interpreter::swap_(char32_t jongsung, bool is_added_additional_data)
	{
		if (jongsung == 0 && !is_added_additional_data)
		{
			element* first = storage_()->pop();
			element* second = storage_()->pop();

			if (first == nullptr)
			{
				return true;
			}
			else if (second == nullptr)
			{
				storage_()->push(first);
				return true;
			}

			storage_()->push(first);
			storage_()->push(second);

			return false;
		}

		return true;
	}
}