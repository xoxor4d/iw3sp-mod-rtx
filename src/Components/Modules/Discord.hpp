#pragma once

namespace Components
{
	class Discord : public Component
	{
	public:
		Discord();
		~Discord();

		static void DiscordUpdate();
		static void	DiscordInit();
		static void	DiscordShutdown();

		static inline bool EnableDiscord_RPC = true;

	private:

	};
}