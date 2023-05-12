#include "STDInc.hpp"

namespace Components
{
	Weapons::Weapons()
	{
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
		
	}

	Weapons::~Weapons()
	{
	}
}