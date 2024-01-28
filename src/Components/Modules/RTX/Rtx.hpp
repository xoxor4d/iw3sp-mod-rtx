#pragma once

namespace Components
{
	class Rtx : public Component
	{
	public:
		Rtx();

		static inline auto loc_disable_world_culling = 0u;
		static inline auto loc_disable_entity_culling = 0u;
		static inline std::vector rtx_disable_world_culling_enum = { "default", "less", "all", "all-but-models" };

		static void setup_rtx();
		static void r_set_3d();
		static void force_dvars_on_frame();
		static void force_dvars_on_init();
		static void set_dvars_defaults_on_mapload();

		static bool skysphere_is_model_valid();
		static void skysphere_change_model(int variant);
		static void skysphere_spawn(int variant = 0);

		static void on_map_load();
		static void on_map_shutdown();

	private:
		static const char* skysphere_get_name_for_variant(int variant);
		static inline bool skysphere_spawned = false;
		static inline int skysphere_variant = 0;
		static inline Game::gentity_s* skysphere_model = nullptr;

		static inline bool noborder_active = false;
		static inline int  noborder_titlebar_height = 0;
	};
}