#include "STDInc.hpp"

namespace Dvars
{
	// Test
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
	Game::dvar_s* r_noborder = nullptr;
	Game::dvar_s* r_videomode = nullptr;
	Game::dvar_s* ui_nativeCursor = nullptr;
	Game::dvar_s* safeArea_horizontal = nullptr;
	Game::dvar_s* safeArea_vertical = nullptr;

	// d3d9
	Game::dvar_s* r_d3d9ex = nullptr;

	// Field of view dvars
	Game::dvar_s* cg_fovScale = nullptr;
	Game::dvar_s* cg_fovMin = nullptr;
	Game::dvar_s* cg_fov = nullptr;

	Game::dvar_s* ui_debugMode = nullptr;

	// Player settings dvars
	Game::dvar_s* p_allowFire = nullptr;
	Game::dvar_s* player_sprintUnlimited = nullptr;

	Game::dvar_s* external_console = nullptr;

	Game::dvar_s* M_RawInput = nullptr;

	// Gamepad
	Game::dvar_s* gpad_use_hold_time = nullptr;

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

		void Dvar_SetIntByName(const char* dvarName, int value)
		{
			const static uint32_t Dvar_SetIntByName_func = 0x589630;
			__asm
			{
				mov		eax, dvarName;
				push	value;
				call	Dvar_SetIntByName_func;
				add     esp, 4;
			}
		}

		void Dvar_Reset(int value/*eax*/, Game::dvar_s* dvar/*ecx*/)
		{
			const static uint32_t Dvar_Reset_func = 0x5898E0;
			__asm
			{
				mov		eax, value;
				mov		ecx, dvar;
				call	Dvar_Reset_func;
			}
		}
	}

	namespace Register
	{

	}

	namespace Override
	{
		void DvarBoolOverride(const char* dvarName, const bool value, Game::dvar_flags flags)
		{
			auto dvar = Dvars::Functions::Dvar_FindVar(dvarName);

			if (!dvar)
			{
				return;
			}

			dvar->current.enabled = value;
			dvar->latched.enabled = value;
			dvar->flags = flags;
			dvar->modified = false;
		}

		void DvarBoolOverride(const char* dvarName, Game::dvar_flags flags)
		{
			auto dvar = Dvars::Functions::Dvar_FindVar(dvarName);

			if (!dvar)
			{
				return;
			}

			dvar->flags = flags;
			dvar->modified = false;
		}

		// < rtx begin

		void DvarIntOverride(const char* dvarName, const int value, Game::dvar_flags flags)
		{
			if (const auto& dvar = Functions::Dvar_FindVar(dvarName); dvar)
			{
				dvar->current.integer = value;
				dvar->latched.integer = value;
				dvar->flags = flags;
				dvar->modified = false;
			}
		}

		void DvarFloatOverride(const char* dvarName, const float value, Game::dvar_flags flags)
		{
			if (const auto& dvar = Functions::Dvar_FindVar(dvarName); dvar)
			{
				dvar->current.value = value;
				dvar->latched.value = value;
				dvar->flags = flags;
				dvar->modified = false;
			}
		}

		// rtx end >

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