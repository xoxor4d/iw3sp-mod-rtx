#pragma once
#include "../Utils/Function.hpp"

namespace Game
{
	// < rtx begin
	extern GfxCmdBufSourceState* gfxCmdBufSourceState;

	static DWORD* backEndDataOut_ptr = (DWORD*)(0x18CB534);  // backendEndDataOut pointer
	extern Game::GfxBackEndData* get_backenddata();

	std::int16_t G_ModelIndex(const char* model_name /*eax*/); // ASM
	static Utils::function<Game::gentity_s* ()> G_Spawn = 0x4EFCF0;
	static Utils::function<bool(Game::gentity_s*)> G_CallSpawnEntity = 0x4E98F0;

	extern void G_DObjUpdate(entityState_s* ent /*eax*/); // ASM 4ED860

	// rtx end >


	const static HWND__* hWnd = reinterpret_cast<HWND__*>(0x13E39A8); // Splash screen (cod.bmp)
	const static HWND* hWndParent = reinterpret_cast<HWND*>(0x13E39B0); // External console
	const static HWND* hWndBuffer = reinterpret_cast<HWND*>(0x13E39B4); // External console buffer

	extern IDirect3DDevice9** dx9_device_ptr;

	typedef void(*longjmp_internal_t)(jmp_buf env, int status);
	extern longjmp_internal_t longjmp_internal;

	typedef void(__cdecl* CommandCB_t)(void);

	void Cmd_AddCommand(const char* name, void(*callback)(), cmd_function_s* data, char);
	void Cmd_AddCommand(const char* name, const char* args, const char* description, void(*callback)(), cmd_function_s* data, char);

	typedef void(*Cmd_ExecuteSingleCommand_t)(int controller, int a2, const char* cmd);
	extern Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand;

	typedef void(__cdecl* Cmd_AddServerCommand_t)(const char* name, CommandCB_t callback, cmd_function_s* data);
	extern Cmd_AddServerCommand_t Cmd_AddServerCommand;
	
	typedef void(__cdecl* Scr_ParamError_t)(const char* error, ...);
	extern Scr_ParamError_t Scr_ParamError;

	//typedef void(__cdecl* Cmd_SetAutoComplete_t)(const char* name, const char* path, const char* extension);
	//extern Cmd_SetAutoComplete_t Cmd_SetAutoComplete;

	typedef void(__cdecl* FS_FreeFile_t)(void* buffer);
	extern FS_FreeFile_t FS_FreeFile;

	typedef int(__cdecl* FS_ReadFile_t)(const char* path, char** buffer);
	extern FS_ReadFile_t FS_ReadFile;

	static Utils::function<void()> R_BeginRemoteScreenUpdate = 0x5DC550;
	static Utils::function<void()> R_EndRemoteScreenUpdate = 0x5DC5A0;

	//typedef void(__cdecl* Cbuf_AddText_t)(int localClientNum, const char* text);
	//extern Cbuf_AddText_t Cbuf_AddText;

	void Cbuf_AddText(int localClientNum /*eax*/, const char* text /*ecx*/);

	//-------------------------------------------------

	constexpr auto CMD_MAX_NESTING = 8;
	extern Game::CmdArgs* cmd_args;
	extern cmd_function_s** cmd_ptr;
	extern cmd_function_s* cmd_functions;

	typedef void(*UpdateViewWeaponAnim_t)();
	extern UpdateViewWeaponAnim_t UpdateViewWeaponAnim;

	typedef int(__cdecl* SND_GetEntChannelFromName_t)(const char* channelName);
	extern SND_GetEntChannelFromName_t SND_GetEntChannelFromName;

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

	// < rtx begin

	// get handle using DB_FindXAssetHeader
	static Utils::function<Material* (const char* fontName, int fontSize)>
		Material_RegisterHandle = 0x5D5DF0;

	// rtx end >

	extern unsigned int Scr_GetConstString(unsigned int index /*eax*/);
	extern const char* Scr_GetString(unsigned int index);
	float Scr_GetFloat/*st0*/(unsigned int index /*eax*/); // asm
	int Scr_GetInt/*eax*/(unsigned int index /*eax*/); // asm 
	extern const char* SL_ConvertToString(unsigned int idx);

	typedef int(__cdecl* Cmd_RemoveCommand_t)(const char* name);
	extern Cmd_RemoveCommand_t Cmd_RemoveCommand;

	typedef unsigned int(__cdecl* Scr_AllocString_t)(const char* s, int sys, size_t size);
	extern Scr_AllocString_t Scr_AllocString;
	extern scr_const_t* scr_const;
	extern unsigned int GScr_AllocString(const char* s);

	extern Game::playerState_s* ps;
	extern Game::gentity_s* g_entities;
	extern Game::gclient_s* g_clients;
	extern Game::PlayerKeyState* playerKeys;
	extern Game::clientUIActive_t* clientUIActives;
	extern Game::clientActive_t* clients;
	extern Game::pmove_t* pmove;
	extern Game::AimAssistGlobals* aaGlobArray;
	extern Game::itemDef_s* my_item;

