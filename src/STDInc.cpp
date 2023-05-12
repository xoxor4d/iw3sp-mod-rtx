#include "STDInc.hpp"

// Disable telemetry data logging
extern "C"
{
	void _cdecl __vcrt_initialize_telemetry_provider() {}
	void _cdecl __telemetry_main_invoke_trigger() {}
	void _cdecl __telemetry_main_return_trigger() {}
	void _cdecl __vcrt_uninitialize_telemetry_provider() {}

	// Enable 'High Performance Graphics'
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
};

// Bridge d3d9
#pragma comment(linker, "/export:Direct3DCreate9=d3d9.Direct3DCreate9")
#pragma comment(linker, "/export:Direct3DCreate9Ex=d3d9.Direct3DCreate9Ex")
#pragma comment(linker, "/export:Direct3DCreate9On12=d3d9.Direct3DCreate9On12")
#pragma comment(linker, "/export:Direct3DCreate9On12Ex=d3d9.Direct3DCreate9On12Ex")
#pragma comment(linker, "/export:PSGPError=d3d9.PSGPError")
#pragma comment(linker, "/export:PSGPSampleTexture=d3d9.PSGPSampleTexture")
#pragma comment(linker, "/export:D3DPERF_BeginEvent=d3d9.D3DPERF_BeginEvent")
#pragma comment(linker, "/export:D3DPERF_EndEvent=d3d9.D3DPERF_EndEvent")
#pragma comment(linker, "/export:D3DPERF_GetStatus=d3d9.D3DPERF_GetStatus")
#pragma comment(linker, "/export:D3DPERF_QueryRepeatFrame=d3d9.D3DPERF_QueryRepeatFrame")
#pragma comment(linker, "/export:D3DPERF_SetMarker=d3d9.D3DPERF_SetMarker")
#pragma comment(linker, "/export:D3DPERF_SetOptions=d3d9.D3DPERF_SetOptions")
#pragma comment(linker, "/export:D3DPERF_SetRegion=d3d9.D3DPERF_SetRegion")
#pragma comment(linker, "/export:Direct3D9EnableMaximizedWindowedModeShim=d3d9.Direct3D9EnableMaximizedWindowedModeShim")
#pragma comment(linker, "/export:DebugSetLevel=d3d9.DebugSetLevel")
#pragma comment(linker, "/export:DebugSetMute=d3d9.DebugSetMute")
#pragma comment(linker, "/export:Direct3DShaderValidatorCreate9=d3d9.Direct3DShaderValidatorCreate9")