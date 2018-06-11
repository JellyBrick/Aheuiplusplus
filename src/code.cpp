#include <Aheuiplusplus/code.hpp>

#include <stdexcept>
#include <utility>

namespace app
{
	bool is_complete_hangul(char32_t character) noexcept
	{
		return character >= 0xAC00 && character <= 0xD7A3;
	}
	char32_t get_chosung(char32_t hangul)
	{
		if (!is_complete_hangul(hangul))
			throw std::invalid_argument("�μ� hangul�� �ϼ��� ���� �ѱ��̿��� �մϴ�.");

		static constexpr char32_t chosungs[] = {
			U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��',
			U'��', U'��', U'��', U'��', U'��', U'��'
		};

		return chosungs[((((hangul - 0xAC00) - ((hangul - 0xAC00) % 28)) / 28) - ((((hangul - 0xAC00) - ((hangul - 0xAC00) % 28)) / 28) % 21)) / 21];
	}
	char32_t get_jungsung(char32_t hangul)
	{
		if (!is_complete_hangul(hangul))
			throw std::invalid_argument("�μ� hangul�� �ϼ��� ���� �ѱ��̿��� �մϴ�.");

		static constexpr char32_t jungsungs[] = {
			U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��',
			U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��'
		};

		return jungsungs[(((hangul - 0xAC00) - ((hangul - 0xAC00) % 28)) / 28) % 21];
	}
	char32_t get_jongsung(char32_t hangul)
	{
		if (!is_complete_hangul(hangul))
			throw std::invalid_argument("�μ� hangul�� �ϼ��� ���� �ѱ��̿��� �մϴ�.");

		static constexpr char32_t jongsungs[] = {
			0, U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��',
			U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��'
		};

		return jongsungs[(hangul - 0xAC00) % 28];
	}
	char32_t get_jungsung_original(char32_t jungsung)
	{
		if (jungsung < 0x314F || jungsung > 0x3163)
			throw std::invalid_argument("�μ� jungsung�� ���� �ѱ� �����̿��� �մϴ�.");

		switch (jungsung)
		{
		case U'��':
		case U'��':
			return U'��';

		case U'��':
		case U'��':
			return U'��';

		case U'��':
		case U'��':
			return U'��';

		case U'��':
		case U'��':
			return U'��';

		case U'��':
		case U'��':
			return U'��';

		case U'��':
		case U'��':
			return U'��';

		case U'��':
		case U'��':
			return U'��';

		case U'��':
		case U'��':
			return U'��';

		case U'��':
		case U'��':
			return U'��';

		case U'��':
		case U'��':
			return U'��';

		default:
			return U'��';
			
		}
	}
	char32_t is_added_additional_data(char32_t jungsung)
	{
		return jungsung != get_jungsung_original(jungsung);
	}
}

namespace app
{
	code::code(const raw_code& code)
	{
		parse_raw_code_(code);
	}
	code::code(const code& code)
		: codes_(code.codes_)
	{}
	code::code(code&& code) noexcept
		: codes_(std::move(code.codes_))
	{}

	code& code::operator=(const raw_code& code)
	{
		parse_raw_code_(code);
		return *this;
	}
	code& code::operator=(const code& code)
	{
		codes_ = code.codes_;
		return *this;
	}
	code& code::operator=(code&& code) noexcept
	{
		codes_ = std::move(code.codes_);
		return *this;
	}

	char32_t code::command(std::size_t x, std::size_t y) const
	{
		return codes_[y][x];
	}
	char32_t& code::command(std::size_t x, std::size_t y)
	{
		return codes_[y][x];
	}
	const raw_code& code::line(std::size_t y) const
	{
		return codes_[y];
	}
	raw_code& code::line(std::size_t y)
	{
		return codes_[y];
	}

	void code::parse_raw_code_(const raw_code& code)
	{
		codes_.clear();

		raw_code code_editable = code;
		code_editable.push_back(U'\n');
		raw_code splited;

		while (code_editable.find(U'\n') != raw_code::npos)
		{
			splited = code_editable.substr(0, code_editable.find(U'\n') + 1);
			codes_.push_back(splited);

			code_editable = code_editable.substr(code_editable.find(U'\n') + 1);
		}
	}

	const std::vector<raw_code>& code::codes() const noexcept
	{
		return codes_;
	}
	std::vector<raw_code>& code::codes() noexcept
	{
		return codes_;
	}
}