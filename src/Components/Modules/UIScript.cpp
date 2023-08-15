#include "STDInc.hpp"

namespace Components
{
	std::unordered_map<std::string, UIScript::UIScriptHandler> UIScript::UIScripts;
	std::unordered_map<int, UIScript::UIOwnerDrawHandler> UIScript::UIOwnerDraws;

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

	Game::uiInfo_s* UIScript::UI_GetInfo()
	{
		return Game::uiInfo;
	}

	void UIScript::Add(const std::string& name, const UIScriptHandler& callback)
	{
		UIScripts[name] = callback;
	}

	void UIScript::AddOwnerDraw(int ownerdraw, const UIOwnerDrawHandler& callback)
	{
		UIOwnerDraws[ownerdraw] = callback;
	}

	bool UIScript::RunMenuScript(const char* name, const char** args)
	{
		if (const auto itr = UIScripts.find(name); itr != UIScripts.end())
		{
			const auto* info = UIScript::UI_GetInfo();
			itr->second(Token(args), info);
			return true;
		}

		return false;
	}

	void UIScript::OwnerDrawInit(int ownerDraw, float x, float y, float w, float h, int horzAlign, int vertAlign, float text_x, float text_y, float scale, Game::Font_s* font, const float* color,
		Game::Material* material, int textStyle, int textAlignMode)
	{
		for (auto i = UIOwnerDraws.begin(); i != UIOwnerDraws.end(); ++i)
		{
			if (i->first == ownerDraw)
			{
				i->second(x, y, w, h, horzAlign, vertAlign, text_x, text_y, scale, font, color, material, textStyle, textAlignMode);
			}	
		}
	}

	__declspec(naked) void UIScript::OwnerDrawStub()
	{
		const static uint32_t stock_scripts_addr = 0x56590E;

		__asm
		{
			pushad;
			push	[esp + 7Ch];			//textAlignMode
			push	[esp + 7Ch];			//textStyle
			push	[esp + 7Ch];			//material
			push	ebp;					//color
			push	[esp + 78h];			//font
			push	[esp + 80h];			//scale
			push	[esp + 7Ch];			//text_y
			push	[esp + 7Ch];			//text_x
			push    edi;					//vertAlign
			push    ebx;					//horzAlign
			push	[esp + 80h];			//h
			push	[esp + 80h];			//w
			push	[esp + 80h];			//y
			push	[esp + 80h];			//x
			push	esi;					//ownerDraw
			call	UIScript::OwnerDrawInit;
			add		esp, 0x3C;
			popad;

			//original function
			sub     esi, 0FAh;
			jmp		stock_scripts_addr;
		}
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
			push	esi;					//token
			push	edx;					//uiscript_name
			call	RunMenuScript;
			add		esp, 8;
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
		UIScript::AddOwnerDraw(197, []([[maybe_unused]] float x, [[maybe_unused]] float y, [[maybe_unused]] float w, [[maybe_unused]] float h, 
			[[maybe_unused]] int horzAlign, [[maybe_unused]] int vertAlign, [[maybe_unused]] float text_x, [[maybe_unused]] float text_y, [[maybe_unused]] float scale,
			[[maybe_unused]] Game::Font_s* font, [[maybe_unused]] const float* color, [[maybe_unused]] Game::Material* material, [[maybe_unused]] int textStyle,
			[[maybe_unused]] int textAlignMode)
		{			
			auto* const scrPlace = Game::ScrPlace_GetFullPlacement();
			Game::UI_DrawText(scrPlace, std::string("Signed in as: "s + Utils::IO::ReadFile("players/profiles/active.txt")).c_str(), 128, font, x, y, horzAlign, vertAlign, scale, color, textStyle);
		});

		// UI_RunMenuScript "clearError" hooking
		Utils::Hook(0x566CC4, UI_RunMenuScript_stub, HOOK_JUMP).install()->quick();
		// OwnerDraw hooking
		Utils::Hook(0x565908, OwnerDrawStub, HOOK_JUMP).install()->quick();
	}

	UIScript::~UIScript()
	{
		UIScripts.clear();
		UIOwnerDraws.clear();
	}
}