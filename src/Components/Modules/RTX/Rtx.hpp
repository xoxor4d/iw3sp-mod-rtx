#pragma once

namespace Components
{
	class Rtx : public Component
	{
	public:
		Rtx();

		static inline auto loc_culling_tweak_mins = 0u;
		static inline auto loc_culling_tweak_maxs = 0u;
		static inline auto loc_culling_tweak_frustum = 0u;
		static inline auto loc_culling_tweak_smodel = 0u;

		static void setup_rtx();
		static void r_set_3d();
		static void rb_show_tess(Game::GfxCmdBufSourceState* source, Game::GfxCmdBufState* state, const float* center, const char* name, const float* color, Game::DebugGlobals* manual_debug_glob = nullptr);
		static void force_dvars_on_frame();
		static void force_dvars_on_init();
		static void set_dvars_defaults_on_mapload();

		static bool skysphere_is_model_valid();
		static void skysphere_change_model(int variant);
		static void skysphere_spawn(int variant = 0);

		static void on_map_load();
		static void on_map_shutdown();

		enum SKY : std::uint32_t
		{
			CLEAR,
			DESERT,
			GALAXY1,
			NIGHT,
			OVERCAST,
			SUNSET,
			GALAXY2,
			NIGHT2,
			BACKLOT,
			BLOC_VACANT,
			BOG,
			CARENTAN,
			CARGOSHIP,
			CITYSTREETS,
			CONVOY_CROSSFIRE,
			COUNTDOWN,
			CRASH,
			CRASH_SNOW,
			CREEK,
			FARM_PIPELINE,
			OVERGROWN_SHIPMENT,
			SHOWDOWN,
			STRIKE,
			ARGUN,
			COUNT,
		};

		static inline constexpr const char* SKY_STRINGS[] =
		{
			"rtx_skysphere_oceanrock",
			"rtx_skysphere_desert",
			"rtx_skysphere_galaxy01",
			"rtx_skysphere_night",
			"rtx_skysphere_overcast",
			"rtx_skysphere_sunset_clouds",
			"rtx_skysphere_galaxy02",
			"rtx_skysphere_night02",
			"rtx_skysphere_backlot",
			"rtx_skysphere_bloc_vacant",
			"rtx_skysphere_bog",
			"rtx_skysphere_carentan",
			"rtx_skysphere_cargo",
			"rtx_skysphere_citystreets",
			"rtx_skysphere_conv_crossf",
			"rtx_skysphere_countdown",
			"rtx_skysphere_crash",
			"rtx_skysphere_crash_snow",
			"rtx_skysphere_creek",
			"rtx_skysphere_farm_pipe",
			"rtx_skysphere_overg_shipm",
			"rtx_skysphere_showdown",
			"rtx_skysphere_strike",
			"rtx_skysphere_argun"
		};

	private:
		static const char* skysphere_get_name_for_variant(std::uint32_t variant);
		static inline bool skysphere_spawned = false;
		static inline int skysphere_variant = 0;
		static inline Game::gentity_s* skysphere_model = nullptr;

		static inline bool noborder_active = false;
		static inline int  noborder_titlebar_height = 0;
	};
}