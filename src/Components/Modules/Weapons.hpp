#pragma once

namespace Components
{
	class Weapons : public Component
	{
	public:
		Weapons();
		~Weapons();
	private:
		static void BG_WeaponFixBurstMode(Game::playerState_s* ps);
		static void BG_FireWeaponStub(Game::playerState_s* playerState, int delayedAction);
		static void FireWeaponStub();
	};
}