#include "STDInc.hpp"

namespace RtxUtils
{
	void unit_quat_to_axis(const float* quat, float(*axis)[3])
	{
		float xx, xy, xz, xw;
		float yy, yz, yw;
		float zz, zw;

		const float scaledX = quat[0] + quat[0];
		xx = scaledX * quat[0];
		xy = scaledX * quat[1];
		xz = scaledX * quat[2];
		xw = scaledX * quat[3];

		const float scaledY = quat[1] + quat[1];
		yy = scaledY * quat[1];
		yz = scaledY * quat[2];
		yw = scaledY * quat[3];

		const float scaledZ = quat[2] + quat[2];
		zz = scaledZ * quat[2];
		zw = scaledZ * quat[3];

		(*axis)[0] = 1.0f - (yy + zz);
		(*axis)[1] = xy + zw;
		(*axis)[2] = xz - yw;
		(*axis)[3] = xy - zw;
		(*axis)[4] = 1.0f - (xx + zz);
		(*axis)[5] = yz + xw;
		(*axis)[6] = xz + yw;
		(*axis)[7] = yz - xw;
		(*axis)[8] = 1.0f - (xx + yy);
	}

	int try_stoi(const std::string& str, const int& default_return_val)
	{
		int ret = default_return_val;

		try
		{
			ret = std::stoi(str);
		}
		catch (const std::invalid_argument)
		{
		}

		return ret;
	}

	float try_stof(const std::string& str, const float& default_return_val)
	{
		float ret = default_return_val;

		try
		{
			ret = std::stof(str);
		}
		catch (const std::invalid_argument)
		{
		}

		return ret;
	}

	void replace_all(std::string& source, const std::string_view& from, const std::string_view& to)
	{
		std::string new_string;
		new_string.reserve(source.length());  // avoids a few memory allocations

		std::string::size_type last_pos = 0;
		std::string::size_type findPos;

		while (std::string::npos != (findPos = source.find(from, last_pos)))
		{
			new_string.append(source, last_pos, findPos - last_pos);
			new_string += to;
			last_pos = findPos + from.length();
		}

		// Care for the rest after last occurrence
		new_string += source.substr(last_pos);

		source.swap(new_string);
	}

	bool starts_with(std::string_view haystack, std::string_view needle)
	{
		return (haystack.size() >= needle.size() && !strncmp(needle.data(), haystack.data(), needle.size()));
	}

	/**
	* @brief			open handle to a file within the home-path (root)
	* @param sub_dir	sub directory within home-path (root)
	* @param file_name	the file name
	* @param print		print generic error message when we fail to open a handle
	* @param file		in-out file handle
	* @return			file handle state (valid or not)
	*/
	bool open_file_homepath(const std::string& sub_dir, const std::string& file_name, bool print, std::ifstream& file)
	{
		if (const auto& var = Dvars::Functions::Dvar_FindVar("fs_homepath");
						var)
		{
			std::string	file_path = var->current.string;
			file_path += "\\" + sub_dir + "\\" + file_name;

			file.open(file_path);
			if (!file.is_open())
			{
				if (print)
				{
					Game::Com_Printf(0, "[ERR] Could not open file '%s'.", file_path.c_str());
				}

				return false;
			}

			return true;
		}

		return false;
	}
}