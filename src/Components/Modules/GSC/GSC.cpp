#include "STDInc.hpp"

namespace Components
{
	std::unordered_map<std::string, Game::scr_function_t> GSC::CustomScrFunctions;
	std::unordered_map<std::string, Game::scr_method_t> GSC::CustomScrMethods;

	void GSC::AddFunction(const char* name, Game::xfunction_t func, int type)
	{
		Game::scr_function_t toAdd;
		toAdd.function = func;
		toAdd.developer = type;

		GSC::CustomScrFunctions.insert_or_assign(Utils::String::ToLower(name), toAdd);
	}

	void GSC::AddMethod(const char* name, Game::xmethod_t method, int developer)
	{
		Game::scr_method_t toAdd;
		toAdd.function = method;
		toAdd.developer = developer;

		GSC::CustomScrMethods.insert_or_assign(Utils::String::ToLower(name), toAdd);
	}

	void GSC::AddMethods()
	{
		GSC::AddMethod("SwitchToWeaponFast", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);
			int weaponIndex = Game::G_GetWeaponIndexForName(Game::Scr_GetString(0));
			ent->client->ps.weapon = weaponIndex;
			Game::SV_GameSendServerCommand(-1, Utils::String::VA("sw %d", weaponIndex));
		}, false);

		GSC::AddMethod("allowFire", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			if (Game::Scr_GetInt(0))
			{
				Dvars::p_allowFire->current.enabled = true;
				Dvars::p_allowFire->latched.enabled = true;
			}
			else
			{
				Dvars::p_allowFire->current.enabled = false;
				Dvars::p_allowFire->latched.enabled = false;
			}
		}, false);

		GSC::AddMethod("IsReloading", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			if (ent->client->ps.weaponstate >= Game::WEAPON_RELOADING && ent->client->ps.weaponstate <= Game::WEAPON_RELOAD_END)
				return Game::Scr_AddInt(true);
			else
				return Game::Scr_AddInt(false);
		}, false);

		GSC::AddMethod("IsSprinting", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);
			if (ent->client->ps.pm_flags & Game::PMF_SPRINTING)
				return Game::Scr_AddInt(true);
			else
				return Game::Scr_AddInt(false);
		}, false);

		GSC::AddMethod("IsUsingNVG", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);
			if (ent->client->ps.weaponstate >= Game::WEAPON_NIGHTVISION_WEAR && ent->client->ps.weaponstate <= Game::WEAPON_NIGHTVISION_REMOVE)
				return Game::Scr_AddInt(true);
			else
				return Game::Scr_AddInt(false);
		}, false);

		GSC::AddMethod("isMantling", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);
			if (ent->client->ps.pm_flags & Game::PMF_MANTLE)
				return Game::Scr_AddInt(true);
			else
				return Game::Scr_AddInt(false);
		}, false);

		GSC::AddMethod("isSwapping", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);
			if (ent->client->ps.weaponstate >= Game::WEAPON_DROPPING && ent->client->ps.weaponstate <= Game::WEAPON_DROPPING_QUICK)
				return Game::Scr_AddInt(true);
			else
				return Game::Scr_AddInt(false);
		}, false);
	}

	void GSC::AddFunctions()
	{
		GSC::AddFunction("exec", []
		{
			const auto str = Game::Scr_GetString(0);

			if (str == nullptr)
			{
				Game::Scr_Error("^1Exec: Illegal parameter!\n");
				return;
			}

			Command::Execute(str);
		}, false);

		GSC::AddFunction("ToUpper", []
		{
			const auto str = Game::Scr_GetString(0);

			if (str == nullptr)
			{
				Game::Scr_Error("^1Scr_GetString: Illegal parameter!\n");
				return;
			}

			return Game::Scr_AddString(Utils::String::ToUpper(str).c_str());
		}, false);
	}

	Game::xmethod_t GSC::Player_GetMethod_Stub(const char** name)
	{
		auto got = GSC::CustomScrMethods.find(*name);

		if (got == GSC::CustomScrMethods.end())
			return Game::Player_GetMethod(name);

		return got->second.function;
	}

	Game::xfunction_t GSC::Scr_GetFunction_Stub(const char** name, int* isDev)
	{
		auto got = GSC::CustomScrFunctions.find(*name);

		if (got == GSC::CustomScrFunctions.end())
			return Game::Scr_GetFunction(name, isDev);

		*isDev = got->second.developer;
		return got->second.function;
	}

	GSC::GSC()
	{
		Utils::Hook(0x4DB7D7, Player_GetMethod_Stub, HOOK_CALL).install()->quick(); // Player_GetMethod
		Utils::Hook(0x5435F1, Scr_GetFunction_Stub, HOOK_CALL).install()->quick();  // Scr_GetFunction

		GSC::AddFunctions();
		GSC::AddMethods();
	}


	GSC::~GSC()
	{
		CustomScrMethods.clear();
		CustomScrFunctions.clear();
	}
}