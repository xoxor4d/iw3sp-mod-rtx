#pragma once

namespace Components
{
	class Rtx : public Component
	{
	public:
		Rtx();

		static void r_set_3d();
		static void force_dvars_on_init();

		static bool skysphere_is_model_valid();
		static void skysphere_change_model(int variant);
		static void skysphere_spawn(int variant = 0);

		/*static inline Game::vec3_t skysphere_model_origin = {};
		static inline Game::vec3_t skysphere_model_rotation = {};*/

	private:
		static const char* skysphere_get_name_for_variant(int variant);
		static inline bool skysphere_spawned = false;
		static inline int skysphere_variant = 0;
		static inline Game::gentity_s* skysphere_model = nullptr;
	};
}