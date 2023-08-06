#include "STDInc.hpp"

namespace Components
{
	void Weapons::BG_WeaponFixBurstMode(Game::playerState_s* ps)
	{
		Game::WeaponDef_s* weapon = Game::BG_WeaponNames[ps->weapon];

		if (weapon->fireType >= Game::WEAPON_FIRETYPE_BURSTFIRE2 && weapon->fireType <= Game::WEAPON_FIRETYPE_BURSTFIRE4 && !Game::ShotLimitReached(weapon, ps))
		{
			if (ps->ammo[weapon->iAmmoIndex] <= 0)
			{
				if (!ps->ammoclip[weapon->iClipIndex])
				{
					ps->weaponShotCount = 0;
				}
			}
		}
	}

	void Weapons::BG_FireWeaponStub(Game::playerState_s* playerState, int delayedAction)
	{
		Game::PM_Weapon_FireWeapon(playerState, delayedAction);
		Weapons::BG_WeaponFixBurstMode(playerState);
	}

	void __declspec(naked) Weapons::FireWeaponStub()
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
			call	Weapons::BG_FireWeaponStub;
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

			int weaponIndex = Game::G_GetWeaponIndexForName(Game::Scr_GetString(0));
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

			int weaponIndex = Game::G_GetWeaponIndexForName(Game::Scr_GetString(0));
			const auto damage_value = Game::Scr_GetInt(1);

			const auto weapon = Game::BG_WeaponNames[weaponIndex];
			weapon->damage = damage_value;
		}, false);
		
		Utils::Hook(0x5C2C91, Weapons::FireWeaponStub, HOOK_JUMP).install()->quick();
	}

	Weapons::~Weapons()
	{
	}
}