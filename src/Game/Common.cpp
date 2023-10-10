#include "STDInc.hpp"

namespace Game
{
	//-------------------------------------------------
	Com_Printf_t Com_Printf = Com_Printf_t(0x532DB0);
	Com_Error_t Com_Error = Com_Error_t(0x533580);
	//-------------------------------------------------
	char* Com_Parse(const char** data_p)
	{
		const static uint32_t Com_Parse_func = 0x58F8B0;
		static char* result{};
		__asm
		{
			mov		edi, data_p;
			call	Com_Parse_func;
			mov		result, eax;
		}
		return result;
	}

	//int a1@<ecx>, const char *a2@<edi>, int a3@<esi>
	void Com_ExecStartupConfigs(int localClientNum, char const* configFile)
	{
		const static uint32_t Com_ExecStartupConfigs_func = 0x534A30;
		__asm
		{
			mov		esi, localClientNum;
			mov		edi, configFile;
			call	Com_ExecStartupConfigs_func;
		}
	}

	int Com_sprintfPos(char* dest/*ecx*/, int destSize /*eax*/, int* destPos, const char* fmt, ...)
	{
		int result;
		const static uint32_t Com_sprintfPos_func = 0x590270;
		__asm
		{
			pushad;
			mov		eax, destSize;
			mov		ecx, dest;
			push	fmt;
			push	destPos;
			call	Com_sprintfPos_func;
			add		esp, 8;
			mov		result, eax;
			popad;
		}
		return result;
	}
}