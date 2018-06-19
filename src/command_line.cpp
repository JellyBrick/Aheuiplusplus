#include <Aheuiplusplus/command_line.hpp>

#include <Aheuiplusplus/interpreter.hpp>

#include <functional>

namespace app
{
	command_line::command_line(const command_line& data)
		: option_aheui_(data.option_aheui_), option_interpreting_mode_(data.option_interpreting_mode_),
		option_version_(data.option_version_), option_loud_mode_(data.option_loud_mode_), option_input_end_mode_(data.option_input_end_mode_),
		option_code_path_(data.option_code_path_)
	{}
	
	command_line& command_line::operator=(const command_line& data)
	{
		option_aheui_ = data.option_aheui_;
		option_interpreting_mode_ = data.option_interpreting_mode_;
		option_version_ = data.option_version_;

		option_loud_mode_ = data.option_loud_mode_;
		option_input_end_mode_ = data.option_input_end_mode_;

		option_code_path_ = data.option_code_path_;

		return *this;
	}

	bool command_line::parse(int argc, char** argv)
	{
		return parse(stdout, argc, argv);
	}
	bool command_line::parse(std::FILE* output_stream, int argc, char** argv)
	{
		if (argc == 1)
		{
			std::fprintf(output_stream, "����: �Է��� �����ϴ�. --help �ɼ��� �̿��� ������ Ȯ���Ͻ� �� �ֽ��ϴ�.\n");
			return false;
		}

		static const char* duplicate_message = " �ɼ��� �ι� �̻� ���Ǿ����ϴ�.";
		static const char* invalid_argument_message = " �ɼ��� �μ��� �ùٸ��� �ʽ��ϴ�. --help �ɼ��� �̿��� �ùٸ� �μ� ���¸� Ȯ���Ͻ� �� �ֽ��ϴ�.";
	
		bool option_help = false;
		bool option_version = false;

		option_aheui_ = false;
		option_interpreting_mode_ = false;
		option_version_ = version::none;

		option_loud_mode_ = false;
		option_input_end_mode_ = false;

		option_code_path_.clear();

		for (int i = 1; i < argc; ++i)
		{
			std::string argument = argv[i];

			if (argument == "--help")
			{
				if (option_help)
				{
					std::fprintf(output_stream, "����: %s%s\n", "--help", duplicate_message);
					return false;
				}

				option_help = true;
			}
			else if (argument == "--version")
			{
				if (option_version)
				{
					std::fprintf(output_stream, "����: %s%s\n", "--version", duplicate_message);
					return false;
				}

				option_version = true;
			}
			else if (argument == "-A")
			{
				if (option_aheui_)
				{
					std::fprintf(output_stream, "����: %s%s\n", "-A", duplicate_message);
					return false;
				}

				option_aheui_ = true;
			}
			else if (argument == "-i")
			{
				if (option_interpreting_mode_)
				{
					std::fprintf(output_stream, "����: %s%s\n", "-i", duplicate_message);
					return false;
				}

				option_interpreting_mode_ = true;
			}
			else if (argument.substr(0, 4) == "-std")
			{
				if (argument.length() <= 5)
				{
					std::fprintf(output_stream, "����: %s%s\n", "-std", invalid_argument_message);
					return false;
				}
				else if (argument[4] != '=')
				{
					std::fprintf(output_stream, "����: %s%s\n", "-std", invalid_argument_message);
					return false;
				}

				std::string argument_of_argument = argument.substr(5);

				if (argument_of_argument.find('.') == std::string::npos)
				{
					try
					{
						int major = std::stoi(argument_of_argument);

						option_version_ = get_version(major);

						if (option_version_ == version::none)
						{
							std::fprintf(output_stream, "����: %s%s\n", "-std", invalid_argument_message);
							return false;
						}
					}
					catch (...)
					{
						std::fprintf(output_stream, "����: %s%s\n", "-std", invalid_argument_message);
						return false;
					}
				}
				else
				{
					std::string major_raw = argument_of_argument.substr(0, argument_of_argument.find('.'));
					std::string minor_raw = argument_of_argument.substr(argument_of_argument.find('.') + 1);

					if (minor_raw.find('.') != std::string::npos)
					{
						std::fprintf(output_stream, "����: %s%s\n", "-std", invalid_argument_message);
						return false;
					}

					int major = std::stoi(major_raw);
					int minor = std::stoi(minor_raw);

					option_version_ = get_version(major, minor);

					if (option_version_ == version::none)
					{
						std::fprintf(output_stream, "����: %s%s\n", "-std", invalid_argument_message);
						return false;
					}
				}
			}
			else if (argument == "-l")
			{
				if (option_loud_mode_)
				{
					std::fprintf(output_stream, "����: %s%s\n", "-l", duplicate_message);
					return false;
				}

				option_loud_mode_ = true;
			}
			else
			{
				if (argument.front() == '-')
				{
					std::fprintf(output_stream, "����: %s�� �� �� ���� �ɼ��Դϴ�. --help �ɼ��� �̿��� �ùٸ� �ɼ��� Ȯ���Ͻ� �� �ֽ��ϴ�.\n", argument.c_str());
					return false;
				}
				else if (!option_code_path_.empty())
				{
					std::fprintf(output_stream, "����: ��ΰ� �ΰ� �̻� �ԷµǾ����ϴ�.\n");
					return false;
				}

				option_code_path_ = argument;
			}
		}

		if (option_help)
		{
			std::fprintf(output_stream,
				"����: %s [option(s)...] [path]\n"
				"path�� ����++(�Ǵ� ����) �ڵ尡 ��ϵ� BOM�� ���� UTF-8�� ���ڵ� �� �ؽ�Ʈ �����̿��� �մϴ�(���������� ����� ��� �ʿ����� �ʽ��ϴ�.).\n"
				"\n"
				"--help - ���� �� �ɼ� ����� ���ϴ�.\n"
				"--version - ���α׷��� ������ ���ϴ�.\n"
				"\n"
				"-A - ���� ���� ���� ��ȯ�մϴ�(����++�� ����� �̿��� �� �����ϴ�.). -std �ɼǰ� �Բ� ���� �� �����ϴ�.\n"
				"-std=<version> - � ������ ����++ ǥ���� ������ �����մϴ�. version�� m �Ǵ� m.n ���·� �����˴ϴ�(�̶� m�� �� ����, n�� �� �����Դϴ�.). -A �ɼǰ� �Բ� ���� �� �����ϴ�.\n"
				"-i - ���������� ���� ��ȯ�մϴ�.\n"
				"\n"
				"-l - �Է��� �޾ƾ� �� �� �Է��� ��û�ϴ� �޼����� ����մϴ�.\n",
				argv[0]);

			return false;
		}
		else if (option_version)
		{
			std::fprintf(output_stream,
				"����++ ǥ�� ���������� %s\n\n%s\n�� ���α׷��� ���� ����Ʈ�����, �ҽ� �ڵ�� �� ������Ʈ���� MIT ���̼����� ���� �����ǰ� �ֽ��ϴ�.\n",
				interpreter::version_string, "https://github.com/kmc7468/Aheuiplusplus");

			return false;
		}
		
		if (option_aheui_ && option_version_ != version::none)
		{
			std::fprintf(output_stream, "����: -A �ɼǰ� -std �ɼ��� �Բ� ���� �� �����ϴ�.\n");

			return false;
		}
		else if (option_interpreting_mode_ && !option_code_path_.empty())
		{
			std::fprintf(output_stream, "����: ���������� ����� ��� path�� �ʿ����� �ʽ��ϴ�.\n");

			return false;
		}
		else if (!option_interpreting_mode_ && option_code_path_.empty())
		{
			std::fprintf(output_stream, "����: �Ϲ� ����� ��� path�� �ʿ��մϴ�.n");

			return false;
		}

		return true;
	}

