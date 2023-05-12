#include "STDInc.hpp"

namespace Components
{
	std::unordered_map<std::string, std::function<void(Command::Params*)>> Command::FunctionMap;

	std::string Command::Params::join(const int index)
	{
		std::string result;

		for (auto i = index; i < this->size(); i++)
		{
			if (i > index) result.append(" ");
			result.append(this->get(i));
		}

		return result;
	}

	Command::ClientParams::ClientParams() : nesting_(Game::cmd_args->nesting)
	{
		assert(Game::cmd_args->nesting < Game::CMD_MAX_NESTING);
	}

	int Command::ClientParams::size()
	{
		return Game::cmd_args->argc[this->nesting_];
	}

	const char* Command::ClientParams::get(const int index)
	{
		if (index >= this->size())
		{
			return "";
		}

		return Game::cmd_args->argv[this->nesting_][index];
	}

	void Command::Add(const char* name, const std::function<void()>& callback)
	{
		Add(name, [callback]([[maybe_unused]] const Command::Params* params)
		{
			callback();
		});
	}

	void Command::Add(const char* name, const char* args, const char* description, const std::function<void()>& callback)
	{
		Add(name, args, description, [callback]([[maybe_unused]] const Command::Params* params)
		{
			callback();
		});
	}

	void Command::Add(const char* name, const std::function<void(Command::Params*)>& callback)
	{
		const auto command = Utils::String::ToLower(name);

		if (Command::FunctionMap.find(command) == Command::FunctionMap.end())
		{
			Command::AddRaw(name, Command::MainCallback);
		}

		Command::FunctionMap.insert_or_assign(command, std::move(callback));
	}

	void Command::Add(const char* name, const char* args, const char* description, const std::function<void(Command::Params*)>& callback)
	{
		const auto command = Utils::String::ToLower(name);

		if (Command::FunctionMap.find(command) == Command::FunctionMap.end())
		{
			Command::AddRaw(name, args, description, Command::MainCallback);
		}

		Command::FunctionMap.insert_or_assign(command, std::move(callback));
	}

	void Command::AddRaw(const char* name, void(*callback)(), bool key)
	{
		Game::Cmd_AddCommand(name, callback, Command::Allocate(), key);
	}

	void Command::AddRaw(const char* name, const char* args, const char* description, void(*callback)(), bool key)
	{
		Game::Cmd_AddCommand(name, args, description, callback, Command::Allocate(), key);
	}

	void Command::Execute(std::string command, bool sync)
	{
		command.append("\n"); // Make sure it's terminated

		if (sync)
		{
			Game::Cmd_ExecuteSingleCommand(0, 0, command.data());
		}
		else
		{
			Game::Cbuf_AddText(0, command.data());
		}
	}

	Game::cmd_function_s* Command::Find(const std::string& command)
	{
		auto* cmdFunction = Game::cmd_functions;

		while (cmdFunction != nullptr)
		{
			if (cmdFunction->name != nullptr && cmdFunction->name == command)
			{
				return cmdFunction;
			}

			cmdFunction = cmdFunction->next;
		}

		return nullptr;
	}

	Game::cmd_function_s* Command::Allocate()
	{
		return Utils::Memory::GetAllocator()->allocate<Game::cmd_function_s>();
	}

	void Command::MainCallback()
	{
		ClientParams params;
		const auto command = Utils::String::ToLower(params[0]);

		const auto got = FunctionMap.find(command);

		if (got != FunctionMap.end())
		{
			got->second(&params);
		}
	}
}