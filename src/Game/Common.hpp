#pragma once

namespace Game
{
	//-------------------------------------------------
	typedef void(__cdecl* Com_Printf_t)(int, const char*, ...);
	extern Com_Printf_t Com_Printf;

	typedef void(__cdecl* Com_Error_t)(int code, const char* fmt, ...);
	extern Com_Error_t Com_Error;
	//-------------------------------------------------
	char* Com_Parse(const char** data_p);
	void Com_ExecStartupConfigs(int localClientNum, char const* configFile);
	int Com_sprintfPos(char* dest, int destSize, int* destPos, const char* fmt, ...);
}