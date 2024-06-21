#pragma once

namespace Components
{
	class RtxMapSettings : public Component
	{
	public:
		RtxMapSettings();

		static inline RtxMapSettings* p_this = nullptr;
		static RtxMapSettings* get() { return p_this; }

		enum PARSE_MODE : std::uint32_t
		{
			SETTINGS,
			CULL,
			MARKER
		};

		struct cell_settings_s
		{
			//int cell_index = -1;
			std::vector<int> forced_cell_indices;
			bool active = false;
		};

		struct marker_settings_s
		{
			float origin[3];
			Game::FxEffect* handle;
			bool active = false;
		};

		struct map_settings_s
		{
			std::string		mapname;
			float			fog_distance = 5000.0f;
			Game::GfxColor	fog_color = { 0x00FFFFFF };
			Game::vec3_t	sun_direction = { 75.0f, -15.0f, -35.0f };
			Game::vec3_t	sun_color = { 255.0f, 255.0f, 255.0f };
			float			sun_intensity = 1.0f;
			int				sky_index = 0;
			std::vector<cell_settings_s> cell_settings;
			bool			cell_overrides_exist = false;
			std::vector<marker_settings_s> map_markers;
		};

		static inline const map_settings_s* settings() { return &m_loaded_map_settings; }
		void set_settings_for_loaded_map(bool reload_settings = false);

	private:
		static inline map_settings_s m_loaded_map_settings = {};
		static inline std::vector<map_settings_s> m_settings;
		static inline std::vector<std::string> m_args;

		map_settings_s* get_or_create_settings();
		void parse_culling();
		void parse_markers();
		void parse_settings();
		bool load_settings();
	};
}
