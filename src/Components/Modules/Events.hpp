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
	private:
		static Utils::Signal<Callback> DvarInitSignal;
		static void Com_InitDvars_stub();
	};
}