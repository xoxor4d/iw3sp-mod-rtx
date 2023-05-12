#include "STDInc.hpp"

namespace Components
{
	std::unordered_map<std::string, std::function<void()>> ServerCommand::ServerCommands;

	void ServerCommand::Add(const char* name, const std::function<void()>& callback)
	{
		ServerCommands[name] = callback;
	}

	void ServerCommand::DispatchServerCommand(const char* cmd_args)
	{
		//Game::Com_Printf(0, "^3CmdArg: %s\n", cmd_args);
		if (const auto itr = ServerCommands.begin(); itr != ServerCommands.end())
		{
			if (!std::strcmp(cmd_args, itr->first.c_str()))
			{
				itr->second();
				return;
			}
		}
	}

	void __declspec(naked) CG_DispatchServerCommand_stub()
	{
		const static uint32_t next_cmd_offset = 0x429963;
		const static uint32_t sw_func_offset = 0x429942;

		//	push	edi; text
		//	push	esi; cmd_args
		//	push    ecx; length (wtf?)
		__asm
		{
			push	esi; //cmd_args
			pushad;
			push	esi;
			call	ServerCommand::DispatchServerCommand;
			add		esp, 4;
			jnz		SW_STOCK;
			popad;
			pop		esi;
		SW_STOCK:
			popad;
			pop		esi;
			repe	cmpsb;
			jnz		NEXT_SERVER_CMD;
			cmp		ebp, 1;
			jmp		sw_func_offset;
		NEXT_SERVER_CMD:
			jmp		next_cmd_offset;
		}
	}

	ServerCommand::ServerCommand()
	{
		Utils::Hook::Nop(0x42993B, 7);
		// Sadly, but I doesn't find resolve about printing error in the game console, 
		// when cmd_args similar with strings and custom server cmds works fine.
		Utils::Hook::Nop(0x42A1BF, 5);
		Utils::Hook(0x42993B, CG_DispatchServerCommand_stub, HOOK_JUMP).install()->quick();
	}

	ServerCommand::~ServerCommand()
	{
		ServerCommands.clear();
	}
}