#include "STDInc.hpp"

namespace Components
{
	Utils::Signal<Renderer::BackendCallback> Renderer::BackendFrameSignal;
	Utils::Signal<Renderer::BackendCallback> Renderer::SingleBackendFrameSignal;

	Utils::Signal<Renderer::Callback> Renderer::EndRecoverDeviceSignal;
	Utils::Signal<Renderer::Callback> Renderer::BeginRecoverDeviceSignal;


	void Renderer::PreVidRestart()
	{
		Renderer::BeginRecoverDeviceSignal();
	}

	void Renderer::PostVidRestart()
	{
		Renderer::EndRecoverDeviceSignal();
	}

	__declspec(naked) void Renderer::PostPreVidRestart()
	{
		__asm
		{
			pushad
			call Renderer::PreVidRestart
			popad

			push 535E80h
			retn
		}
	}

	__declspec(naked) void Renderer::PostVidRestartStub()
	{
		__asm
		{
			pushad
			call Renderer::PostVidRestart
			popad

			push 445390h
			retn
		}
	}

	bool GetPlayerADS() {return *reinterpret_cast <bool*>(0x720004);}
	float GetCurrentWeaponAdsProgress(){return *reinterpret_cast<float*>(0x714BA8 + 208);}

	//	Reversed function
	double CG_GetViewFov(int localClientNum)
	{
		float calc_fov;

		calc_fov = Dvars::Functions::Dvar_FindVar("cg_fov")->current.value;
		unsigned int offhand_index = Game::g_clients->predictedPlayerState.offHandIndex;
		if ((Game::g_clients->predictedPlayerState.weapFlags & 2) == 0)
		{
			offhand_index = Game::g_clients->predictedPlayerState.weapon;
		}
		const auto weapon = Game::BG_WeaponNames[offhand_index];

		auto check_flags_and_fovmin = [&]() -> float
		{
			if ((Game::g_clients->predictedPlayerState.eFlags & 0x300) != 0)
			{
				if (Game::g_clients->predictedPlayerState.viewlocked)
				{
					//	OMG! We have the function which get the current state of the turret.
					auto func = Utils::Hook::Call<int()>(0x4356B0)();
					if (Game::BG_WeaponNames[func]->overlayInterface == Game::WEAPOVERLAYINTERFACE_TURRETSCOPE)
						return calc_fov = Dvars::Functions::Dvar_FindVar("turretScopeZoom")->current.value;
				}
				return calc_fov = 55.0f;
			}

			calc_fov = Dvars::cg_fovScale->current.value * calc_fov;
			float calc_fov_min = Dvars::cg_fovMin->current.value - calc_fov;

			if (calc_fov_min >= 0.0)
				return Dvars::cg_fovMin->current.value;

			//	From MW2
			if (calc_fov > 170.0f)
				return calc_fov = 170.0f;

			return calc_fov;
		};

		if (Game::g_clients->predictedPlayerState.pm_type == 5)
		{
			calc_fov = 90.0f;
			return check_flags_and_fovmin();
		}

		float adsProgress = GetCurrentWeaponAdsProgress();

		if (weapon->aimDownSight)
		{
			/*
			if (Game::g_clients->predictedPlayerState.fWeaponPosFrac == 1.0f)
			{
				calc_fov = weapon->fAdsZoomFov;
				return check_flags_and_fovmin();
			}
			*/

			if (adsProgress == 1.0)
			{
				calc_fov = weapon->fAdsZoomFov;
				return check_flags_and_fovmin();
			}

			if (0.0f != adsProgress)
			{
				float ads_factor;

				bool playerInAds = GetPlayerADS();

				if (playerInAds)
				{
					const float w_pos_frac = adsProgress - (1.0f - weapon->fAdsZoomInFrac);
					if (w_pos_frac <= 0.0f)
					{
						return check_flags_and_fovmin();
					}
					ads_factor = w_pos_frac / weapon->fAdsZoomInFrac;
				}
				else
				{
					const float w_pos_frac = adsProgress - (1.0f - weapon->fAdsZoomOutFrac);
					if (w_pos_frac <= 0.0f)
					{
						return check_flags_and_fovmin();
					}
					ads_factor = w_pos_frac / weapon->fAdsZoomOutFrac;
				}

				if (ads_factor > 0.0f)
				{
					calc_fov = calc_fov - ads_factor * (calc_fov - weapon->fAdsZoomFov);
				}
			}
		}

		return check_flags_and_fovmin();
	}

	Renderer::Renderer()
	{
		//	Begin device recovery (not D3D9Ex)
		Utils::Hook(0x5DA0A0, []
		{
			Game::DB_BeginRecoverLostDevice();
			Renderer::BeginRecoverDeviceSignal();
		}, HOOK_CALL).install()->quick();

		//	End device recovery (not D3D9Ex)
		Utils::Hook(0x5DA1A3, []
		{
			Renderer::EndRecoverDeviceSignal();
			Game::DB_EndRecoverLostDevice();
		}, HOOK_CALL).install()->quick();

		//	Begin vid_restart
		Utils::Hook(0x444FCF, Renderer::PostPreVidRestart, HOOK_CALL).install()->quick();
		//	End vid_restart
		Utils::Hook(0x4450CD, Renderer::PostVidRestartStub, HOOK_CALL).install()->quick();
		
		Utils::Hook(0x42E1D1, CG_GetViewFov, HOOK_CALL).install()->quick();
	}

	Renderer::~Renderer()
	{
		Renderer::BackendFrameSignal.clear();
		Renderer::SingleBackendFrameSignal.clear();

		Renderer::EndRecoverDeviceSignal.clear();
		Renderer::BeginRecoverDeviceSignal.clear();
	}
}