#include "STDInc.hpp"

namespace Components
{
	void __declspec(naked) FireWeaponStub()
	{
		const static uint32_t offset_return = 0x5C2C96;

		__asm
		{
			push	eax;
			mov		eax, Dvars::p_allowFire;
			cmp		byte ptr[eax + 12], 1;
			pop		eax;

			// disableFire is false
			jne		DISABLE;

			// disableFire is true
			jmp		STOCK;
		DISABLE:
			jmp		offset_return;
		STOCK:
			push	eax;
			call	Game::PM_Weapon_FireWeapon;
			add		esp, 4;
			jmp		offset_return;
		}
	}

	Weapons::Weapons()
	{
		Events::OnDvarInit([]
		{
			Dvars::p_allowFire = Dvars::Register::Dvar_RegisterBool("p_allowFire", "Enables/Disables player fire", true, Game::saved_flag);
		});

		GSC::AddFunction("getweapondamage", []
		{
			if (Game::Scr_GetNumParam() != 1)
			{
				Game::Scr_Error("getweapondamage( <string> weaponName ) requires one parameter");
				return;
			}

			int weaponIndex = Game::BG_FindWeaponIndexForName(Game::Scr_GetString(0));
			const auto weapon = Game::BG_WeaponNames[weaponIndex];
			return Game::Scr_AddInt(weapon->damage);
		}, false);

		GSC::AddFunction("setweapondamage", []
		{
			if (Game::Scr_GetNumParam() != 2)
			{
				Game::Scr_Error("setweapondamage( <string> weaponName, <int> value ) requires two parameters");
				return;
			}

			int weaponIndex = Game::BG_FindWeaponIndexForName(Game::Scr_GetString(0));
			const auto damage_value = Game::Scr_GetInt(1);

			const auto weapon = Game::BG_WeaponNames[weaponIndex];
			weapon->damage = damage_value;
		}, false);
		
		Utils::Hook(0x5C2C91, FireWeaponStub, HOOK_JUMP).install()->quick();
	}

	Weapons::~Weapons()
	{
	}
}