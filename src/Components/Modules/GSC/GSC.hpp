#pragma once

namespace Components
{
	class GSC : public Component
	{
	public:
		static void AddFunction(const char*, Game::xfunction_t, int = 0);
		static void AddMethod(const char*, Game::xmethod_t, int = 0);
		GSC();
		~GSC();
	private:
		static std::unordered_map<std::string, Game::scr_function_t> CustomScrFunctions;
		static std::unordered_map<std::string, Game::scr_method_t> CustomScrMethods;
		static Game::xmethod_t Player_GetMethod_Stub(const char**);
		static Game::xfunction_t Scr_GetFunction_Stub(const char**, int*);
		static void AddFunctions();
		static void AddMethods();
	};
}