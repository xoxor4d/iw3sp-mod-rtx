#pragma once

namespace Components
{
	class UIScript : public Component
	{
	public:
		UIScript();
		~UIScript();

		class Token
		{
		public:
			Token() : token(nullptr) {}
			Token(const char** args) : token(nullptr) { this->parse(args); }
			Token(const Token& obj) { this->token = obj.token; }

			template<typename T> T get() const;
			bool isValid() const;

		private:
			char* token;

			void parse(const char** args);
		};

		using UIScriptHandler = std::function<void(const Token& token, const Game::uiInfo_s* info)>;

		//int ownerDraw, float x, float y, float w, float h, int horzAlign, int vertAlign
		using UIOwnerDrawHandler = std::function<void(float x, float y, float w, float h, int horzAlign, int vertAlign, float text_x, float text_y, float scale, 
			Game::Font_s* font, const float* color, Game::Material* material, int textStyle, int textAlignMode)>;

		static Game::uiInfo_s* UI_GetInfo();

		static void Add(const std::string& name, const UIScriptHandler& callback);
		static void AddOwnerDraw(int ownerdraw, const UIOwnerDrawHandler& callback);

		static bool RunMenuScript(const char* name, const char** args);
	private:
		static void OwnerDrawInit(int ownerDraw, float x, float y, float w, float h, int horzAlign, int vertAlign, float text_x, float text_y, float scale, 
			Game::Font_s* font, const float* color, Game::Material* material, int textStyle, int textAlignMode);
		static void OwnerDrawStub();

		static void UI_RunMenuScript_stub();

		static std::unordered_map<std::string, UIScriptHandler> UIScripts;
		static std::unordered_map<int, UIOwnerDrawHandler> UIOwnerDraws;
	};
}