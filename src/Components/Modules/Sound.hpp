#pragma once

namespace Components
{
	class Sound : public Component
	{
	public:
		Sound();
		~Sound();
	private:
		static void SND_SetChannelVolumes(const float* channelvolume, int priority, int fademsec);
		static void UpdateShellShockSound_stub();
		static void PlayerBreathSounds_stub();
		static void SetChannelVolCmd_stub();
		static void SND_Update_stub();
		static void SND_SetChannelVolumesShellshocked();
	};
}