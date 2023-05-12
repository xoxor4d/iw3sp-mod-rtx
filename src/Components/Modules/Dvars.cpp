#include "STDInc.hpp"

namespace Dvars
{
	//	Test
	Game::dvar_s* test_dvar_int = nullptr;
	Game::dvar_s* test_dvar_bool = nullptr;
	Game::dvar_s* test_dvar_string = nullptr;
	Game::dvar_s* test_dvar_enum = nullptr;
	Game::dvar_s* test_dvar_float = nullptr;
	Game::dvar_s* test_dvar_vec2 = nullptr;
	Game::dvar_s* test_dvar_vec3 = nullptr;
	Game::dvar_s* test_dvar_vec4 = nullptr;
	Game::dvar_s* test_dvar_bool_savedflag = nullptr;

	Game::dvar_s* ui_language = nullptr;
	Game::dvar_s* language_first_setting = nullptr;

	//	d3d9
	Game::dvar_s* r_d3d9ex = nullptr;

	//	Field of view dvars
	Game::dvar_s* cg_fovScale = nullptr;
	Game::dvar_s* cg_fovMin = nullptr;
	Game::dvar_s* cg_fov = nullptr;


	namespace Functions
	{
		Dvar_FindVar_t Dvar_FindVar = (Dvar_FindVar_t)0x587B00;

		void Dvar_SetStringByName(const char* dvarName, const char* value)
		{
			const static uint32_t Dvar_SetStringByName_func = 0x589720;
			__asm
			{
				mov		eax, dvarName;
				push	value;
				call	Dvar_SetStringByName_func;
				add     esp, 4;
			}
		}
	}

	namespace Register
	{

	}

	namespace Override
	{
		void DvarVec4Override(const char* dvarName, const float* dvar_value)
		{
			auto dvar = Dvars::Functions::Dvar_FindVar(dvarName);

			if (!dvar)
			{
				return;
			}

			for (auto i = 0; i < 4; i++)
			{
				dvar->current.vector[i] = dvar_value[i];
				dvar->latched.vector[i] = dvar_value[i];
				dvar->modified = false;
			}
		}

		void DvarVec4OverrideDefaultValue(const char* dvarName, const float* dvar_value)
		{
			auto dvar = Dvars::Functions::Dvar_FindVar(dvarName);

			if (!dvar)
			{
				return;
			}

			for (auto i = 0; i < 4; i++)
			{
				dvar->reset.vector[i] = dvar_value[i];
				dvar->modified = false;
			}
		}
	}
}