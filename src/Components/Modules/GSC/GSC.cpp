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