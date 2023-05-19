#include "STDInc.hpp"

namespace Components
{
	std::unordered_map<std::string, UIScript::UIScriptHandler> UIScript::UIScripts;

	void UIScript::Add(const std::string& name, const UIScriptHandler& callback)
	{
		UIScripts[name] = callback;
	}

	template<> int UIScript::Token::get() const
	{
		if (this->isValid())
		{
			return std::strtol(this->token, nullptr, 0);
		}

		return 0;
	}

	template<> const char* UIScript::Token::get() const
	{
		if (this->isValid())
		{
			return this->token;
		}

		return "";
	}

	template<> std::string UIScript::Token::get() const
	{
		return { this->get<const char*>() };
	}

	bool UIScript::Token::isValid() const
	{
		return (this->token && this->token[0]);
	}

	void UIScript::Token::parse(const char** args)
	{
		if (args)
		{
			this->token = Game::Com_Parse(args);
		}
	}

	int* UI_GetInfo()
	{
		return Game::uiInfo;
	}

	bool UIScript::RunMenuScript(const char* name, const char** args)
	{
		if (const auto itr = UIScripts.find(name); itr != UIScripts.end())
		{
			const auto* info = UI_GetInfo();
			itr->second(Token(args), info);
			return true;
		}

		return false;
	}

	//xoxor4d asm code
	//https://github.com/xoxor4d/iw3xo-dev/blob/develop/src/components/modules/_ui.cpp#L998
	__declspec(naked) void UIScript::UI_RunMenuScript_stub()
	{
		const static char* overwritten_str = "clearError";
		const static uint32_t stock_scripts_addr = 0x566CC9;  // next op after hook spot
		const static uint32_t if_addon_return_addr = 0x5676CE; // jump to the valid return point if we had a valid match in addons
		__asm
		{
			lea     edx, [esp + 30h];

			pushad;
			push	edx;
			call	RunMenuScript;
			add		esp, 4;
			test	al, al;
			je		STOCK_FUNC;				// jump if UI_uiScriptsAddons was false
			popad;

			jmp		if_addon_return_addr;	// return to skip the stock function

		STOCK_FUNC:
			popad;
			push	overwritten_str;		// the original push we hooked at
			jmp		stock_scripts_addr;		// jump back and exec the original function
		}
	}

	UIScript::UIScript()
	{
		// UI_RunMenuScript "clearError" hooking
		Utils::Hook(0x566CC4, UI_RunMenuScript_stub, HOOK_JUMP).install()->quick();
	}

	UIScript::~UIScript()
	{
		UIScripts.clear();
	}
}