	bool command_line::option_aheui() const noexcept
	{
		return option_aheui_;
	}
	void command_line::option_aheui(bool new_option_aheui) noexcept
	{
		option_aheui_ = new_option_aheui;
	}
	bool command_line::option_interpreting_mode() const noexcept
	{
		return option_interpreting_mode_;
	}
	void command_line::option_interpreting_mode(bool new_option_interpreting_mode) noexcept
	{
		option_interpreting_mode_ = new_option_interpreting_mode;
	}
	version command_line::option_version() const noexcept
	{
		return option_version_;
	}
	void command_line::option_version(version new_option_version) noexcept
	{
		option_version_ = new_option_version;
	}

	bool command_line::option_loud_mode() const noexcept
	{
		return option_loud_mode_;
	}
	void command_line::option_loud_mode(bool new_option_loud_mode) noexcept
	{
		option_loud_mode_ = new_option_loud_mode;
	}
	bool command_line::option_input_end_mode() const noexcept
	{
		return option_input_end_mode_;
	}
	void command_line::option_input_end_mode(bool new_option_input_end_mode) noexcept
	{
		option_input_end_mode_ = new_option_input_end_mode;
	}

	std::string command_line::option_code_path() const
	{
		return option_code_path_;
	}
	void command_line::option_code_path(const std::string& new_option_code_path)
	{
		option_code_path_ = new_option_code_path;
	}
}