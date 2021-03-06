﻿#ifndef AHEUIPLUSPLUS_HEADER_CODE_HPP
#define AHEUIPLUSPLUS_HEADER_CODE_HPP

#include <cstdio>
#include <string>
#include <vector>

namespace app
{
	using raw_code = std::u32string;

	bool is_complete_hangul(char32_t character) noexcept;
	char32_t get_chosung(char32_t hangul);
	char32_t get_jungsung(char32_t hangul);
	char32_t get_jongsung(char32_t hangul);
	char32_t get_jungsung_original(char32_t jungsung);
	char32_t is_added_additional_data(char32_t jungsung);
	
	char32_t get_complete_hangul(char32_t chosung, char32_t jungsung);
	char32_t get_complete_hangul(char32_t chosung, char32_t jungsung, char32_t jongsung);

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
	std::wstring char32_to_wchar(char32_t character);
	char32_t wchar_to_char32(wchar_t high_surrogate, wchar_t low_surrogate = 0);
#endif
	std::string char32_to_u8char(char32_t character);
	char32_t u8char_to_char32(unsigned char first, unsigned char second = 0, unsigned char third = 0, unsigned char fourth = 0);
	
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
	int get_wchar_length(char32_t character);
	int get_wchar_length(wchar_t first);
#endif
	int get_u8char_length(char32_t character);
	int get_u8char_length(unsigned char first);

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
	std::wstring read_wchar(std::FILE* input_stream);
	void write_wchar(std::FILE* output_stream, const std::wstring& character);
#endif
	std::string read_u8char(std::FILE* input_stream);
	void write_u8char(std::FILE* input_stream, const std::string& character);

	char32_t read_char(std::FILE* input_stream);
	void write_char(std::FILE* input_stream, char32_t character);
	void unread_char(std::FILE* input_stream, char32_t character);

	class code final
	{
	public:
		code() = default;
		code(const raw_code& code);
		code(const code& code);
		code(code&& code) noexcept;
		~code() = default;

	public:
		code& operator=(const raw_code& code);
		code& operator=(const code& code);
		code& operator=(code&& code) noexcept;
		bool operator==(const code& code) const = delete;
		bool operator!=(const code& code) const = delete;

	public:
		char32_t command(std::size_t x, std::size_t y) const;
		char32_t& command(std::size_t x, std::size_t y);
		const raw_code& line(std::size_t y) const;
		raw_code& line(std::size_t y);

	private:
		void parse_raw_code_(const raw_code& code);

	public:
		const std::vector<raw_code>& codes() const noexcept;
		std::vector<raw_code>& codes() noexcept;

	private:
		std::vector<raw_code> codes_;
	};
}

#endif