	extern Game::WinMouseVars_t* s_wmv;
	extern Game::snd_local_t* g_snd;
	extern Game::clipMap_t* cm;
	//extern Game::GfxWorld* gfx_world;

	typedef int(__cdecl* StringTable_LookupRowNumForValue_t)(const StringTable* table, int comparisonColumn, const char* value);
	extern StringTable_LookupRowNumForValue_t StringTable_LookupRowNumForValue;

	extern const char* StringTable_Lookup(const StringTable* table /*<ebp>*/, const int comparisonColumn /*<esi>*/, const char* value/*<eax>*/, const int valueColumn /*<edi>*/);
	extern void StringTable_GetAsset(const char* filename, const StringTable** tablePtr);

	typedef void(*Scr_AddInt_t)(int value);
	extern Scr_AddInt_t Scr_AddInt;

	typedef void(*Scr_AddFloat_t)(float value);
	extern Scr_AddFloat_t Scr_AddFloat;

	void Scr_AddString(const char* value /*esi*/);

	extern bool Key_IsCatcherActive(int mask);
	//extern Game::PlayerKeyState* playerKeys;

	extern const char* SEH_GetCurrentLanguage();

	typedef Game::gentity_s* (*GetPlayerEntity_t)(Game::scr_entref_t entref);
	extern GetPlayerEntity_t GetPlayerEntity;

	//	#define uiMem (*((uiMem_t*)(0x12909C8)))
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
	
	extern Game::WeaponDef_s** BG_WeaponNames;
	extern Game::weaponInfo_s* cg_weaponsArray;
	extern Game::scrVmPub_t* scrVmPub;
	extern Game::GfxDrawMethod* gfxDrawMethod;

	extern Game::cg_s* cgs;
	extern Game::cgMedia_t* cgMedia;
	extern Game::sharedUiInfo_t* sharedUiInfo;
	extern int* g_waitingForKey;

	extern int* g_entities_int;

	extern DWORD* dword_FDBDCC;
	extern __int16* word_13E45E8;
	extern __int16* word_13E45DC;

	extern int* g_animRateOffsets;

	void SV_SendServerCommand(int clientNum, const char* fmt);
	void SV_GameSendServerCommand(int clientNum, const char* fmt);

	static Utils::function<void(Game::pmove_t* pm, Game::pml_t* pml)> PM_AirMove = 0x5BF480;
	static Utils::function<void(Game::pmove_t* pm)> PM_UpdateSprint = 0x5B72F0;


	extern char* byte_14C80E0;

	typedef void(__cdecl* CG_GameMessage_t)(const char* message, int a2);
	extern CG_GameMessage_t CG_GameMessage;

	extern int* dword_129ADC4;
	extern int* dword_129AFC8;

	extern Game::languageInfo_t* g_languages;

	const char* SEH_GetLanguageName(unsigned int iLanguage);
	const char* UI_SafeTranslateString/*eax*/(const char* reference /*eax*/);

	typedef const char*(__cdecl* UI_ReplaceConversionString_t)(const char* reference, const char* value);
	extern UI_ReplaceConversionString_t UI_ReplaceConversionString;

	int SEH_GetLanguageIndexFromName(const char* language, int* langindex);

	int I_stricmp/*eax*/(int a1/*eax*/, const char* a2 /*edx*/, const char* a3);

	static Utils::function<unsigned int()> G_RegisterWeapon = 0x4B6140;
	static Utils::function<unsigned int(const char* weaponName, void* registerWeaponFunction)> BG_GetWeaponIndexForName = 0x5BECE0;

	typedef unsigned int(__cdecl* BG_FindWeaponIndexForName_t)(const char* weaponName);
	extern BG_FindWeaponIndexForName_t BG_FindWeaponIndexForName;

	extern int* level_initializing;

	extern Game::localization_t* localization;

	extern game_hudelem_s* g_hudelems;
	extern game_hudelem_field_t* fields_0;

	void FS_DisplayPath(int bLanguageCull);

	extern Game::searchpath_s** fs_searchpaths;

	typedef void(__cdecl* FS_AddIwdFilesForGameDirectory_t)(const char* path, char* pszGameFolder);
	extern FS_AddIwdFilesForGameDirectory_t FS_AddIwdFilesForGameDirectory;

	void FS_AddLocalizedGameDirectory(const char* dir/*edi*/, const char* path);
	int FS_FOpenFileWriteToDir(const char* a1/*eax*/, const char* a2/*esi*/, int a3);

	void PM_Weapon_FireWeapon(Game::playerState_s* ps, int delayedAction);

	void StartWeaponAnim(int weaponIndex /*eax*/, Game::DObj_s* obj /*edi*/, int animIndex, float transitionTime);

	Game::Font_s* UI_GetFontHandleStock(int fontEnum, const ScreenPlacement* scrPlace, float scale);

	bool ShotLimitReached(Game::WeaponDef_s* weaponDef, Game::playerState_s* playerState);

	void PM_Weapon_BeginWeaponRaise(Game::playerState_s* playerState, int time, int anim, float aim, int altSwitch);

