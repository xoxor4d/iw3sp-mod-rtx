#include "STDInc.hpp"

namespace Components
{
	RtxMapSettings* RtxMapSettings::p_this = nullptr;
	RtxMapSettings* RtxMapSettings::get() { return p_this; }

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
					m_max_distance = s.max_distance;
					m_color = s.m_color;

					Rtx::skysphere_spawn(s.skybox);

					found = true;
					break;
				}
			}

			if (!found)
			{
				m_max_distance = 5000.0f;
				m_color.packed = D3DCOLOR_XRGB(200, 200, 220);

				if (!Flags::HasFlag("no_default_sky") && !Rtx::skysphere_is_model_valid())
				{
					Rtx::skysphere_spawn(5); // always spawn sunset
				}
			}
		}
	}

	constexpr auto INI_MAPNAME_ARG = 0;
	constexpr auto INI_SKYBOX_ARG = 1;
	constexpr auto INI_FOG_MAX_ARG = 2;
	constexpr auto INI_FOG_COLOR_ARG_BEGIN = 3;

	bool RtxMapSettings::load_settings()
	{
		m_settings.clear();
		m_settings.reserve(32);

		std::ifstream file;
		if (RtxUtils::open_file_homepath("iw3sp_data", "map_settings.ini", false, file))
		{
			std::string input;
			std::vector<std::string> args;

			// read line by line
			while (std::getline(file, input))
			{
				// ignore comment
				if (RtxUtils::starts_with(input, "//"))
				{
					continue;
				}

				// split string on ','
				args = RtxUtils::split(input, ',');

				if (args.size() == INI_FOG_COLOR_ARG_BEGIN+3) // fog colors rgb are last
				{
					const DWORD color = D3DCOLOR_XRGB(
						RtxUtils::try_stoi(args[INI_FOG_COLOR_ARG_BEGIN+0], 255),
						RtxUtils::try_stoi(args[INI_FOG_COLOR_ARG_BEGIN+1], 255),
						RtxUtils::try_stoi(args[INI_FOG_COLOR_ARG_BEGIN+2], 255));

					m_settings.push_back(
						{
							args[INI_MAPNAME_ARG],
							RtxUtils::try_stoi(args[INI_SKYBOX_ARG], 0),
							RtxUtils::try_stof(args[INI_FOG_MAX_ARG], 5000.0f),
							color
						});
				}
			}

			file.close();
			return true;
		}

		return false;
	}

	RtxMapSettings::RtxMapSettings()
	{
		Command::Add("mapsettings_update", [this]
		{
			RtxMapSettings::set_settings_for_loaded_map(true);
		});
	}
}