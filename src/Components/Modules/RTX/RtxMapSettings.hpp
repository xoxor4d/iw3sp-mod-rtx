#pragma once

namespace Components
{
	class RtxMapSettings : public Component
	{
	public:
		RtxMapSettings();

		static RtxMapSettings* p_this;
		static RtxMapSettings* get();

		static inline float m_max_distance = 5000.0f;
		static inline Game::GfxColor m_color = { 0x00FFFFFF };

		void set_settings_for_loaded_map(bool reload_settings = false);

	private:
		struct settings_s
		{
			std::string mapname;
			int skybox = 0;
			float max_distance = 5000.0f;
			Game::GfxColor m_color = { 0x00FFFFFF };
		};

		static inline std::vector<settings_s> m_settings;
		bool load_settings();
	};
}