	static const char* g_he_font_old[] =
	{
		"default",		// HE_FONT_DEFAULT
		"bigfixed",		// HE_FONT_BIGFIXED
		"smallfixed",	// HE_FONT_SMALLFIXED
		"objective",	// HE_FONT_OBJECTIVE
		"big",			// HE_FONT_BIG
		"small",		// HE_FONT_SMALL
	};

	unsigned int G_GetWeaponIndexForName(const char* name);

	int DObjGetBoneIndex(Game::DObj_s* obj, unsigned int name, unsigned __int8* index);
	int XModelGetBoneIndex(Game::XModel* model, unsigned int name, unsigned int offset, unsigned __int8* index);

	void ChangeViewmodelDobj(int weapIndex /*eax*/, unsigned __int8 weaponModel /*cl*/, Game::XModel* newHands, Game::XModel* newGoggles, Game::XModel* newRocket, Game::XModel* newKnife, char updateClientInfo);
	float Vec2Normalize(float* v);
	void Cbuf_InsertText(int localClientNum /*eax*/, const char* text);
	void UI_KeyEvent(int down /*edi*/, int localClientNum, int key);
	void Key_WriteBindings(int localClientNum /*eax*/, char* buffer);

	typedef int(*FS_Printf_t)(int file, const char* fmt, ...);
	extern FS_Printf_t FS_Printf;

	void CL_DrawStretchPic(const Game::ScreenPlacement* ScrPlace, float x, float y, float w, float h, int horzAlign, int vertAlign, float s1, float t1, float s2, float t2, float* color, Game::Material* material);

	//Gamepad stuff
	constexpr auto KEY_NAME_COUNT = 95;
	constexpr auto LOCALIZED_KEY_NAME_COUNT = 95;
	extern Game::keyname_t* keyNames;
	extern Game::keyname_t* localizedKeyNames;

	typedef void(*UI_ReplaceConversions_t)(const char* sourceString, ConversionArguments* arguments, char* outputString);
	extern UI_ReplaceConversions_t UI_ReplaceConversions;

	int UI_GetActiveMenu();

	void UI_FilterStringForButtonAnimation(char* str, unsigned int strMaxSize);

	typedef void(*UI_SetActiveMenu_t)(int localClientNum, int menu);
	extern UI_SetActiveMenu_t UI_SetActiveMenu;

	void Key_SetBinding(int localClientNum /*eax*/, int keyNum /*ecx*/, const char* binding);

	void AimAssist_UpdateTweakables(const AimInput* input);
	void AimAssist_UpdateAdsLerp(const AimInput* input);
	typedef void(*AimAssist_ApplyAutoMelee_t)(const AimInput* input, const AimOutput* output);
	extern AimAssist_ApplyAutoMelee_t AimAssist_ApplyAutoMelee;
	void AimAssist_ApplyMeleeCharge(const AimInput* input /*eax*/, const AimOutput* output);

	extern Game::GraphFloat* aaInputGraph;

	float GraphGetValueFromFraction(int knotCount, const float(*knots)[2], float fraction);
	float GraphFloat_GetValue(const GraphFloat* graph, const float fraction);
	int BG_WeaponAmmo(int weaponIndex /*eax*/, const Game::playerState_s* ps /*ecx*/);
	void vectoangles(const float* vec /*esi*/, float* angles /*edi*/);
	float AngleNormalize360(float angle);
	float AngleNormalize180(float angle);

	typedef float(*DiffTrackAngle_t)(float tgt, float cur, float rate, float deltaTime);
	extern DiffTrackAngle_t DiffTrackAngle;

	typedef float(*AngleSubtract_t)(const float a1, const float a2);
	extern AngleSubtract_t AngleSubtract;

	unsigned int SEH_DecodeLetter(char firstChar, char secondChar, int* usedCount);
	unsigned int SEH_ReadCharFromString(const char** text);
	Glyph* R_GetCharacterGlyph(Font_s* font, unsigned int letter);

	typedef XAnimTree_s*(*CG_CreateWeaponViewModelXAnim_t)(Game::WeaponDef_s* weaponDef);
	extern CG_CreateWeaponViewModelXAnim_t CG_CreateWeaponViewModelXAnim;

	void RB_DrawStretchPicRotate(Game::Material* material, float x, float y, float w, float h, float s0, float t0, float s1, float t1, float sinAngle, float cosAngle, unsigned int color);

	void Sys_SnapVector();
	Game::vec_t Vec3Normalize(Game::vec3_t v);

	typedef void(*IN_RecenterMouse_t)();
	extern IN_RecenterMouse_t IN_RecenterMouse;

	typedef void(*IN_MouseMove_t)();
	extern IN_MouseMove_t IN_MouseMove;

	typedef void(*IN_Init_t)();
	extern IN_Init_t IN_Init;

	typedef void(*UI_MouseEvent_t)(int x, int y);
	extern UI_MouseEvent_t UI_MouseEvent;

	void UI_DrawHandlePic(const float* color /*ecx*/, Game::ScreenPlacement* ScrPlace /*edx*/, float x, float y, float w, float h, int horzAlign, int vertAlign, Game::Material* material);
}