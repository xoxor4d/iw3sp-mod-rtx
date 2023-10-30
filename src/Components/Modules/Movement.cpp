#include "STDInc.hpp"

namespace Components
{
	void Pmove(Game::pmove_t* pm)
	{
		const int server_time = pm->cmd.serverTime;
		const int command_time = pm->ps->commandTime;

		const int fixed_pmove = Dvars::Functions::Dvar_FindVar("fixed_pmove")->current.integer;

		if ((pm->ps->pm_flags & 0x800) != 0)
		{
			pm->cmd.buttons &= 0x300u;
		}
		else
		{
			if ((pm->ps->pm_flags & 0x400) == 0)
				goto CONT;
			pm->cmd.buttons &= 0x301u;
		}

		CONT:
		// stock pmove
		if (server_time >= command_time)
		{
			if (server_time > command_time + 1000)
			{
				pm->ps->commandTime = server_time - 1000;
			}

			pm->numtouch = 0;
			Game::pml_t* pml = (Game::pml_t*)pm;
			if (pm->ps->commandTime != server_time)
			{
				while (true)
				{
					int msec = server_time - pm->ps->commandTime;

					if (Dvars::Functions::Dvar_FindVar("pmove_fixed")->current.enabled)
					{
						pml->frametime = 0;
						if (msec > fixed_pmove) 
						{
							msec = fixed_pmove;
						}
					}
					else 
					{
						if (msec > 66)
						{
							msec = 66;
						}
					}

					pm->cmd.serverTime = pm->ps->commandTime + msec;
					Utils::Hook::Call<void(Game::pmove_t*)>(0x5BCBB0)(pm);
					memcpy(&pm->oldcmd, &pm->cmd, sizeof(pm->oldcmd));
					if (pm->ps->commandTime == server_time)
					{
						break;
					}
				}
			}
		}
	}

	void Movement::PM_Accelerate(Game::vec3_t wishdir, Game::playerState_s* ps, Game::pml_t* pml, float wishspeed, float accel)
	{
		if (ps->pm_flags & 8)
		{
			Game::vec3_t pushDir = {
				wishdir[0] * wishspeed - ps->velocity[0],
				wishdir[1] * wishspeed - ps->velocity[1],
				wishdir[2] * wishspeed - ps->velocity[2]
			};

			float pushLen = Game::Vec3Normalize(pushDir);
			float canPush = pml->frametime * accel * wishspeed;
			if (canPush > pushLen)
				canPush = pushLen;

			ps->velocity[0] = pushDir[0] * canPush + ps->velocity[0];
			ps->velocity[1] = pushDir[1] * canPush + ps->velocity[1];
			ps->velocity[2] = pushDir[2] * canPush + ps->velocity[2];
		}
		else
		{
			float currentspeed = ps->velocity[0] * wishdir[0] + ps->velocity[1] * wishdir[1] + ps->velocity[2] * wishdir[2];
			float addspeed = wishspeed - currentspeed;

			if (addspeed > 0.0f)
			{
				float stopspeed = Dvars::Functions::Dvar_FindVar("stopspeed")->current.value;
				float value = (stopspeed <= wishspeed) ? wishspeed : stopspeed;
				float control = value;
				float accelspeed = pml->frametime * accel * value;
				if (accelspeed > addspeed)
					accelspeed = addspeed;

				ps->velocity[0] = wishdir[0] * accelspeed + ps->velocity[0];
				ps->velocity[1] = wishdir[1] * accelspeed + ps->velocity[1];
				ps->velocity[2] = wishdir[2] * accelspeed + ps->velocity[2];
			}
		}
	}

	float Movement::PM_MoveScale(Game::playerState_s* ps, float fmove, float rmove, float umove)
	{
		float maxMove = std::max({ std::abs(fmove), std::abs(rmove), std::abs(umove) });
		if (maxMove == 0.0f)
			return 0.0f;

		float scale = (ps->speed * maxMove) / (127.0f * sqrt(umove * umove + fmove * fmove + rmove * rmove));
		scale *= ((ps->pm_flags & 0x40) || ps->leanf != 0.0f) ? 0.4000000059604645f : 1.0f;

		if (ps->pm_type == Game::PM_TYPE_NOCLIP)
			return (Game::pmove->cmd.buttons & (Game::CMD_BUTTON_MELEE | Game::CMD_BUTTON_SPRINT)) ? scale * 15.0f : scale * 3.0f;

		if (ps->pm_type == Game::PM_TYPE_UFO)
			return (Game::pmove->cmd.buttons & (Game::CMD_BUTTON_MELEE | Game::CMD_BUTTON_SPRINT)) ? scale * 15.0f : scale * 6.0f;

		return scale;
	}

