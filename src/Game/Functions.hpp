#pragma once
#include "../Utils/Function.hpp"

namespace Game
{
	extern HWND__* hWnd;
	extern HWND__* hWndParent;
	extern HWND__* hwndBuffer;

	extern IDirect3DDevice9** dx9_device_ptr;

	typedef void(__cdecl* CommandCB_t)(void);

	typedef void(*Com_PrintMessage_t)(int, const char*, char);
	extern Com_PrintMessage_t Com_PrintMessage;

	void Cmd_AddCommand(const char* name, void(*callback)(), cmd_function_s* data, char);
	void Cmd_AddCommand(const char* name, const char* args, const char* description, void(*callback)(), cmd_function_s* data, char);

	typedef void(*Cmd_ExecuteSingleCommand_t)(int controller, int a2, const char* cmd);
	extern Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand;

	typedef void(__cdecl* Cmd_AddServerCommand_t)(const char* name, CommandCB_t callback, cmd_function_s* data);
	extern Cmd_AddServerCommand_t Cmd_AddServerCommand;

	typedef void(__cdecl* Com_Printf_t)(int, const char*, ...);
	extern Com_Printf_t Com_Printf;

	typedef void(__cdecl* Com_Error_t)(int code, const char* fmt, ...);
	extern Com_Error_t Com_Error;
	
	typedef void(__cdecl* Scr_ParamError_t)(const char* error, ...);
	extern Scr_ParamError_t Scr_ParamError;

	//typedef void(__cdecl* Cmd_SetAutoComplete_t)(const char* name, const char* path, const char* extension);
	//extern Cmd_SetAutoComplete_t Cmd_SetAutoComplete;

	typedef void(__cdecl* FS_FreeFile_t)(void* buffer);
	extern FS_FreeFile_t FS_FreeFile;

	typedef int(__cdecl* FS_ReadFile_t)(const char* path, char** buffer);
	extern FS_ReadFile_t FS_ReadFile;

	typedef void (*DB_LoadXAssets_t)(XZoneInfo* data, unsigned int count, int sync);
	extern DB_LoadXAssets_t DB_LoadXAssets;

	typedef void(*DB_EnumXAssets_t)(XAssetType type, void(*)(XAssetHeader, void*), const void* userdata, bool overrides);
	extern DB_EnumXAssets_t DB_EnumXAssets;

	static XZoneName* g_zoneNames = (XZoneName*)0xAC3000;

	static Utils::function<void()> R_BeginRemoteScreenUpdate = 0x5DC550;
	static Utils::function<void()> R_EndRemoteScreenUpdate = 0x5DC5A0;

	//typedef void(__cdecl* Cbuf_AddText_t)(int localClientNum, const char* text);
	//extern Cbuf_AddText_t Cbuf_AddText;

	void Cbuf_AddText(int localClientNum /*eax*/, const char* text /*ecx*/);

	//-------------------------------------------------

	extern HANDLE DatabaseHandle;

	//XAssets
	extern XAssetHeader* DB_XAssetPool;
	extern unsigned int* g_poolSize;
	extern const char** g_assetNames;

	typedef int(__cdecl* DB_GetXAssetSizeHandler_t)();
	extern DB_GetXAssetSizeHandler_t* DB_GetXAssetSizeHandlers;

	typedef const char* (*DB_GetXAssetNameHandler_t)(Game::XAssetHeader* asset);
	extern DB_GetXAssetNameHandler_t* DB_GetXAssetNameHandlers;

	XAssetHeader DB_ReallocXAssetPool(XAssetType type, unsigned int new_size);

	constexpr auto CMD_MAX_NESTING = 8;
	extern Game::CmdArgs* cmd_args;
	extern cmd_function_s** cmd_ptr;
	extern cmd_function_s* cmd_functions;

	extern int Sys_MilliSeconds();

	typedef void(*DB_BeginRecoverLostDevice_t)();
	extern DB_BeginRecoverLostDevice_t DB_BeginRecoverLostDevice;

	typedef void(*DB_EndRecoverLostDevice_t)();
	extern DB_EndRecoverLostDevice_t DB_EndRecoverLostDevice;

	typedef void(*UpdateViewWeaponAnim_t)();
	extern UpdateViewWeaponAnim_t UpdateViewWeaponAnim;

	typedef int(__cdecl* SND_GetEntChannelFromName_t)(const char* channelName);
	extern SND_GetEntChannelFromName_t SND_GetEntChannelFromName;

	static Utils::function<void()> DB_SyncXAssets = 0x45B6F0;

