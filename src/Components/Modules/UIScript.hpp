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

		using UIScriptHandler = std::function<void(const Token& token, const int* info)>;

		static void Add(const std::string& name, const UIScriptHandler& callback);
		static bool RunMenuScript(const char* name, const char** args);
		static void UI_RunMenuScript_stub();
	private:
		static std::unordered_map<std::string, UIScriptHandler> UIScripts;
	};
}