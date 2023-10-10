#pragma once

namespace Components
{
	class Events : public Component
	{
	public:
		typedef void(Callback)();

		Events();
		~Events();

		static void OnDvarInit(const Utils::Slot<Callback>& callback);
		static void OnVMShutdown(const Utils::Slot<Callback>& callback);
	private:
		static Utils::Signal<Callback> DvarInitSignal;
		static Utils::Signal<Callback> ShutdownSystemTasks;

		static void Com_InitDvars_stub();
		static void Scr_ShutdownSystem_Hk(unsigned char sys);
	};
}