	typedef XAssetHeader(*DB_FindXAssetHeader_t)(XAssetType type, const char* name);
	extern DB_FindXAssetHeader_t DB_FindXAssetHeader;

	typedef bool(__cdecl* DB_IsXAssetDefault_t)(XAssetType assettype, const char* name);
	extern DB_IsXAssetDefault_t DB_IsXAssetDefault;

	extern snd_alias_list_t* Com_FindSoundAlias(const char* name);

	static Utils::function<snd_alias_list_t* (const char* soundname)>
		Com_PickSoundAliasFromList_t = 0x581500;

	inline snd_alias_list_t* Com_PickSoundAliasFromList(const char* soundname) {
		return Com_PickSoundAliasFromList_t(soundname);
	}

	extern Game::GfxBackEndData* _frontEndDataOut;
	extern Game::GfxBackEndData* _backEndData;

	extern int* scr_numParam;


	typedef Game::xmethod_t(Player_GetMethod_t)(const char**);
	extern Player_GetMethod_t* Player_GetMethod;
	typedef Game::xfunction_t(Scr_GetFunction_t)(const char**, int*);
	extern Scr_GetFunction_t* Scr_GetFunction;

	extern char* isDvarSystemActive;
	extern bool CL_IsCgameInitialized();

	int __cdecl Scr_GetNumParam(void);
	void Scr_Error(const char* error);

	//From IW3xo
	extern DWORD* scrVarPub;
	extern int* scrVarPub_p4;
	extern char* error_message;
	extern char* errortype;
	static Utils::function<void()> Scr_Error_Internal = 0x5520E0;

	extern Game::Font_s* R_RegisterFont(char const* fontName, int imageTrack);

	extern unsigned int Scr_GetConstString(unsigned int index /*eax*/);
	extern const char* Scr_GetString(unsigned int index);
	float Scr_GetFloat/*st0*/(unsigned int index /*eax*/); // asm
	int Scr_GetInt/*eax*/(unsigned int index /*eax*/); // asm 
	extern const char* SL_ConvertToString(unsigned int idx);

	typedef int(__cdecl* Cmd_RemoveCommand_t)(const char* name);
	extern Cmd_RemoveCommand_t Cmd_RemoveCommand;

	//15.10.22
	typedef unsigned int(__cdecl* Scr_AllocString_t)(const char* s, int sys, size_t size);
	extern Scr_AllocString_t Scr_AllocString;
	extern scr_const_t* scr_const;
	extern unsigned int GScr_AllocString(const char* s);

	//16.10.22
	extern Game::playerState_s* ps;
	extern Game::gentity_s* g_entities;
	extern Game::gclient_s* g_clients;
	extern Game::pmove_t* pmove;
	extern Game::itemDef_s* my_item;
	//extern Game::snd_local_t* g_snd;
	//06.01.23
	extern Game::clipMap_t* cm;
	//01.02.23
	//extern Game::GfxWorld* gfx_world;

	//22.10.22
	typedef int(__cdecl* StringTable_LookupRowNumForValue_t)(const StringTable* table, int comparisonColumn, const char* value);
	extern StringTable_LookupRowNumForValue_t StringTable_LookupRowNumForValue;

	extern const char* StringTable_Lookup(const StringTable* table /*<ebp>*/, const int comparisonColumn /*<esi>*/, const char* value/*<eax>*/, const int valueColumn /*<edi>*/);
	extern void StringTable_GetAsset(const char* filename, const StringTable** tablePtr);

	//26.10.22
	typedef void(*Scr_AddInt_t)(int value);
	extern Scr_AddInt_t Scr_AddInt;

	typedef void(*Scr_AddFloat_t)(float value);
	extern Scr_AddFloat_t Scr_AddFloat;

	void Scr_AddString(const char* value /*esi*/);

	extern bool Key_IsCatcherActive(int localClientNum, int mask);
	//extern Game::PlayerKeyState* playerKeys;

	extern const char* SEH_GetCurrentLanguage();

	//29.10.22
	typedef Game::gentity_s* (*GetPlayerEntity_t)(Game::scr_entref_t entref);
	extern GetPlayerEntity_t GetPlayerEntity;

	//	#define uiMem (*((uiMem_t*)(0x12909C8)))
	//30.10.22
	extern Game::ScreenPlacement* scrPlace;
	extern Game::ScreenPlacement* scrPlaceFull;
	extern Game::ScreenPlacement* scrPlaceFullUnsafe;

