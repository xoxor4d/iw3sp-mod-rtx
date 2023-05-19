#include "STDInc.hpp"

namespace Components
{
	Utils::Signal<Events::Callback> Events::DvarInitSignal;

	void Events::OnDvarInit(const Utils::Slot<Callback>& callback)
	{
		DvarInitSignal.connect(callback);
	}

	void Events::Com_InitDvars_stub()
	{
		DvarInitSignal();
		DvarInitSignal.clear();

		Utils::Hook::Call<void()>(0x534630)(); // Com_InitDvars
	}

	Events::Events()
	{
		Utils::Hook(0x534B15, Com_InitDvars_stub, HOOK_CALL).install()->quick();
	}

	Events::~Events()
	{
	}
}