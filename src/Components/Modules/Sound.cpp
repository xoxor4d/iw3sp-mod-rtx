#include "STDInc.hpp"

namespace Components
{
	void Sound::SND_SetChannelVolumes(const float* channelvolume, int priority, int fademsec)
	{
		Game::snd_channelvolgroup* channelGroup = &Game::g_snd->channelVolGroups[priority];
		channelGroup->active = 1;

		if (fademsec < 1)
			fademsec = 1;

		if (Game::g_snd->entchannel_count > 0)
		{
			float fadeDurationSec = static_cast<float>(fademsec);

			float* p_goalrate = &channelGroup->channelIndex[0].goalrate;

			for (int chanIndex = 0; chanIndex < Game::g_snd->entchannel_count; ++chanIndex)
			{
				float channelVol = channelvolume[chanIndex];
				float fademsecb = *(float*)((char*)p_goalrate + (unsigned int)Game::g_snd->channelvol - (DWORD)channelGroup - 8);

				if (chanIndex == Game::SND_GetEntChannelFromName("music") || chanIndex == Game::SND_GetEntChannelFromName("musicnopause"))
				{
					*(p_goalrate - 1) = channelVol * Dvars::Functions::Dvar_FindVar("snd_musicVolume")->current.value;
					p_goalrate += 3;
					*(p_goalrate - 5) = fademsecb;
					*(p_goalrate - 3) = ((channelVol * Dvars::Functions::Dvar_FindVar("snd_musicVolume")->current.value) - fademsecb) / fadeDurationSec;
				}
				else if (chanIndex == Game::SND_GetEntChannelFromName("voice") || chanIndex == Game::SND_GetEntChannelFromName("mission") || chanIndex == Game::SND_GetEntChannelFromName("announcer"))
				{
					*(p_goalrate - 1) = channelVol * Dvars::Functions::Dvar_FindVar("snd_voiceVolume")->current.value;
					p_goalrate += 3;
					*(p_goalrate - 5) = fademsecb;
					*(p_goalrate - 3) = ((channelVol * Dvars::Functions::Dvar_FindVar("snd_voiceVolume")->current.value) - fademsecb) / fadeDurationSec;
				}
				else
				{
					*(p_goalrate - 1) = channelVol;
					p_goalrate += 3;
					*(p_goalrate - 5) = fademsecb;
					*(p_goalrate - 3) = (channelVol - fademsecb) / fadeDurationSec;
				}
			}
		}

		if (channelGroup != Game::g_snd->channelvol)
		{
			if (priority + 1 >= 5)
			{
				Game::g_snd->channelvol = &Game::g_snd->channelVolGroups[priority];
			}
			else
			{
				bool* channelVolGroupsActive = &Game::g_snd->channelVolGroups[priority + 1].active;
				while (!*channelVolGroupsActive)
				{
					channelVolGroupsActive += 772;
					if (reinterpret_cast<int>(channelVolGroupsActive) >= reinterpret_cast<int>(&Game::g_snd->entchaninfo[4].name[24]))
					{
						Game::g_snd->channelvol = channelGroup;
						break;
					}
				}
			}
		}
	}

	void __declspec(naked) Sound::SetChannelVolCmd_stub()
	{
		const static uint32_t retn_addr = 0x42913D;
		__asm
		{
			push	esi;
			call	SND_SetChannelVolumes;
			add		esp, 4;
			jmp		retn_addr;
		}
	}

	void __declspec(naked) Sound::UpdateShellShockSound_stub()
	{
		const static uint32_t retn_addr = 0x42A878;
		__asm
		{
			push	esi;
			call	SND_SetChannelVolumes;
			add		esp, 4;
			jmp		retn_addr;
		}
	}

	void __declspec(naked) Sound::PlayerBreathSounds_stub()
	{
		const static uint32_t retn_addr = 0x43378C;
		__asm
		{
			push	esi;
			call	SND_SetChannelVolumes;
			add		esp, 4;
			jmp		retn_addr;
		}
	}

	void Sound::SND_SetChannelVolumesShellshocked()
	{
		if (!(Game::g_clients->ps.pm_flags & Game::PMF_SHELLSHOCKED))
		{
			Game::g_snd->channelvol->channelIndex[Game::SND_GetEntChannelFromName("music")].volume = Dvars::Functions::Dvar_FindVar("snd_musicVolume")->current.value;
			Game::g_snd->channelvol->channelIndex[Game::SND_GetEntChannelFromName("musicnopause")].volume = Dvars::Functions::Dvar_FindVar("snd_musicVolume")->current.value;

			Game::g_snd->channelvol->channelIndex[Game::SND_GetEntChannelFromName("voice")].volume = Dvars::Functions::Dvar_FindVar("snd_voiceVolume")->current.value;
			Game::g_snd->channelvol->channelIndex[Game::SND_GetEntChannelFromName("mission")].volume = Dvars::Functions::Dvar_FindVar("snd_voiceVolume")->current.value;
			Game::g_snd->channelvol->channelIndex[Game::SND_GetEntChannelFromName("announcer")].volume = Dvars::Functions::Dvar_FindVar("snd_voiceVolume")->current.value;
		}
	}

	void Sound::SND_Update_stub()
	{
		Utils::Hook::Call<void()>(0x5CECE0)();
		SND_SetChannelVolumesShellshocked();
	}

	Sound::Sound()
	{
		Utils::Hook(0x429138, Sound::SetChannelVolCmd_stub, HOOK_JUMP).install()->quick();
		Utils::Hook(0x42A873, Sound::UpdateShellShockSound_stub, HOOK_JUMP).install()->quick();
		Utils::Hook(0x433787, Sound::PlayerBreathSounds_stub, HOOK_JUMP).install()->quick();

		Utils::Hook(0x44786C, Sound::SND_Update_stub, HOOK_CALL).install()->quick();
		Utils::Hook(0X5D43C3, Sound::SND_Update_stub, HOOK_CALL).install()->quick();

		Events::OnDvarInit([]
		{
			Game::dvar_s* snd_musicVolume = Dvars::Register::Dvar_RegisterFloat("snd_musicVolume", "options menu music", 0.8f, 0.0f, 1.0f, Game::saved);
			Game::dvar_s* snd_voiceVolume = Dvars::Register::Dvar_RegisterFloat("snd_voiceVolume", "options voice music", 0.8f, 0.0f, 1.0f, Game::saved);
		});
	}

	Sound::~Sound()
	{
	}
}