	Game::ScreenPlacement* ScrPlace_GetFullPlacement();
	Game::ScreenPlacement* ScrPlace_GetUnsafeFullPlacement();
	void R_AddCmdDrawTextASM(const char* text, int max_chars, void* font, float x, float y, float x_scale, float y_scale, float rotation, const float* color, int style);
	void R_AddCmdDrawStretchPic(Game::Material* material, float x, float y, float w, float h, float null1, float null2, float null3, float null4, float* color); // ASM
	const char* UI_GetFontHandle(const ScreenPlacement* ScrPlace, float fontscale);
	extern sharedUiInfo_t* sharedUiInfo;
	extern Game::uiInfo_s* uiInfo;
	int R_TextHeight(Font_s* font);
	long double R_NormalizedTextScale(Font_s* font, float scale);

	int UI_TextWidth(const char* text, int maxChars, Font_s* font, float scale);
	int UI_TextHeight(Font_s* font, float scale);
	int R_TextWidth/*eax*/(const char* text /*eax*/, signed int maxChars, Game::Font_s* font/*ecx*/);


	void ScrPlace_ApplyRect(const ScreenPlacement* ScrPlace, float* x, float* y, float* w, float* h, int horzAlign, int vertAlign);
	void UI_DrawText(const ScreenPlacement* ScrPlace, const char* text, int maxChars, Font_s* font, float ix, float iy, int horzAlign, int vertAlign, float scale, const float* color, int style);
	
	//01.11.22
	extern Game::WeaponDef_s** BG_WeaponNames;

	extern Game::cg_s* cgs;
	extern int* g_entities_int;

	extern Game::weaponInfo_s* cg_viewModelArray;

	extern int* cg_weaponsArray;
	extern DWORD* dword_FDBDCC;
	extern __int16* word_13E45E8;
	extern __int16* word_13E45DC;

	extern int* g_animRateOffsets;

	void SV_SendServerCommand(int clientNum, const char* fmt);
	void SV_GameSendServerCommand(int clientNum, const char* fmt);

	//21.12.22
	static Utils::function<void(Game::pmove_t* pm, Game::pml_t* pml)> PM_AirMove = 0x5BF480;
	static Utils::function<void(Game::pmove_t* pm)> PM_UpdateSprint = 0x5B72F0;


	extern char* byte_14C80E0;

	typedef void(__cdecl* CG_GameMessage_t)(const char* message, int a2);
	extern CG_GameMessage_t CG_GameMessage;

	//24.03.2023
	extern int* dword_129ADC4;
	extern int* dword_129AFC8;

	extern const char* g_languages;
	const char* SEH_GetLanguageName(unsigned int iLanguage);
	const char* UI_SafeTranslateString/*eax*/(const char* reference /*eax*/);

	typedef const char*(__cdecl* UI_ReplaceConversionString_t)(const char* reference, const char* value);
	extern UI_ReplaceConversionString_t UI_ReplaceConversionString;

	//	New g_languages array.
	static const char* g_languages_new[] = { "english", "french", "german", "italian",
				  "spanish", "british", "russian", "polish",
				  "korean", "taiwanese", "japanese", "chinese",
				  "thai", "leet", "czech" };

	int SEH_GetLanguageIndexFromName(const char* language, int* langindex);

	int I_stricmp/*eax*/(int a1/*eax*/, const char* a2 /*edx*/, const char* a3);


	char* Com_Parse(const char** data_p);

	typedef int(__cdecl* BG_FindWeaponIndexForName_t)(const char* weaponName);
	extern BG_FindWeaponIndexForName_t BG_FindWeaponIndexForName;

	extern int Sys_IsDatabaseReady2(void);

	extern Game::localization_t* localization;

	void Sys_CreateConsole/*ax*/(HINSTANCE hInstance /*edi*/);
	void Sys_ShowConsole();

	void FS_DisplayPath(int bLanguageCull);

	extern Game::searchpath_s** fs_searchpaths;

	typedef void(__cdecl* FS_AddIwdFilesForGameDirectory_t)(const char* path, char* pszGameFolder);
	extern FS_AddIwdFilesForGameDirectory_t FS_AddIwdFilesForGameDirectory;

	void FS_AddLocalizedGameDirectory(const char* dir/*edi*/, const char* path);
	int FS_FOpenFileWriteToDir(const char* a1/*eax*/, const char* a2/*esi*/, int a3);
	void Com_ExecStartupConfigs(int localClientNum, char const* configFile);

	void PM_Weapon_FireWeapon(Game::playerState_s* ps, int delayedAction);
}