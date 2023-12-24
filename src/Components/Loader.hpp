#pragma once

namespace Components
{
	class Component
	{
	public:
		Component() = default;
		virtual ~Component() = default;

		virtual void preDestroy() {}
		virtual bool unitTest() { return true; } // Unit testing entry
	};

	class Loader
	{
	public:
		static void Initialize();
		static void Uninitialize();
		static void PreDestroy();
		static void PreDestroyNoPostGame();
		static void Register(Component* component);

		static bool IsPregame();
		static bool IsPostgame();
		static bool IsUninitializing();
		static bool BinaryCheck();

		template <typename T>
		static T* GetInstance()
		{
			for (auto& component : Loader::Components)
			{
				if (typeid(*component) == typeid(T))
				{
					return reinterpret_cast<T*>(component);
				}
			}

			return nullptr;
		}

	private:
		static bool Pregame;
		static bool Postgame;
		static bool Uninitializing;
		static std::vector<Component*> Components;
	};
}

#include "Modules/Singleton.hpp"
#include "Modules/Config.hpp"
#include "Modules/Flags.hpp"
#include "Modules/Branding.hpp"
#include "Modules/FastFiles.hpp"
#include "Modules/Scheduler.hpp"
#include "Modules/Command.hpp"
#include "Modules/AssetHandler.hpp"
#include "Modules/FileSystem.hpp"
#include "Modules/CommonPatch.hpp"
#include "Modules/Language.hpp"
#include "Modules/Maps.hpp"
#include "Modules/RawMouse.hpp"
#include "Modules/Renderer.hpp"
#include "Modules/D3D9Ex.hpp"
#include "Modules/GSC/GSC.hpp"
#include "Modules/GSC/ScriptExtension.hpp"
#include "Modules/Movement.hpp"
#include "Modules/Weapons.hpp"
#include "Modules/Discord.hpp"
#include "Modules/TextRenderer.hpp"
#include "Modules/UIScript.hpp"
#include "Modules/ServerCommand.hpp"
#include "Modules/LocalizedStrings.hpp"
#include "Modules/Toast.hpp"
#include "Modules/Achievements.hpp"
#include "Modules/Events.hpp"
#include "Modules/Exception.hpp"
#include "Modules/Updater.hpp"
#include "Modules/Window.hpp"
#include "Modules/Sound.hpp"
#include "Modules/Gamepad.hpp"
#include "Modules/GUI/GUI.hpp"
#include "Modules/GUI/Markdown.hpp"
#include "Modules/Changelog.hpp"
#include "Modules/Rtx.hpp"
