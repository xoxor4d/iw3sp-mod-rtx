#include "STDInc.hpp"

namespace Components
{
	Utils::Signal<Events::Callback> Events::DvarInitSignal;
	Utils::Signal<Events::Callback> Events::ShutdownSystemTasks;

	void Events::OnDvarInit(const Utils::Slot<Callback>& callback)
	{
		DvarInitSignal.connect(callback);
	}

	void Events::OnVMShutdown(const std::function<void()>& callback)
	{
		ShutdownSystemTasks.connect(callback);
	}

	void Events::Com_InitDvars_stub()
	{
		DvarInitSignal();
		DvarInitSignal.clear();

		Utils::Hook::Call<void()>(0x534630)(); // Com_InitDvars
	}

	void Events::Scr_ShutdownSystem_Hk(unsigned char sys)
	{
		ShutdownSystemTasks();
		ShutdownSystemTasks.clear();

		Utils::Hook::Call<void(unsigned char)>(0x557D00)(sys); // Scr_ShutdownSystem
	}

	Events::Events()
	{
		Utils::Hook(0x534B15, Com_InitDvars_stub, HOOK_CALL).install()->quick();

		Utils::Hook(0x4C5478, Scr_ShutdownSystem_Hk, HOOK_CALL).install()->quick(); // G_LoadGame
		Utils::Hook(0x4B9C13, Scr_ShutdownSystem_Hk, HOOK_CALL).install()->quick(); // G_ShutdownGame
	}

	Events::~Events()
	{
	}
}