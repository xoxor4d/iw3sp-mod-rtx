#include "STDInc.hpp"

namespace Components
{
	constexpr auto INI_MAPNAME_ARG = 0;
	constexpr auto INI_FOG_DIST = 1;
	constexpr auto INI_FOG_COLOR_BEGIN = 2;
	constexpr auto INI_SUN_DIR_BEGIN = 5;
	constexpr auto INI_SUN_COLOR_BEGIN = 8;
	constexpr auto INI_SUN_INTENSITY = 11;
	constexpr auto INI_SKY_INDEX = 12;
	constexpr auto INI_ARGS_TOTAL = 13;

	void RtxMapSettings::set_settings_for_loaded_map(bool reload_settings)
	{
		if ((m_settings.empty() || reload_settings) && !RtxMapSettings::load_settings())
		{
			return;
		}

		if (Game::rgp->world && Game::rgp->world->name)
		{
			std::string map_name = Game::rgp->world->name;
			RtxUtils::replace_all(map_name, std::string("maps/mp/"), "");	// if mp map
			RtxUtils::replace_all(map_name, std::string("maps/"), "");		// if sp map
			RtxUtils::replace_all(map_name, std::string(".d3dbsp"), "");

			bool found = false;
			for (const auto& s : m_settings)
			{
				if (s.mapname == map_name)
				{
					m_loaded_map_settings = s;

					// 0-255 to 0-1 range
					RtxUtils::scale3(m_loaded_map_settings.sun_color, (1.0f / 255.0f), m_loaded_map_settings.sun_color);

					Rtx::skysphere_spawn(s.sky_index);

					// spawn map markers
					for (auto i = 0u; i < s.map_markers.size(); i++)
					{
						if (s.map_markers[i].active)
						{
							if (const auto fx = Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_FX, Utils::String::VA("rtx/markers/rtx_marker_%02d", i)).fx; fx)
							{
								m_loaded_map_settings.map_markers[i].handle = Game::FX_SpawnOrientedEffect(Game::IDENTITY_AXIS[0], fx, 0, &s.map_markers[i].origin[0]);
								int x = 0;
							}
						}
					}

					found = true;
					break;
				}
			}

			if (!found)
			{
				// grab map defaults
				if (Game::gfx_world)
				{
					Game::vec3_t sun_fwd = {};
					RtxUtils::angle_vectors(Game::gfx_world->sunParse.angles, sun_fwd, nullptr, nullptr);
					RtxUtils::scale3(sun_fwd, -1.0f, sun_fwd);
					RtxUtils::copy(sun_fwd, m_loaded_map_settings.sun_direction, 3);

					RtxUtils::copy(Game::gfx_world->sunParse.sunColor, m_loaded_map_settings.sun_color, 3);
					m_loaded_map_settings.sun_intensity = Game::gfx_world->sunParse.sunLight;
				}

				if (!Flags::HasFlag("no_default_sky") && !Rtx::skysphere_is_model_valid())
				{
					Rtx::skysphere_spawn(0);
				}
			}
		}
	}

	RtxMapSettings::map_settings_s* RtxMapSettings::get_or_create_settings()
	{
		// check if map settings exist
		for (auto& e : m_settings)
		{
			if (e.mapname._Equal(m_args[INI_MAPNAME_ARG]))
			{
				return &e;
			}
		}

		// create defaults if not
		m_settings.push_back(map_settings_s(m_args[INI_MAPNAME_ARG]));
		return &m_settings.back();
	}

	void RtxMapSettings::parse_culling()
	{
		if (map_settings_s* s = get_or_create_settings(); s)
		{
			s->cell_settings.clear();
			s->cell_settings.resize(256);
			bool any_valid_overrides = false;

			// for each cell with its forced indices with format -> [cell](index index index)
			for (auto a = 1u; a < m_args.size(); a++)
			{
				const auto& str = m_args[a];

				if (str.empty())
				{
					// print msg here
					continue;
				}

				// which cell are we writing settings for?
				const auto cell_index = RtxUtils::try_stoi(RtxUtils::split_string_between_delims(str, '[', ']'), -1);
				if (cell_index >= 0)
				{
					// limit the vector to 1024 entries
					if (cell_index >= 1024)
					{
						continue;
					}

					// resize vector to up to 1024 entries
					if (static_cast<std::uint32_t>(cell_index) >= s->cell_settings.size())
					{
						s->cell_settings.resize(cell_index + 1);
					}

					// get cell
					const auto c = &s->cell_settings[cell_index];

					// ignore duplicate cells
					if (c->active)
					{
						continue;
					}

					// get inidices
					const auto indices_str = RtxUtils::split_string_between_delims(str, '(', ')');
					const auto split_indices = RtxUtils::split(indices_str, ' ');

					for (const auto& i : split_indices)
					{
						c->forced_cell_indices.push_back(RtxUtils::try_stoi(i, -1));
					}

					c->active = true;
					any_valid_overrides = true;
				}
			}

			s->cell_overrides_exist = any_valid_overrides;
		}
	}

	void RtxMapSettings::parse_markers()
	{
		if (map_settings_s* s = get_or_create_settings(); s)
		{
			// kill active fx in-case we reload settings 
			for (auto i = 0u; i < m_loaded_map_settings.map_markers.size(); i++)
			{
				if (m_loaded_map_settings.map_markers[i].active && m_loaded_map_settings.map_markers[i].handle)
				{
					Game::FX_KillEffect(m_loaded_map_settings.map_markers[i].handle);
				}
			}

			s->map_markers.clear();
			s->map_markers.resize(100);

			for (auto a = 1u; a < m_args.size(); a++)
			{
				const auto& str = m_args[a];

				if (str.empty())
				{
					// print msg here
					continue;
				}

				// which marker are we writing settings for?
				const auto marker_index = RtxUtils::try_stoi(RtxUtils::split_string_between_delims(str, '[', ']'), -1);
				if (marker_index >= 0)
				{
					// limit the vector to 100 entries
					if (marker_index >= 100)
					{
						continue;
					}

					// get marker
					const auto m = &s->map_markers[marker_index];

					// ignore duplicate markers
					if (m->active)
					{
						continue;
					}

					// get and assign origin
					const auto origin_str = RtxUtils::split_string_between_delims(str, '(', ')');
					if (const auto xyz = RtxUtils::split(origin_str, ' ');
						xyz.size() == 3u)
					{
						m->origin[0] = RtxUtils::try_stof(xyz[0], 0.0f);
						m->origin[1] = RtxUtils::try_stof(xyz[1], 0.0f);
						m->origin[2] = RtxUtils::try_stof(xyz[2], 0.0f);
						m->active = true;
					}
				}
			}
		}
	}

	void RtxMapSettings::parse_settings()
	{
		if (m_args.size() == INI_ARGS_TOTAL)
		{
			m_settings.push_back(
				{
					m_args[INI_MAPNAME_ARG],
					RtxUtils::try_stof(m_args[INI_FOG_DIST], 5000.0f),
					D3DCOLOR_XRGB
					(
						RtxUtils::try_stoi(m_args[INI_FOG_COLOR_BEGIN + 0], 255),
						RtxUtils::try_stoi(m_args[INI_FOG_COLOR_BEGIN + 1], 255),
						RtxUtils::try_stoi(m_args[INI_FOG_COLOR_BEGIN + 2], 255)
					),
					{
						RtxUtils::try_stof(m_args[INI_SUN_DIR_BEGIN + 0], 75.0f),
						RtxUtils::try_stof(m_args[INI_SUN_DIR_BEGIN + 1], -15.0f),
						RtxUtils::try_stof(m_args[INI_SUN_DIR_BEGIN + 2], -35.0f)
					},
					{
						RtxUtils::try_stof(m_args[INI_SUN_COLOR_BEGIN + 0], 255),
						RtxUtils::try_stof(m_args[INI_SUN_COLOR_BEGIN + 1], 255),
						RtxUtils::try_stof(m_args[INI_SUN_COLOR_BEGIN + 2], 255)
					},
					RtxUtils::try_stof(m_args[INI_SUN_INTENSITY], 1.0f),
					RtxUtils::try_stoi(m_args[INI_SKY_INDEX], 2)
				});
		}
	}

	bool RtxMapSettings::load_settings()
	{
		m_settings.clear();
		m_settings.reserve(32);

		std::ifstream file;
		if (RtxUtils::open_file_homepath("iw3sp_data", "map_settings.ini", false, file))
		{
			std::string input;
			auto parse_mode = PARSE_MODE::SETTINGS;

			// read line by line
			while (std::getline(file, input))
			{
				// ignore comment
				if (RtxUtils::starts_with(input, "//"))
				{
					continue;
				}

				if (parse_mode == SETTINGS && RtxUtils::starts_with(input, "#CULL"))
				{
					parse_mode = CULL;
					continue;
				}

				if (parse_mode == CULL && RtxUtils::starts_with(input, "#MARKER"))
				{
					parse_mode = MARKER;
					continue;
				}

				// split string on ','
				m_args = RtxUtils::split(input, ',');

				switch (parse_mode)
				{
				case SETTINGS:
					parse_settings();
					break;
				case CULL:
					parse_culling();
					break;
				case MARKER:
					parse_markers();
					break;
				}
			}

			file.close();
			return true;
		}

		return false;
	}

	RtxMapSettings::RtxMapSettings()
	{
		Command::Add("mapsettings_update", "", "reloads the map_settings.ini file in root/iw3xo/rtx", [this]
		{
			RtxMapSettings::set_settings_for_loaded_map(true);
		});

		Command::Add("mapsettings_get_defaults", "", "get sun direction, color and intensity in map_settings format based on the stock map settings", [this]
		{
			if (Game::gfx_world)
			{
				Game::vec3_t sun_fwd = {};
				RtxUtils::angle_vectors(Game::gfx_world->sunParse.angles, sun_fwd, nullptr, nullptr);
				RtxUtils::scale3(sun_fwd, -1.0f, sun_fwd);

				Game::vec3_t sun_color = {};
				RtxUtils::scale3(Game::gfx_world->sunParse.sunColor, 255.0f, sun_color);

				const auto str = Utils::String::VA("SunDir - SunColor - SunIntensity [%.3f,%.3f,%.3f,   %.1f,%.1f,%.1f,   %.2f]\n",
					sun_fwd[0], sun_fwd[1], sun_fwd[2], sun_color[0], sun_color[1], sun_color[2], Game::gfx_world->sunParse.sunLight);

				Game::Com_PrintMessage(0, str, 0);
				DEBUG_PRINT(str);
			}
		});
	}
}