	void Movement::PM_NoclipMove(Game::pmove_t* pm, Game::pml_t* pml)
	{
		Game::PM_AirMove(pm, pml);

		pm->ps->viewHeightTarget = 60;
		float speed = sqrtf(pm->ps->velocity[0] * pm->ps->velocity[0] + pm->ps->velocity[1] * pm->ps->velocity[1] + pm->ps->velocity[2] * pm->ps->velocity[2]);
		if (speed >= 1.0)
		{
			if (Dvars::Functions::Dvar_FindVar("stopspeed")->current.value > speed)
				speed = Dvars::Functions::Dvar_FindVar("stopspeed")->current.value;
			float curFriction = Dvars::Functions::Dvar_FindVar("friction")->current.value * 1.5f;
			float drop = (speed * curFriction * pml->frametime) + 0.0f;
			float newspeed = speed - drop;
			if (newspeed < 0.0f)
				newspeed = 0.0f;
			pm->ps->velocity[0] = pm->ps->velocity[0] * (newspeed / speed);
			pm->ps->velocity[1] = pm->ps->velocity[1] * (newspeed / speed);
			pm->ps->velocity[2] = pm->ps->velocity[2] * (newspeed / speed);
		}
		else
		{
			pm->ps->velocity[0] = 0.0f;
			pm->ps->velocity[1] = 0.0f;
			pm->ps->velocity[2] = 0.0f;
		}

		//LMB - UP | RMB - DOWN
		const float fmove = pm->cmd.forwardmove;
		const float smove = pm->cmd.rightmove;
		float umove = (pm->cmd.buttons & Game::CMD_BUTTON_ATTACK) ? 127.0f : 0.0f;
		umove -= (pm->cmd.buttons & Game::CMD_BUTTON_ADS_PC) ? 127.0f : 0.0f;

		Game::vec3_t wishdir = 
		{
			pml->up[0] * umove + pml->forward[0] * fmove + pml->right[0] * smove,
			pml->up[1] * umove + pml->forward[1] * fmove + pml->right[1] * smove,
			pml->up[2] * umove + pml->forward[2] * fmove + pml->right[2] * smove
		};

		PM_Accelerate(wishdir, pm->ps, pml, (Game::Vec3Normalize(wishdir) * PM_MoveScale(pm->ps, fmove, smove, umove)), 9.0f);
		pm->ps->origin[0] = pml->frametime * pm->ps->velocity[0] + pm->ps->origin[0];
		pm->ps->origin[1] = pml->frametime * pm->ps->velocity[1] + pm->ps->origin[1];
		pm->ps->origin[2] = pml->frametime * pm->ps->velocity[2] + pm->ps->origin[2];
	}

	void Movement::PM_UFOMove(Game::pmove_t *pm, Game::pml_t *pml)
	{	
		Game::PM_AirMove(pm, pml);

		const float fmove = pm->cmd.forwardmove;
		const float smove = pm->cmd.rightmove;
		const float umove = (pm->cmd.buttons & Game::CMD_BUTTON_ATTACK) ? 127.0f : (pm->cmd.buttons & Game::CMD_BUTTON_ADS_PC) ? -127.0f : 0.0f;

		pm->ps->viewHeightTarget = 60;
		float speed = sqrtf(pm->ps->velocity[0] * pm->ps->velocity[0] + pm->ps->velocity[1] * pm->ps->velocity[1] + pm->ps->velocity[2] * pm->ps->velocity[2]);

		if (fmove == 0.0f && smove == 0.0f && umove == 0.0f || speed < 1.0f)
		{
			pm->ps->velocity[0] = 0.0f;
			pm->ps->velocity[1] = 0.0f;
			pm->ps->velocity[2] = 0.0f;
		}
		else
		{
			if (Dvars::Functions::Dvar_FindVar("stopspeed")->current.value > speed)
				speed = Dvars::Functions::Dvar_FindVar("stopspeed")->current.value;

			float curFriction = Dvars::Functions::Dvar_FindVar("friction")->current.value * 1.5f;
			float drop = (speed * curFriction * pml->frametime) + 0.0f;
			float newspeed = speed - drop;
			if (newspeed < 0.0f)
				newspeed = 0.0f;
			pm->ps->velocity[0] = pm->ps->velocity[0] * (newspeed / speed);
			pm->ps->velocity[1] = pm->ps->velocity[1] * (newspeed / speed);
			pm->ps->velocity[2] = pm->ps->velocity[2] * (newspeed / speed);
		}

		float scale = PM_MoveScale(pm->ps, fmove, smove, umove);
		Game::vec3_t wishdir =
		{
			pml->up[0] * umove + pml->forward[0] * fmove + pml->right[0] * smove,
			pml->up[1] * umove + pml->forward[1] * fmove + pml->right[1] * smove,
			pml->up[2] * umove + pml->forward[2] * fmove + pml->right[2] * smove
		};

		PM_Accelerate(wishdir, pm->ps, pml, (Game::Vec3Normalize(wishdir) * PM_MoveScale(pm->ps, fmove, smove, umove)), 9.0f);
		pm->ps->origin[0] = pml->frametime * pm->ps->velocity[0] + pm->ps->origin[0];
		pm->ps->origin[1] = pml->frametime * pm->ps->velocity[1] + pm->ps->origin[1];
		pm->ps->origin[2] = pml->frametime * pm->ps->velocity[2] + pm->ps->origin[2];
	}

