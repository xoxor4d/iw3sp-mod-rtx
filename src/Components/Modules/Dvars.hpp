#pragma once

namespace Dvars
{
	// Test
	extern Game::dvar_s* test_dvar_int;
	extern Game::dvar_s* test_dvar_bool;
	extern Game::dvar_s* test_dvar_string;
	extern Game::dvar_s* test_dvar_enum;
	extern Game::dvar_s* test_dvar_float;
	extern Game::dvar_s* test_dvar_vec2;
	extern Game::dvar_s* test_dvar_vec3;
	extern Game::dvar_s* test_dvar_vec4;
	extern Game::dvar_s* test_dvar_bool_savedflag;

	extern Game::dvar_s* ui_language;
	extern Game::dvar_s* language_first_setting;
	extern Game::dvar_s* r_noborder;
	extern Game::dvar_s* ui_nativeCursor;

	// d3d9
	extern Game::dvar_s* r_d3d9ex;

	// Field of view dvars
	extern Game::dvar_s* cg_fovScale;
	extern Game::dvar_s* cg_fovMin;
	extern Game::dvar_s* cg_fov;

	extern Game::dvar_s* ui_debugMode;

	// Player settings dvars
	extern Game::dvar_s* p_allowFire;

	extern Game::dvar_s* external_console;

	namespace Functions
	{
		typedef Game::dvar_s* (__cdecl* Dvar_FindVar_t)(const char* dvarName);
		extern Dvar_FindVar_t Dvar_FindVar;

		void Dvar_SetStringByName(const char* dvarName, const char* value);
		void Dvar_SetIntByName(const char* dvarName, int value);
		void Dvar_Reset(int value/*eax*/, Game::dvar_s* dvar/*ecx*/);
	}

	namespace Register
	{
		static Utils::function<Game::dvar_s* (const char* dvarName, Game::dvar_type typeInt, std::uint16_t flags, const char* description, std::int32_t defaultValue, std::int32_t null1, std::int32_t null2, std::int32_t null3, std::int32_t minValue, std::int32_t maxValue)>
			Dvar_RegisterInt_r = 0x588750;

		inline Game::dvar_s* Dvar_RegisterInt(const char* dvar_name, const char* description, std::int32_t default_value, std::int32_t min_value, std::int32_t max_value, std::uint16_t flags)
		{
			return Dvar_RegisterInt_r(dvar_name, Game::dvar_type::integer, flags, description, default_value, 0, 0, 0, min_value, max_value);
		}

		static Utils::function<Game::dvar_s* (const char* dvar_name, Game::dvar_type type_bool, std::uint16_t flags, const char* description, std::int32_t default_value, std::int32_t null1, std::int32_t null2, std::int32_t null3, std::int32_t null4, std::int32_t null5)>
			Dvar_RegisterBool_r = 0x588750;

		inline Game::dvar_s* Dvar_RegisterBool(const char* dvar_name, const char* description, std::int32_t default_value, std::uint16_t flags)
		{
			return Dvar_RegisterBool_r(dvar_name, Game::dvar_type::boolean, flags, description, default_value, 0, 0, 0, 0, 0);
		}

		static Utils::function<Game::dvar_s* (const char* dvar_name, Game::dvar_type type_enum, std::uint16_t flags, const char* description, std::int32_t default_index, std::int32_t null1, std::int32_t null2, std::int32_t null3, std::int32_t enumSize, const char** enum_data)>
			Dvar_RegisterEnum_r = 0x588750;

		inline Game::dvar_s* Dvar_RegisterEnum(const char* dvar_name, const char* description, std::int32_t default_value, std::int32_t enum_size, const char** enum_data, std::uint16_t flags)
		{
			return Dvar_RegisterEnum_r(dvar_name, Game::dvar_type::enumeration, flags, description, default_value, 0, 0, 0, enum_size, enum_data);
		}

		static Utils::function<Game::dvar_s* (const char* dvar_name, Game::dvar_type type_string, std::uint16_t flags, const char* description, const char* default_value, std::int32_t null1, std::int32_t null2, std::int32_t null3, std::int32_t null4, std::int32_t null5)>
			Dvar_RegisterString_r = 0x588750;

		inline Game::dvar_s* Dvar_RegisterString(const char* dvar_name, const char* description, const char* default_value, std::uint16_t flags)
		{
			const auto dvar = Dvars::Functions::Dvar_FindVar(dvar_name);
			if (!dvar)
			{
				return Dvar_RegisterString_r(dvar_name, Game::dvar_type::string, flags, description, default_value, 0, 0, 0, 0, 0);
			}

			return dvar;
		}

		static Utils::function<Game::dvar_s* (const char* dvar_name, Game::dvar_type type_float2, std::uint16_t flags, const char* description, float x, float y, std::int32_t null1, std::int32_t null2, float min, float max)>
			Dvar_RegisterVec2_r = 0x588750;

		inline Game::dvar_s* Dvar_RegisterVec2(const char* dvar_name, const char* description, float x, float y, float min_value, float max_value, std::uint16_t flags) {
			return Dvar_RegisterVec2_r(dvar_name, Game::dvar_type::vec2, flags, description, x, y, 0, 0, min_value, max_value);
		}

		static Utils::function<Game::dvar_s* (const char* dvar_name, Game::dvar_type type_float3, std::uint16_t flags, const char* description, float x, float y, float z, std::int32_t null, float min, float max)>
			Dvar_RegisterVec3_r = 0x588750;

		inline Game::dvar_s* Dvar_RegisterVec3(const char* dvar_name, const char* description, float x, float y, float z, float min_value, float max_value, std::uint16_t flags) {
			return Dvar_RegisterVec3_r(dvar_name, Game::dvar_type::vec3, flags, description, x, y, z, 0, min_value, max_value);
		}

		static Utils::function<Game::dvar_s* (const char* dvar_name, Game::dvar_type type_float4, std::uint16_t flags, const char* description, float x, float y, float z, float w, float min, float max)>
			Dvar_RegisterVec4_r = 0x588750;

		inline Game::dvar_s* Dvar_RegisterVec4(const char* dvar_name, const char* description, float x, float y, float z, float w, float min_value, float max_value, std::uint16_t flags) {
			return Dvar_RegisterVec4_r(dvar_name, Game::dvar_type::vec4, flags, description, x, y, z, w, min_value, max_value);
		}

		static Utils::function<Game::dvar_s* (const char* dvar_name, Game::dvar_type type_float2, std::uint16_t flags, const char* description, float x, float y, std::int32_t null1, std::int32_t null2, float min, float max)>
			Dvar_RegisterFloat_r = 0x588750;

		inline Game::dvar_s* Dvar_RegisterFloat(const char* dvar_name, const char* description, float default_value, float min_value, float max_value, std::uint16_t flags) {
			return Dvar_RegisterFloat_r(dvar_name, Game::dvar_type::value, flags, description, default_value, 0, 0, 0, min_value, max_value);
		}

		static Utils::function<Game::dvar_s* (const char* dvar_name, Game::dvar_type type, std::uint16_t flags, const char* description, int x, int y, int z, int w, int min, int max)>
			Dvar_RegisterNew = 0x588750;
	}

	namespace Override
	{
		void DvarBoolOverride(const char* dvarName, const bool value, Game::dvar_flags flags);
		void DvarBoolOverride(const char* dvarName, Game::dvar_flags flags);
		void DvarVec4Override(const char* dvarName, const float* dvar_value);
		void DvarVec4OverrideDefaultValue(const char* dvarName, const float* dvar_value);
	}
}