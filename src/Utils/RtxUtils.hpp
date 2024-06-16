#pragma once

namespace RtxUtils
{
#define	PITCH				0		// up / down
#define	YAW					1		// left / right
#define	ROLL				2		// fall over
#define M_PI				3.14159265358979323846f

	void unit_quat_to_axis(const float* quat, float(*axis)[3]);
	void angle_vectors(const float* angles, float* forward, float* right, float* up);
	void scale3(const float* v1, float scalar, float* out);
	void copy(const float* in, float* out, int size);

	int try_stoi(const std::string& str, const int& default_return_val);
	float try_stof(const std::string& str, const float& default_return_val = 0.0f);
	void replace_all(std::string& source, const std::string_view& from, const std::string_view& to);
	bool starts_with(std::string_view haystack, std::string_view needle);
	std::string split_string_between_delims(const std::string& str, const char delim_start, const char delim_end);

	static std::vector<std::string> split(const std::string& raw_input, const std::vector<char>& delims)
	{
		std::vector<std::string> strings;

		auto find_first_delim = [](const std::string_view& input, const std::vector<char>& delims) -> std::pair<char, std::size_t>
			{
				auto first_delim = 0;
				auto first_delim_index = static_cast<std::size_t>(-1);
				auto index = 0u;

				for (auto& delim : delims)
				{
					if ((index = input.find(delim)) != std::string::npos)
					{
						if (first_delim_index == -1 || index < first_delim_index)
						{
							first_delim = delim;
							first_delim_index = index;
						}
					}
				}

				return { (char)first_delim, first_delim_index };
			};

		std::string input = raw_input;

		while (!input.empty())
		{
			const auto split_delim = find_first_delim(input, delims);
			if (split_delim.first != 0)
			{
				strings.push_back(input.substr(0, split_delim.second));
				input = input.substr(split_delim.second + 1);
			}
			else
			{
				break;
			}
		}

		strings.push_back(input);
		return strings;
	}

	static std::vector<std::string> split(std::string& str, char delimiter)
	{
		return split(str, std::vector < char >({ delimiter }));
	}

	static std::vector<std::string> split(const std::string& str, char delimiter)
	{
		return split(str, std::vector < char >({ delimiter }));
	}

	bool open_file_homepath(const std::string& sub_dir, const std::string& file_name, bool print, std::ifstream& file);
}