	void __declspec(naked) Pmovesingle_stub()
	{
		const static uint32_t retn_addr = 0x5BD428;
		__asm
		{
			add		esp, 98h;
			call	Game::Sys_SnapVector;
			jmp		retn_addr;
		}
	}

	void __declspec(naked) Movement::PM_GetSprintLeft_stub()
	{
		const static uint32_t stock_retn_addr = 0x5B779F;
		const static uint32_t unlimited_speed_jump = 0x5B77A3;
		__asm
		{
			mov		edi, [esi + 5ACh]; //lastSprintStart = ps->sprintState.lastSprintStart;

			push	eax;
			mov		eax, Dvars::player_sprintUnlimited;
			cmp		byte ptr[eax + 12], 1;
			pop		eax;

			// player_sprintUnlimited is false
			jne		DISABLE;

			// player_sprintUnlimited is true
			jmp		ENABLE;
		DISABLE:
			jmp		stock_retn_addr;
		ENABLE:
			jmp		unlimited_speed_jump;
		}
	}

	void __declspec(naked) Movement::PM_GetSprintLeftLastTime_stub()
	{
		const static uint32_t stock_retn_addr = 0x5B7855;
		const static uint32_t unlimited_speed_jump = 0x5B7881;
		__asm
		{
			cmp     eax, 3FFFh; //MaxSprintTime = (int)v4;

			push	eax;
			mov		eax, Dvars::player_sprintUnlimited;
			cmp		byte ptr[eax + 12], 1;
			pop		eax;

			// player_sprintUnlimited is false
			jne		DISABLE;

			// player_sprintUnlimited is true
			jmp		ENABLE;
		DISABLE:
			jmp		stock_retn_addr;
		ENABLE:
			jmp		unlimited_speed_jump;
		}
	}

	void __declspec(naked) Movement::PM_UpdateSprint_stub()
	{
		const static uint32_t continue_offset = 0x5B7AC6;
		const static uint32_t jmp_offset = 0x5B7AE8;

		__asm
		{
			push	eax;
			mov		eax, Dvars::player_sprintUnlimited;
			cmp		byte ptr[eax + 12], 1;
			pop		eax;	

			je		ENABLE;

			mov     edx, [ebp + 4];
			fstp    st;
			jmp		continue_offset;

		ENABLE:
			jmp		jmp_offset;
		}
	}

	Movement::Movement()
	{
		Events::OnDvarInit([]
		{
			Dvars::player_sprintUnlimited = Dvars::Register::Dvar_RegisterBool("player_sprintUnlimited", "Whether player can sprint forever or not", false, Game::saved_flag);
		});

		GSC::AddMethod("UnlimitedSprint", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			if (Game::Scr_GetInt(0))
			{
				Dvars::player_sprintUnlimited->current.enabled = true;
				Dvars::player_sprintUnlimited->latched.enabled = true;
			}
			else
			{
				Dvars::player_sprintUnlimited->current.enabled = false;
				Dvars::player_sprintUnlimited->latched.enabled = false;
			}
		}, false);

		//	Int dvar register test
		//Game::dvar_s* fixed_pmove = Dvars::Register::Dvar_RegisterInt("fixed_pmove", "", 125, 0, 1000, Game::none);
		//Game::dvar_s* pmove_fixed = Dvars::Register::Dvar_RegisterBool("pmove_fixed", "Turn on/off pmove_fixed", true, Game::none);

		Utils::Hook(0x5BD012, PM_UFOMove, HOOK_CALL).install()->quick();
		Utils::Hook(0x5BCFB1, PM_NoclipMove, HOOK_CALL).install()->quick();
 
		//Utils::Hook(0x427085, Pmove, HOOK_CALL).install()->quick();
		//Utils::Hook(0x4A3A14, Pmove, HOOK_CALL).install()->quick();

		Utils::Hook::Nop(0x5BD039, 5);  // nop the original function (ufo)
		Utils::Hook::Nop(0x5BCFD8, 5);	//nop the original function (noclip)

		//Add for cj mode only
		//Utils::Hook::Nop(0x5BD422, 5);
		//Utils::Hook(0x5BD422, Pmovesingle_stub, HOOK_JUMP).install()->quick();

		//Unlimited sprint
		Utils::Hook(0x5B7799, PM_GetSprintLeft_stub, HOOK_JUMP).install()->quick();
		Utils::Hook::Nop(0x5B7850, 5);
		Utils::Hook(0x5B7850, PM_GetSprintLeftLastTime_stub, HOOK_JUMP).install()->quick();
		Utils::Hook::Nop(0x5B7AC1, 5);
		Utils::Hook(0x5B7AC1, PM_UpdateSprint_stub, HOOK_JUMP).install()->quick();
	}

	Movement::~Movement()
	{
	}
}