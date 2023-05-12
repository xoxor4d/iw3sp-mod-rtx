#include "STDInc.hpp"

namespace Components
{
	bool Loader::Pregame = true;
	bool Loader::Postgame = false;
	bool Loader::Uninitializing = false;
	std::vector<Component*> Loader::Components;

	bool Loader::IsPregame()
	{
		return Loader::Pregame;
	}

	bool Loader::IsPostgame()
	{
		return Loader::Postgame;
	}

	bool Loader::IsUninitializing()
	{
		return Loader::Uninitializing;
	}

	void Loader::Initialize()
	{
		Loader::Pregame = true;
		Loader::Postgame = false;
		Loader::Uninitializing = false;

		Loader::Register(new Flags());
		Loader::Register(new Command());
		Loader::Register(new Branding());
		Loader::Register(new CommonPatch());
		Loader::Register(new Maps());
		Loader::Register(new D3D9Ex());
		Loader::Register(new Renderer());
		Loader::Register(new Scheduler());
		Loader::Register(new FastFiles());
		Loader::Register(new GSC());
		Loader::Register(new Movement());
		Loader::Register(new Weapons());
		Loader::Register(new Discord());
		Loader::Register(new UIScript());


		Loader::Pregame = false;
		// Make sure preDestroy is called when the game shuts down
		//Scheduler::OnGameShutdown(Loader::PreDestroy);
	}

	void Loader::Uninitialize()
	{
		Loader::Uninitializing = true;
		Loader::PreDestroyNoPostGame();

		std::reverse(Loader::Components.begin(), Loader::Components.end());
		for (auto component : Loader::Components)
		{
			delete component;
		}

		Loader::Components.clear();
		//Utils::Memory::GetAllocator()->clear();
		Loader::Uninitializing = false;
	}

	void Loader::PreDestroy()
	{
		if (!Loader::Postgame)
		{
			Loader::Postgame = true;

			auto components = Loader::Components;

			std::reverse(components.begin(), components.end());
			for (auto component : components)
			{
				component->preDestroy();
			}
		}
	}

	void Loader::PreDestroyNoPostGame()
	{
		if (!Loader::Postgame)
		{
			auto components = Loader::Components;

			std::reverse(components.begin(), components.end());
			for (auto component : components)
			{
				component->preDestroy();
			}

			Loader::Postgame = true;
		}
	}

	void Loader::Register(Component* component)
	{
		if (component)
		{
			Loader::Components.push_back(component);
		}
	}
}