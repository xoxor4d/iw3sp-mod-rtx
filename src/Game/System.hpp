#pragma once

namespace Game
{
	extern char* sys_processSemaphoreFile;
	//-------------------------------------------------
	//-------------------------------------------------
	extern int Sys_MilliSeconds();
	extern int Sys_MillisecondsRaw();
	extern int Sys_IsDatabaseReady2();
	void Sys_CreateConsole/*ax*/(HINSTANCE hInstance /*edi*/);
	void Sys_ShowConsole();
}