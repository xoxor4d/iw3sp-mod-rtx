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

	void Sys_SnapVector()
	{
		Game::g_clients->predictedPlayerState.velocity[0] = rint(Game::g_clients->predictedPlayerState.velocity[0]);
		Game::g_clients->predictedPlayerState.velocity[1] = rint(Game::g_clients->predictedPlayerState.velocity[1]);
		Game::g_clients->predictedPlayerState.velocity[2] = rint(Game::g_clients->predictedPlayerState.velocity[2]);
	}

	Game::vec_t Vec3Normalize(Game::vec3_t v) {
		float length, ilength;

		length = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
		length = sqrt(length);

		if (length) {
			ilength = 1 / length;
			v[0] *= ilength;
			v[1] *= ilength;
			v[2] *= ilength;
		}

		return length;
	}

	void PM_Accelerate(Game::vec3_t wishdir, Game::playerState_s* ps, Game::pml_t* pml, float wishspeed, float accel)
	{
		double addspeed;
		double value;
		double v8;
		float v9;
		float v10;
		float v11;
		float accelspeed;
		float v14;
		float v15;
		Game::vec3_t v16;

		if (ps->pm_flags & 8)
		{
			v16[0] = wishdir[0] * wishspeed - ps->velocity[0];
			v16[1] = wishdir[1] * wishspeed - ps->velocity[1];
			v16[2] = wishdir[2] * wishspeed - ps->velocity[2];

			v15 = Vec3Normalize(v16);

			v14 = pml->frametime * accel * wishspeed;
			v8 = v14;
			if (v15 < v14)
				v8 = v15;
			ps->velocity[0] = v16[0] * v8 + ps->velocity[0];
			ps->velocity[1] = v16[1] * v8 + ps->velocity[1];
			ps->velocity[2] = v16[2] * v8 + ps->velocity[2];
		}
		else
		{
			v9 = ps->velocity[0] * wishdir[0] + ps->velocity[1] * wishdir[1] + ps->velocity[2] * wishdir[2];
			//v9 = ps->velocity[0] * wishdir[0] + ps->velocity[1] * wishdir[1] + ps->velocity[2] * wishdir[2];
			v10 = wishspeed - v9;
			if (v10 > 0.0)
			{
				if (Dvars::Functions::Dvar_FindVar("stopspeed")->current.value <= wishspeed)
				{
					value = wishspeed;
					addspeed = v10;
				}
				else
				{
					addspeed = v10;
					value = Dvars::Functions::Dvar_FindVar("stopspeed")->current.value;
				}

				v11 = value;
				accelspeed = pml->frametime * accel * value;
				if (accelspeed > addspeed)
					accelspeed = addspeed;

				ps->velocity[0] = wishdir[0] * accelspeed + ps->velocity[0];
				ps->velocity[1] = wishdir[1] * accelspeed + ps->velocity[1];
				ps->velocity[2] = wishdir[2] * accelspeed + ps->velocity[2];
			}
		}
	}

	float PM_MoveScale(Game::playerState_s* ps, float fmove, float rmove, float umove)
	{
		double v4; // st7
		int pm_type; // eax
		float v7; // [esp+4h] [ebp-10h]
		double v8; // [esp+4h] [ebp-10h]
		float scale; // [esp+4h] [ebp-10h]
		float v10; // [esp+10h] [ebp-4h]
		float v11; // [esp+10h] [ebp-4h]
		float result; // [esp+10h] [ebp-4h]

		v7 = fabs(fmove);
		v10 = fabs(rmove);
		if (v7 < v10)
			v7 = v10;
		v11 = fabs(umove);
		if (v7 >= v11)
			v4 = v7;
		else
			v4 = v11;
		if (0.0 == v4)
			return 0.0;
		v8 = v4 * ps->speed;

		result = umove * umove + fmove * fmove + rmove * rmove;
		scale = v8 / (sqrt(result) * 127.0);

		if ((ps->pm_flags & 0x40) != 0 || ps->leanf != 0.0f)
			scale = scale * 0.4000000059604645;
		pm_type = ps->pm_type;
		if (pm_type == 2)
		{
			auto buttons = Game::pmove->cmd.buttons;

			if (buttons & Game::CMD_BUTTON_MELEE || buttons & Game::CMD_BUTTON_SPRINT)
				return scale * 15.0;
			else
				return scale * 3.0;
		}
		if (pm_type == 3)
		{
			auto buttons = Game::pmove->cmd.buttons;

			if (buttons & Game::CMD_BUTTON_MELEE || buttons & Game::CMD_BUTTON_SPRINT)
				return scale * 15.0;
			else
				return scale * 6.0;
		}
		return scale;
	}

	void PM_NoclipMove(Game::pmove_t* pm, Game::pml_t* pml)
	{
		Game::PM_AirMove(pm, pml);

		float z;
		float umovea = 0.0f;
		Game::vec3_t wishdir;


		pm->ps->viewHeightTarget = 60;
		float scale = sqrtf(pm->ps->velocity[0] * pm->ps->velocity[0] + pm->ps->velocity[1] * pm->ps->velocity[1] + pm->ps->velocity[2] * pm->ps->velocity[2]);
		if (scale >= 1.0)
		{
			if (Dvars::Functions::Dvar_FindVar("stopspeed")->current.value > scale)
				scale = Dvars::Functions::Dvar_FindVar("stopspeed")->current.value;
			float smove = Dvars::Functions::Dvar_FindVar("friction")->current.value * 1.5;
			float pmle = scale * smove * pml->frametime + 0.0;
			float pmlb = scale - pmle;
			if (pmlb < 0.0)
				pmlb = 0.0;
			float pmlf = pmlb / scale;
			pm->ps->velocity[0] = pm->ps->velocity[0] * pmlf;
			pm->ps->velocity[1] = pm->ps->velocity[1] * pmlf;
			pm->ps->velocity[2] = pm->ps->velocity[2] * pmlf;
			z = pmlf * pm->ps->velocity[2];
		}
		else
		{
			pm->ps->velocity[0] = 0.0;
			pm->ps->velocity[1] = 0.0;
			z = 0.0;
		}
		pm->ps->velocity[2] = z;

		const float fmove = pm->cmd.forwardmove;
		const float smove = pm->cmd.rightmove;

		//LMB - UP | RMB - DOWN
		auto oldButtons = pm->cmd.buttons;

		if (oldButtons & Game::CMD_BUTTON_ATTACK)
		{
			umovea = 127.0f + 0.0f;
		}
		if (oldButtons & 0x80000) //from mw2
		{
			umovea = umovea - 127.0f;
		}

		float pmlc = PM_MoveScale(pm->ps, fmove, smove, umovea);

		wishdir[0] = pml->up[0] * umovea + pml->forward[0] * fmove + pml->right[0] * smove;
		wishdir[1] = pml->up[1] * umovea + pml->forward[1] * fmove + pml->right[1] * smove;
		wishdir[2] = pml->up[2] * umovea + pml->forward[2] * fmove + pml->right[2] * smove;
		float pmlg = Vec3Normalize(wishdir);

		PM_Accelerate(wishdir, pm->ps, pml, (pmlg * pmlc), 9.0);

		pm->ps->origin[0] = pml->frametime * pm->ps->velocity[0] + pm->ps->origin[0];
		pm->ps->origin[1] = pml->frametime * pm->ps->velocity[1] + pm->ps->origin[1];
		pm->ps->origin[2] = pml->frametime * pm->ps->velocity[2] + pm->ps->origin[2];
	}

	void PM_UFOMove(Game::pmove_t *pm, Game::pml_t *pml)
	{	
		Game::PM_AirMove(pm, pml);

		const float fmove = pm->cmd.forwardmove; //w,s
		const float smove = pm->cmd.rightmove; //a,d
		float umove = 0.0f;

		float z, curFriction, v9, v10;
		Game::vec3_t forward;

		const auto ps = pm->ps;
		ps->viewHeightTarget = 60;

		//LMB - UP | RMB - DOWN
		auto oldButtons = pm->cmd.buttons;

		if (oldButtons & Game::CMD_BUTTON_ATTACK)
		{
			umove = 127.0f + 0.0f;
		}
		if (oldButtons & 0x80000)
		{
			umove = umove - 127.0f;
		}

		float scale = sqrtf( ps->velocity[0] * ps->velocity[0] + ps->velocity[1] * ps->velocity[1] + ps->velocity[2] * ps->velocity[2] );
		if (fmove == 0.0 && smove == 0.0 && umove == 0.0 || scale < 1.0f)
		{
			ps->velocity[0] = 0.0f;
			ps->velocity[1] = 0.0f;
			z = 0.0f;
		}
		else
		{
			if(Dvars::Functions::Dvar_FindVar("stopspeed")->current.value > scale)
				scale = Dvars::Functions::Dvar_FindVar("stopspeed")->current.value;

			curFriction = Dvars::Functions::Dvar_FindVar("friction")->current.value * 1.5;

			v9 = scale * curFriction * pml->frametime + 0.0;
			float pmlb = scale - v9;
			if (pmlb < 0.0f)
				pmlb = 0.0f;
			v10 = pmlb / scale;
			ps->velocity[0] = v10 * ps->velocity[0];
			ps->velocity[1] = v10 * ps->velocity[1];
			ps->velocity[2] = v10 * ps->velocity[2];
			z = v10 * ps->velocity[2];
		}

		ps->velocity[2] = z;
		float pmlf = PM_MoveScale(ps, fmove, smove, umove);

		float v14, v15, v16;

		float v6 = pml->right[2] * 0.0;
		forward[0] = v6 - pml->right[1];
		forward[1] = pml->right[0] - v6;
		forward[2] = pml->right[1] * 0.0 - pml->right[0] * 0.0;
		float v7 = 0.0 * umove;
		v14 = forward[0] * fmove + pml->right[0] * smove + v7;
		v15 = v7 + pml->right[1] * smove + forward[1] * fmove;
		v16 = smove * pml->right[2] + fmove * forward[2] + umove;

		forward[0] = v14;
		forward[1] = v15;
		forward[2] = v16;


		float pmlg = Vec3Normalize(forward);
		PM_Accelerate(forward, ps, pml, (pmlg * pmlf), 9.0);
		float frametime = pml->frametime;
		ps->origin[0] = frametime * ps->velocity[0] + ps->origin[0];
		ps->origin[1] = frametime * ps->velocity[1] + ps->origin[1];
		ps->origin[2] = frametime * ps->velocity[2] + ps->origin[2];
	}

	void __declspec(naked) Pmovesingle_stub()
	{
		const static uint32_t retn_addr = 0x5BD428;
		__asm
		{
			add		esp, 98h;
			call	Sys_SnapVector;
			jmp		retn_addr;
		}
	}

	Movement::Movement()
	{
		//	Int dvar register test
		Game::dvar_s* fixed_pmove = Dvars::Register::Dvar_RegisterInt("fixed_pmove", "", 125, 0, 1000, Game::none);
		Game::dvar_s* pmove_fixed = Dvars::Register::Dvar_RegisterBool("pmove_fixed", "Turn on/off pmove_fixed", true, Game::none);

		Utils::Hook(0x5BD012, PM_UFOMove, HOOK_CALL).install()->quick();
		Utils::Hook(0x5BCFB1, PM_NoclipMove, HOOK_CALL).install()->quick();
 
		Utils::Hook(0x427085, Pmove, HOOK_CALL).install()->quick();
		Utils::Hook(0x4A3A14, Pmove, HOOK_CALL).install()->quick();

		Utils::Hook::Nop(0x5BD039, 5);  // nop the original function (ufo)
		Utils::Hook::Nop(0x5BCFD8, 5);	//nop the original function (noclip)

		Utils::Hook::Nop(0x5BD422, 5);
		Utils::Hook(0x5BD422, Pmovesingle_stub, HOOK_JUMP).install()->quick();
	}

	Movement::~Movement()
	{
	}
}