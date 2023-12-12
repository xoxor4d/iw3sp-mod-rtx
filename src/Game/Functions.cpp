#include "STDInc.hpp"

namespace Game
{
	//d3d9
	IDirect3D9** d3d9 = reinterpret_cast<IDirect3D9**>(0x1623F84);
	IDirect3DDevice9** dx9_device_ptr = reinterpret_cast<IDirect3DDevice9**>(0x1623F88);
	
	longjmp_internal_t longjmp_internal = longjmp_internal_t(0x64285C);

	//Cmd
	Cmd_ExecuteSingleCommand_t	Cmd_ExecuteSingleCommand = Cmd_ExecuteSingleCommand_t(0x531380);

	cmd_function_s* cmd_functions = reinterpret_cast<cmd_function_s*>(0xF78A6C);
	Game::CmdArgs* cmd_args = reinterpret_cast<Game::CmdArgs*>(0xF789E8);
	cmd_function_s** cmd_ptr = reinterpret_cast<cmd_function_s**>(0xF78A6C);

	//Cbuf_AddText_t Cbuf_AddText = Cbuf_AddText_t(0x530320); // in 1.0 this function works without __asm block
	void Cbuf_AddText(int localClientNum/*ecx*/, const char* text /*eax*/)
	{
		const static uint32_t Cbuf_AddText_func = 0x530320;
		__asm
		{
			mov		eax, text;
			mov		ecx, localClientNum;
			call	Cbuf_AddText_func;
		}
	}

	FS_FreeFile_t FS_FreeFile = (FS_FreeFile_t)0x580310; //temp, not correct function and offset from 1.0 version :(
	FS_ReadFile_t FS_ReadFile = (FS_ReadFile_t)0x578C50; //usercall func

	void Cmd_AddCommand(const char* name, void(*callback)(), cmd_function_s* data, char)
	{
		data->name = name;
		data->autoCompleteDir = nullptr;
		data->autoCompleteExt = nullptr;
		data->function = callback;
		data->next = *cmd_ptr;
		*cmd_ptr = data;
	}

	void Cmd_AddCommand(const char* name, const char* args, const char* description, void(*callback)(), cmd_function_s* data, char)
	{
		data->name = name;
		data->autoCompleteDir = args;
		data->autoCompleteExt = description;
		data->function = callback;
		data->next = *cmd_ptr;
		*cmd_ptr = data;
	}

	bool CL_IsCgameInitialized()
	{
		// Get the value from the offset.
		int result = *(BYTE*)0x85BD71;
		if (result == 1)
			return true;
		else
			return false;
	}

	FS_AddIwdFilesForGameDirectory_t FS_AddIwdFilesForGameDirectory = FS_AddIwdFilesForGameDirectory_t(0x579FD0);

	snd_alias_list_t* __cdecl Com_FindSoundAlias_FastFile(const char* name)
	{
		XAssetHeader header;

		header = DB_FindXAssetHeader(ASSET_TYPE_SOUND, name);

		if (DB_IsXAssetDefault(ASSET_TYPE_SOUND, name))
		{
			Game::Com_Error(10, "Missing soundalias \"%s\".\n", name);
			return NULL;
		}
		return header.sound;
	}

	snd_alias_list_t* Com_FindSoundAlias(const char* name)
	{
		const auto com_useFastFiles = Dvars::Functions::Dvar_FindVar("useFastFile");

		if (com_useFastFiles && com_useFastFiles->current.enabled)
		{
			return Com_FindSoundAlias_FastFile(name);
		}
		return Com_PickSoundAliasFromList(name);
	}

	Game::Font_s* __cdecl R_RegisterFont_FastFile(const char* fontname)
	{
		Game::XAssetHeader header;

		header = Game::DB_FindXAssetHeader(ASSET_TYPE_FONT, fontname);

		if (Game::DB_IsXAssetDefault(ASSET_TYPE_FONT, fontname))
		{
			Game::Com_Error(10, "Missing font file \"%s\".\n", fontname);
			return 0;
		}
		return header.font;
	}

	Game::Font_s* R_RegisterFont(const char* fontName, int imageTrack)
	{
		//const auto com_useFastFiles = Game::Dvar_FindVar("useFastFile");

		//if (com_useFastFiles && com_useFastFiles->current.enabled)
		//{
		//	return Game::R_RegisterFont_FastFile(fontName);
		//}
		return Game::R_RegisterFont_FastFile(fontName);
		//I don't see the point in using this function because the game always uses fastfiles by default
		//return R_RegisterFont_LoadObj(fontName); 
	}

	StringTable_LookupRowNumForValue_t StringTable_LookupRowNumForValue = (StringTable_LookupRowNumForValue_t)0x586010;

	const char* StringTable_Lookup(const StringTable* table /*<ebp>*/, const int comparisonColumn /*<esi>*/, const char* value/*<eax>*/, const int valueColumn /*<edi>*/)
	{
		int row;
		int result;
		
		if (!table)
		{
			Game::Com_Error(13, "Unable to find the lookup table in the fastfile, aborting lookup\n");
			return "";
		}
		row = Game::StringTable_LookupRowNumForValue(table, comparisonColumn, value);
		if (valueColumn >= table->columnCount)
			return "";
		if (row >= table->rowCount)
			return "";
		if (row < 0)
			return "";
		if (valueColumn < 0)
			return "";
		result = valueColumn + row * table->columnCount;
		if (!table->values[result])
			return "";
		return table->values[result];
	}


	void StringTable_GetAsset(const char* filename, const StringTable** tablePtr)
	{
		const Game::StringTable* stringTable = 0;
		const Game::StringTable* oldTable = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_STRINGTABLE, filename).stringTable;
		*tablePtr = oldTable;
		return;
	}

	Player_GetMethod_t* Player_GetMethod = reinterpret_cast<Player_GetMethod_t*>(0x49B2E0);
	Scr_GetFunction_t* Scr_GetFunction = reinterpret_cast<Scr_GetFunction_t*>(0x4DAD80);

	char* isDvarSystemActive = reinterpret_cast<char*>(0xE3AE5E);

	Scr_ParamError_t Scr_ParamError = Scr_ParamError_t(0x5902D0);
	int Scr_GetNumParam(void) {
		return *reinterpret_cast<int*>(0x128725C);
	}
	//From IW3xo
	DWORD* scrVarPub /*char[1024]*/ = reinterpret_cast<DWORD*>(0x126498C);
	int* scrVarPub_p4 /*scrVarpub+4*/ = reinterpret_cast<int*>(0x1264990);
	char* error_message /*char[1023]*/ = reinterpret_cast<char*>(0x128B560);
	char* errortype /*char[1023]*/ = reinterpret_cast<char*>(0x128B95F);

	void Scr_Error(const char* error)
	{
		*Game::scrVarPub_p4 = 1;								// display type?

		if (!*Game::scrVarPub)									// if no pointer to error_message
		{
			strncpy(*&Game::error_message, error, 1023u);		// copy errorText to location of error_message
			*Game::errortype = 0;								// what does that do?
			*Game::scrVarPub = (DWORD)(Game::error_message);	// scrVarPub pointing to location of error_message
		}

		Game::Scr_Error_Internal();
	}

	unsigned int Scr_GetConstString(unsigned int index /*eax*/)
	{
		const static uint32_t Scr_GetConstString_func = 0x558440;
		__asm
		{
			mov     eax, index;
			call	Scr_GetConstString_func;
		}
	}

	const char* SL_ConvertToString(unsigned int idx)
	{
		struct stringList
		{
			int unk;
			char string[1];
			int unk2;
		};

		return (*reinterpret_cast<stringList**>(0xFDBBC4))[idx & 0xFFFF].string;
	}

	const char* Scr_GetString(unsigned int index)
	{
		unsigned int ConstString;
		ConstString = Game::Scr_GetConstString(index);
		return SL_ConvertToString(ConstString);
	}

	float Scr_GetFloat/*st0*/(unsigned int index /*eax*/)
	{
		const static uint32_t Scr_GetFloat_func = 0x5583A0;
		__asm
		{
			mov		eax, index;
			xor		eax, eax;
			call	Scr_GetFloat_func;
		}
	}

	int Scr_GetInt/*eax*/(unsigned int index /*eax*/)
	{
		int result;
		const static uint32_t Scr_GetInt_func = 0x5580F0;
		__asm
		{
			mov		eax, index;
			call	Scr_GetInt_func;
			mov		result, eax;
		}
		return result;
	}

	Scr_AllocString_t Scr_AllocString = (Scr_AllocString_t)0x54CEC0;

	scr_const_t* scr_const = reinterpret_cast<scr_const_t*>(0xF523E8);

	unsigned int GScr_AllocString(const char* s)
	{
		unsigned int stringVal = Game::Scr_AllocString(s, 1, strlen(s) + 1);
		Game::Com_Printf(0, "GScr_AllocString: string '%s' ^2added\n", s);
		return stringVal;
	}

	// Game data
	Game::cgMedia_t* cgMedia = reinterpret_cast<Game::cgMedia_t*>(0x729400);
	Game::sharedUiInfo_t* sharedUiInfo = reinterpret_cast<Game::sharedUiInfo_t*>(0x129AD80);
	int* g_waitingForKey = reinterpret_cast<int*>(0x1E209C4);


	Game::cg_s* cgs = reinterpret_cast<Game::cg_s*>(0x6FA590);
	Game::playerState_s* ps = reinterpret_cast<Game::playerState_s*>(0x714BA8);
	Game::gclient_s* g_clients = reinterpret_cast<Game::gclient_s*>(0xE0DA00);
	Game::PlayerKeyState* playerKeys = reinterpret_cast<Game::PlayerKeyState*>(0x816764);
	Game::clientActive_t* clients = reinterpret_cast<Game::clientActive_t*>(0x85BD98);
	Game::clientUIActive_t* clientUIActives = reinterpret_cast<Game::clientUIActive_t*>(0x85BD70);
	Game::AimAssistGlobals* aaGlobArray = reinterpret_cast<Game::AimAssistGlobals*>(0x6E1788);
	//Game::clientStatic_t* cls = reinterpret_cast<Game::clientStatic_t*>(0x6E1788);

	Game::GraphFloat* aaInputGraph = reinterpret_cast<Game::GraphFloat*>(0x6E25F0);

	Game::pmove_t* pmove = reinterpret_cast<Game::pmove_t*>(0x7FDE88);

	Game::gentity_s* g_entities = reinterpret_cast<Game::gentity_s*>(0xC81418);

	Game::clipMap_t* cm = reinterpret_cast<Game::clipMap_t*>(0xF788C8);
	Game::weaponInfo_s* cg_weaponsArray = reinterpret_cast<Game::weaponInfo_s*>(0x6F7C88);
	Game::WinMouseVars_t* s_wmv = reinterpret_cast<Game::WinMouseVars_t*>(0x13E06F0);
	Game::snd_local_t* g_snd = reinterpret_cast<Game::snd_local_t*>(0x14C30E0);

	Game::scrVmPub_t* scrVmPub = reinterpret_cast<Game::scrVmPub_t*>(0x1287240);

	Game::GfxDrawMethod* gfxDrawMethod = reinterpret_cast<Game::GfxDrawMethod*>(0x1D9E0A0);

	Game::localization_t* localization = reinterpret_cast<Game::localization_t*>(0x13E0700);

	Game::game_hudelem_s* g_hudelems = reinterpret_cast<Game::game_hudelem_s*>(0xC74818);

	Game::game_hudelem_field_t* fields_0 = reinterpret_cast<Game::game_hudelem_field_t*>(0x6814B8);

	Scr_AddInt_t Scr_AddInt = Scr_AddInt_t(0x558B80);
	Scr_AddFloat_t Scr_AddFloat = Scr_AddFloat_t(0x558BC0);
	void Scr_AddString(const char* value /*esi*/)
	{
		const static uint32_t Scr_AddString_func = 0x558D10;
		__asm
		{
			mov		esi, value;
			call	Scr_AddString_func;
		}
	}

	bool Key_IsCatcherActive(int mask)
	{
		return (mask & Game::clientUIActives->keyCatchers);
	}

	const char* SEH_GetCurrentLanguage()
	{
		int language = Dvars::Functions::Dvar_FindVar("loc_language")->current.integer;
		const char* language_char;

		switch (language)
		{
			case 0: return language_char = "english"; break;
			case 1: return language_char = "french"; break;
			case 2: return language_char = "german"; break;
			case 3: return language_char = "italian"; break;
			case 4: return language_char = "spanish"; break;
			case 5: return language_char = "british"; break;
			case 6: return language_char = "russian"; break;
			case 7: return language_char = "polish"; break;
			case 8: return language_char = "korean"; break;
			case 9: return language_char = "taiwanese"; break;
			case 10: return language_char = "japanese"; break;
			case 11: return language_char = "chinese"; break;
			case 12: return language_char = "thai"; break;
			case 13: return language_char = "leet"; break;
			case 14: return language_char = "czech"; break;
			default: return "english"; break;
		}
	}

	GetPlayerEntity_t GetPlayerEntity = GetPlayerEntity_t(0x4C75E0);

	ScreenPlacement* scrPlace = reinterpret_cast<ScreenPlacement*>(0x8F8C68);
	ScreenPlacement* scrPlaceFull = reinterpret_cast<ScreenPlacement*>(0x8F8C20);
	ScreenPlacement* scrPlaceFullUnsafe = reinterpret_cast<ScreenPlacement*>(0x8F8CB0);

	Game::searchpath_s** fs_searchpaths = reinterpret_cast<Game::searchpath_s**>(0x1E209E4);

	ScreenPlacement* ScrPlace_GetFullPlacement()
	{
		return scrPlaceFull;
	}

	ScreenPlacement* ScrPlace_GetUnsafeFullPlacement()
	{
		return scrPlaceFullUnsafe;
	}

	SND_GetEntChannelFromName_t SND_GetEntChannelFromName = SND_GetEntChannelFromName_t(0x5CA4D0);

	//xoxor is very madness man! Thanks for code!
	void R_AddCmdDrawTextASM(const char* text, int max_chars, void* font, float x, float y, float x_scale, float y_scale, float rotation, const float* color, int style)
	{
		const static uint32_t R_AddCmdDrawText_func = 0x5DB7B0;
		__asm
		{
			push	style;
			sub     esp, 14h;

			fld		rotation;
			fstp[esp + 10h];

			fld		y_scale;
			fstp[esp + 0Ch];

			fld		x_scale;
			fstp[esp + 8];

			fld		y;
			fstp[esp + 4];

			fld		x;
			fstp[esp];

			push	font;
			push	max_chars;
			push	text;
			mov		ecx, [color];

			call	R_AddCmdDrawText_func;
			add		esp, 24h;
		}
	}

	void R_AddCmdDrawStretchPic(Game::Material* material, float x, float y, float w, float h, float null1, float null2, float null3, float null4, float* color)
	{
		const static uint32_t R_AddCmdDrawStretchPic_func = 0x5DB2A0;
		__asm
		{
			pushad;
			push	color;
			mov		eax, [material];
			sub		esp, 20h;

			fld		null4;
			fstp[esp + 1Ch];

			fld		null3;
			fstp[esp + 18h];

			fld		null2;
			fstp[esp + 14h];

			fld		null1;
			fstp[esp + 10h];

			fld		h;
			fstp[esp + 0Ch];

			fld		w;
			fstp[esp + 8h];

			fld		y;
			fstp[esp + 4h];

			fld		x;
			fstp[esp];

			call	R_AddCmdDrawStretchPic_func;
			add		esp, 24h;
			popad;
		}
	}

	//Something changed function, but anyway functionally.
	const char* UI_GetFontHandle(const ScreenPlacement* ScrPlace, float fontscale)
	{
		const char* font;
		float fontscalea = ScrPlace->scaleVirtualToReal[1] * fontscale;

		const auto ui_smallFont = Dvars::Functions::Dvar_FindVar("ui_smallFont");
		const auto ui_extraBigFont = Dvars::Functions::Dvar_FindVar("ui_extraBigFont");
		const auto ui_bigFont = Dvars::Functions::Dvar_FindVar("ui_bigFont");

		if (ui_smallFont && ui_smallFont->current.value < fontscalea)
		{
			if (ui_extraBigFont && ui_extraBigFont->current.value > fontscalea)
			{
				return font = "fonts/bigFont";

				if (ui_bigFont && ui_bigFont->current.value > fontscalea)
				{
					return font = "fonts/normalFont";
				}
			}
			else
			{
				return font = "fonts/extraBigFont";
			}
		}
		else
		{
			return font = "fonts/extraBigFont";
		}
	}

	Game::Font_s* UI_GetFontHandleStock(int fontEnum, const ScreenPlacement* ScrPlace, float scale)
	{
		//Game::Com_Printf(0, "UI_GetFontHandleStock: \nFontEnum: %d\nScale: %f\n ScrPlace: %d\n", fontEnum, ScrPlace, scale);
		Game::Font_s* bigFont = Game::R_RegisterFont("fonts/bigFont", sizeof("fonts/bigFont"));
		Game::Font_s* boldFont = Game::R_RegisterFont("fonts/boldFont", sizeof("fonts/boldFont"));
		Game::Font_s* consoleFont = Game::R_RegisterFont("fonts/consoleFont", sizeof("fonts/consoleFont"));
		Game::Font_s* extraBigFont = Game::R_RegisterFont("fonts/extraBigFont", sizeof("fonts/extraBigFont"));
		Game::Font_s* normalFont = Game::R_RegisterFont("fonts/normalFont", sizeof("fonts/normalFont"));
		Game::Font_s* objectiveFont = Game::R_RegisterFont("fonts/objectiveFont", sizeof("fonts/objectiveFont"));
		Game::Font_s* smallFont = Game::R_RegisterFont("fonts/smallFont", sizeof("fonts/smallFont"));
		//from mw2
		//Game::Font_s* hudsmallfont = Game::R_RegisterFont("fonts/hudsmallfont", sizeof("fonts/hudsmallfont"));
		//Game::Font_s* hudbigfont = Game::R_RegisterFont("fonts/hudbigfont", sizeof("fonts/hudbigfont"));

		switch (fontEnum)
		{
		case 1:
			return normalFont;
		case 2:
			return bigFont;
		case 3:
			return smallFont;
		case 4:
			return boldFont;
		case 5:
			return consoleFont;
		case 6:
			return objectiveFont;
		//case 7:
		//	return hudsmallfont;
		//case 8:
			//return hudbigfont;
		}

		double fontscalea = ScrPlace->scaleVirtualToReal[1] * scale;

		const auto ui_smallFont = Dvars::Functions::Dvar_FindVar("ui_smallFont");
		const auto ui_extraBigFont = Dvars::Functions::Dvar_FindVar("ui_extraBigFont");
		const auto ui_bigFont = Dvars::Functions::Dvar_FindVar("ui_bigFont");

		if (fontEnum == 4)
		{
			if (ui_smallFont->current.value >= fontscalea)
				return smallFont;
			if (ui_bigFont->current.value <= fontscalea)
				return boldFont;
			return normalFont;
		}
		if (ui_smallFont->current.value >= fontscalea)
			return smallFont;
		if (ui_extraBigFont->current.value <= fontscalea)
			return extraBigFont;
		if (ui_bigFont->current.value > fontscalea)
			return normalFont;
		return bigFont;
	}



	int R_TextHeight(Font_s* font)
	{
		return font->pixelHeight;
	}

	long double R_NormalizedTextScale(Font_s* font, float scale)
	{
		return (float)((float)(48.0 * scale) / (float)R_TextHeight(font));
	}

	int UI_TextWidth(const char* text, int maxChars, Font_s* font, float scale)
	{
		float normscale = R_NormalizedTextScale(font, scale);
		return (int)((float)R_TextWidth(text, maxChars, font) * normscale);
	}

	int UI_TextHeight(Font_s* font, float scale)
	{
		float normscale = R_NormalizedTextScale(font, scale);
		return (int)((float)R_TextHeight(font) * normscale);
	}

	void ScrPlace_ApplyRect(const ScreenPlacement* ScrPlace, float* x, float* y, float* w, float* h, int horzAlign, int vertAlign)
	{
		float v7;
		float v8;
		float v9;
		float v10;

		switch (horzAlign)
		{
		case 7:
			v7 = *x * ScrPlace->scaleVirtualToReal[0];
			v8 = (float)(ScrPlace->realViewableMin[0] + ScrPlace->realViewableMax[0]) * 0.5;
			*x = v7 + v8;
			*w = *w * ScrPlace->scaleVirtualToReal[0];
			break;
		case 6:
			*x = *x * ScrPlace->scaleRealToVirtual[0];
			*w = *w * ScrPlace->scaleRealToVirtual[0];
			break;
		case 5:
			break;
		case 4:
			*x = *x * ScrPlace->scaleVirtualToFull[0];
			*w = *w * ScrPlace->scaleVirtualToFull[0];
			break;
		case 3:
			*x = (float)(*x * ScrPlace->scaleVirtualToReal[0]) + ScrPlace->realViewableMax[0];
			*w = *w * ScrPlace->scaleVirtualToReal[0];
			break;
		case 2:
			v7 = *x * ScrPlace->scaleVirtualToReal[0];
			v8 = 0.5 * ScrPlace->realViewportSize[0];
			*x = v7 + v8;
			*w = *w * ScrPlace->scaleVirtualToReal[0];
			break;
		case 1:
			*x = (float)(*x * ScrPlace->scaleVirtualToReal[0]) + ScrPlace->realViewableMin[0];
			*w = *w * ScrPlace->scaleVirtualToReal[0];
			break;
		default:
			*x = (float)(*x * ScrPlace->scaleVirtualToReal[0]) + ScrPlace->subScreenLeft;
			*w = *w * ScrPlace->scaleVirtualToReal[0];
			break;
		}

		switch (vertAlign)
		{
		case 7:
			v9 = *y * ScrPlace->scaleVirtualToReal[1];
			v10 = (float)(ScrPlace->realViewableMin[1] + ScrPlace->realViewableMax[1]) * 0.5;
			*y = v9 + v10;
			*h = *h * ScrPlace->scaleVirtualToReal[1];
		case 6:
			*y = *y * ScrPlace->scaleRealToVirtual[1];
			*h = *h * ScrPlace->scaleRealToVirtual[1];
			break;
		case 5:
			return;
		case 4:
			*y = *y * ScrPlace->scaleVirtualToFull[1];
			*h = *h * ScrPlace->scaleVirtualToFull[1];
			break;
		case 3:
			*y = (float)(*y * ScrPlace->scaleVirtualToReal[1]) + ScrPlace->realViewableMax[1];
			*h = *h * ScrPlace->scaleVirtualToReal[1];
			break;
		case 2:
			v9 = *y * ScrPlace->scaleVirtualToReal[1];
			v10 = 0.5 * ScrPlace->realViewportSize[1];
			*y = v9 + v10;
			*h = *h * ScrPlace->scaleVirtualToReal[1];
			break;
		case 1:
			*y = (float)(*y * ScrPlace->scaleVirtualToReal[1]) + ScrPlace->realViewableMin[1];
			*h = *h * ScrPlace->scaleVirtualToReal[1];
			break;
		default:
			*y = *y * ScrPlace->scaleVirtualToReal[1];
			*h = *h * ScrPlace->scaleVirtualToReal[1];
			break;
		}
	}

	void UI_DrawText(const ScreenPlacement* ScrPlace, const char* text, int maxChars, Font_s* font, float ix, float iy, int horzAlign, int vertAlign, float scale, const float* color, int style)
	{
		float xScale = scale * 48.0f / static_cast<float>(font->pixelHeight);
		float yScale = xScale;

		ScrPlace_ApplyRect(ScrPlace, &ix, &iy, &xScale, &yScale, horzAlign, vertAlign);
		int x = floor(ix + 0.5);
		int y = floor(iy + 0.5);
		R_AddCmdDrawTextASM(text, maxChars, font, x, y, xScale, yScale, 0.0, color, style);
	}

	Game::uiInfo_s* uiInfo = reinterpret_cast<Game::uiInfo_s*>(0x1290F50);

	//Command which doesn't work in 1.5 version because __usercall
	//R_TextWidth_t R_TextWidth = R_TextWidth_t(0x5D5250);
	int R_TextWidth/*eax*/(const char* text /*eax*/, signed int maxChars, Game::Font_s* font/*ecx*/)
	{
		int result = 0;
		const static uint32_t R_TextWidth_func = 0x5D5250;
		__asm
		{
			push	font;
			push	maxChars;
			mov		eax, [text];
			call    R_TextWidth_func;
			add		esp, 0x8;
			mov		result, eax;
		}
		return result;
	}

	Game::WeaponDef_s** BG_WeaponNames = reinterpret_cast<Game::WeaponDef_s**>(0x14227A8);

	void SV_SendServerCommand(int clientNum, const char* fmt)
	{
		const static uint32_t SV_GameSendCommand_func = 0x5C7F00;
		__asm
		{
			mov		ecx, clientNum;
			push	fmt;
			call    SV_GameSendCommand_func;
			add     esp, 4;
		}
	}

	void SV_GameSendServerCommand(int clientNum, const char* fmt)
	{
		if (!Game::CL_IsCgameInitialized())
		{
			Game::Com_Printf(0, "Game [^2DEBUG^7]: you can't activate this command when you in main menu.\n");
			return;
		}

		if (clientNum == -1)
			Game::SV_SendServerCommand(0, fmt);
	}

	void Dvar_SetStringByName(const char* dvarName, const char* value)
	{
		const static uint32_t Dvar_SetStringByName_func = 0x589720;
		__asm
		{
			mov		eax, dvarName;
			push	value;
			call	Dvar_SetStringByName_func;
			add     esp, 4;
		}
	}

	CG_GameMessage_t CG_GameMessage = CG_GameMessage_t(0x41F560);

	int* dword_129ADC4 = reinterpret_cast<int*>(0x129ADC4);
	int* dword_129AFC8 = reinterpret_cast<int*>(0x129AFC8);

	Game::languageInfo_t* g_languages = reinterpret_cast<Game::languageInfo_t*>(0x6DD360);
	const char* SEH_GetLanguageName(unsigned int iLanguage)
	{
		if (iLanguage > 14)
			return Game::g_languages[0].pszName;
		else
			return Game::g_languages[iLanguage].pszName;
	}

	const char* UI_SafeTranslateString/*eax*/(const char* reference /*eax*/)
	{
		const static uint32_t UI_SafeTranslateString_func = 0x5680C0;

		const char* result;
		__asm
		{
			mov     eax, reference;
			call	UI_SafeTranslateString_func;
			mov		result, eax;
		}
		return result;
	}

	UI_ReplaceConversionString_t UI_ReplaceConversionString = UI_ReplaceConversionString_t(0x5681D0);

	int SEH_GetLanguageIndexFromName(const char* language, int* langindex)
	{
		for (int i = 0; i < 15; i++)
		{
			if (!stricmp(language, Game::g_languages[i].pszName))
			{
				return *langindex = i;
			}
		}
		return *langindex = 0;
	}

	int I_stricmp/*eax*/(int a1/*eax*/, const char* a2 /*edx*/, const char* a3)
	{
		const static uint32_t I_stricmp_func = 0x58FFD0;
		int result;
		__asm
		{
			push	a3;
			mov		eax, a1;
			mov		edx, a2;
			call	I_stricmp_func;
			add		esp, 4;
			mov		result, eax;
		}
		return result;
	}

	BG_FindWeaponIndexForName_t BG_FindWeaponIndexForName = BG_FindWeaponIndexForName_t(0x5BEC90);

	//temp
	int* level_initializing = reinterpret_cast<int*>(0xE18E40);

	void FS_DisplayPath(int bLanguageCull)
	{
		const static uint32_t FS_DisplayPath_func = 0x579C80;
		__asm
		{
			mov		eax, bLanguageCull;
			call	FS_DisplayPath_func;
		}
	}

	void FS_AddLocalizedGameDirectory(const char* dir/*edi*/, const char* path)
	{
		const static uint32_t FS_AddLocalizedGameDirectory_func = 0x57A6D0;
		__asm
		{
			push	path;
			mov		edi, dir;
			call	FS_AddLocalizedGameDirectory_func;
			add		esp, 4;
		}
	}

	int FS_FOpenFileWriteToDir(const char* a1/*eax*/, const char* a2/*esi*/, int a3)
	{
		const static uint32_t FS_FOpenFileWriteToDir_func = 0x577E30;
		int result;
		__asm
		{
			push	a3;
			mov		eax, a1;
			mov		esi, a2;
			call	FS_FOpenFileWriteToDir_func;
			add		esp, 4;
			mov		result, eax;
		}
		return result;
	}

	void PM_Weapon_FireWeapon(Game::playerState_s* playerState, int delayedAction)
	{
		const static uint32_t PM_Weapon_FireWeapon_func = 0x5C18D0;
		__asm
		{
			push	delayedAction;
			mov		eax, playerState;
			call	PM_Weapon_FireWeapon_func;
			add		esp, 4;
		}
	}

	bool ShotLimitReached(Game::WeaponDef_s* weaponDef, Game::playerState_s* playerState)
	{
		bool result;
		const static uint32_t ShotLimitReached_func = 0x5C1020;
		__asm
		{
			mov		eax, weaponDef;
			mov		ecx, ps;
			call	ShotLimitReached_func;
			mov		result, al;
		}
		return result;
	}

	void StartWeaponAnim(int weaponIndex /*eax*/, Game::DObj_s* obj /*edi*/, int animIndex, float transitionTime)
	{
		const static uint32_t StartWeaponAnim_func = 0x4307C0;
		__asm
		{
			pushad;
			mov		eax, weaponIndex;
			mov		edi, obj;
			push	transitionTime;
			push	animIndex;
			call	StartWeaponAnim_func;
			add		esp, 8;
			popad;
		}
	}

	void PM_Weapon_BeginWeaponRaise(Game::playerState_s* playerState, int time, int anim, float aim, int altSwitch)
	{
		const static uint32_t PM_Weapon_BeginWeaponRaise_func = 0x5C03C0;
		__asm
		{
			pushad;
			mov		eax, playerState;
			mov		edx, time;
			push	altSwitch;
			push	aim;
			push	anim;
			call	PM_Weapon_BeginWeaponRaise_func;
			add		esp, 0xC;
			popad;
		}
	}

	unsigned int G_GetWeaponIndexForName(const char* name)
	{
		if(level_initializing)
			return Game::BG_GetWeaponIndexForName(name, Game::G_RegisterWeapon);
		else
			return Game::BG_FindWeaponIndexForName(name);
	}

	void CL_DrawStretchPic(const Game::ScreenPlacement* ScrPlace, float x, float y, float w, float h, int horzAlign, int vertAlign, float s1, float t1, float s2, float t2, float* color, Game::Material* material)
	{
		ScrPlace_ApplyRect(ScrPlace, &x, &y, &w, &h, horzAlign, vertAlign);
		R_AddCmdDrawStretchPic(material, x, y, w, h, s1, t1, s2, t2, color);
	}

	int XModelGetBoneIndex(Game::XModel* model, unsigned int name, unsigned int offset, unsigned __int8* index)
	{
		unsigned int numBones;
		unsigned int localBoneIndex;
		unsigned __int16* boneNames;

		boneNames = model->boneNames;
		numBones = model->numBones;

		for (localBoneIndex = 0; ; ++localBoneIndex)
		{
			if (localBoneIndex >= numBones)
				return 0;
			if (name == boneNames[localBoneIndex])
				break;
		}
		*index = localBoneIndex + offset;
		return 1;
	}

	/*
	int DObjGetBoneIndex(Game::DObj_s* obj, unsigned int name, unsigned __int8* index)
	{
		int j; // [esp+0h] [ebp-18h]
		int ja; // [esp+0h] [ebp-18h]
		unsigned int boneIndex; // [esp+4h] [ebp-14h]
		int numModels; // [esp+8h] [ebp-10h]
		Game::XModel* model; // [esp+Ch] [ebp-Ch]
		Game::XModel* modela; // [esp+Ch] [ebp-Ch]
		unsigned int localBoneIndex; // [esp+10h] [ebp-8h]
		Game::XModel** models; // [esp+14h] [ebp-4h]

		localBoneIndex = *index;
		if (localBoneIndex == 255)
			return 0;
		models = obj->models;

		numModels = obj->numModels;
		if (localBoneIndex < obj->numBones)
		{
			for (j = 0; j < numModels; ++j)
			{
				model = models[j];
				if (localBoneIndex < model->numBones)
				{
					if (name != model->boneNames[localBoneIndex])
						break;
					return 1;
				}
				localBoneIndex -= model->numBones;
			}
		}
		boneIndex = 0;
		for (ja = 0; ja < numModels; ++ja)
		{
			modela = models[ja];
			if (XModelGetBoneIndex(modela, name, boneIndex, index))
				return 1;
			boneIndex += modela->numBones;
		}
		*index = -1;
		return 0;
	}
	*/

	//skill issue
	int DObjGetBoneIndex(Game::DObj_s* obj, unsigned int name, unsigned __int8* index)
	{
		return 1;
	}

	void ChangeViewmodelDobj(int weapIndex /*eax*/, unsigned __int8 weaponModel /*cl*/, Game::XModel* newHands, Game::XModel* newGoggles, Game::XModel* newRocket, Game::XModel* newKnife, char updateClientInfo)
	{
		const static uint32_t ChangeViewmodelDobj_func = 0x431400;
		__asm
		{
			pushad;
			mov		eax, weapIndex;
			mov		cl, weaponModel;
			push	1;
			push	newKnife;
			push	newRocket;
			push	newGoggles;
			push	newHands;
			call	ChangeViewmodelDobj_func;
			add		esp, 0x14;
			popad;
		}
	}

	float Vec2Normalize(float* v)
	{
		/*i'm not sure that this code return the correct result*/
		float result;
		const static uint32_t Vec2Normalize_func = 0x57BA10;
		__asm
		{
			mov		esi, [v];
			call	Vec2Normalize_func;
			fstp	[result];
		}

		return result;
	}

	void Cbuf_InsertText(int localClientNum /*eax*/, const char* text)
	{
		const static uint32_t Cbuf_InsertText_func = 0x5303E0;
		__asm
		{
			pushad;
			mov		eax, localClientNum;
			push	text;
			call	Cbuf_InsertText_func;
			add		esp, 4;
			popad;
		}
	}

	//(int a1@<ecx>, int a2@<edi>, int a3, int a4)
	void UI_KeyEvent(int down /*edi*/, int localClientNum, int key)
	{
		const static uint32_t UI_KeyEvent_func = 0x567C80;
		__asm
		{
			mov		edi, down;
			push	key;
			push	localClientNum;
			call	UI_KeyEvent_func;
			add		esp, 8;
		}
	}

	void Key_WriteBindings(int localClientNum /*eax*/, char* buffer)
	{
		const static uint32_t Key_WriteBindings_func = 0x443B00;
		__asm
		{
			pushad;
			mov		eax, localClientNum;
			push	buffer;
			call	Key_WriteBindings_func;
			add		esp, 4;
			popad;
		}
	}

	FS_Printf_t FS_Printf = (FS_Printf_t)0x578A90;

	//Gamepad stuff
	keyname_t* keyNames = reinterpret_cast<keyname_t*>(0x6DFB30);
	keyname_t* localizedKeyNames = reinterpret_cast<keyname_t*>(0x6DFE30);

	UI_ReplaceConversions_t UI_ReplaceConversions = UI_ReplaceConversions_t(0x568240);

	int UI_GetActiveMenu()
	{
		return *(int*)(0x1E209B4);
	}

	void updateButtonString(char* str, unsigned int strMaxSize, const char* findStr, const char* replaceStr) {
		auto location = strstr(str, findStr);

		if (location) {
			std::size_t replaceOffset = location - str;
			std::size_t findStrLen = strlen(findStr);
			std::size_t replaceStrLen = strlen(replaceStr);

			if (replaceOffset + replaceStrLen < strMaxSize) {
				memmove(location + replaceStrLen,
					location + findStrLen,
					strMaxSize - replaceOffset - findStrLen);

				for (int i = 0; i < static_cast<int>(replaceStrLen); ++i) {
					location[i] = replaceStr[i];
				}
			}
		}
	}

	void UI_FilterStringForButtonAnimation(char* str, unsigned int strMaxSize)
	{
		const auto remainder = Game::Sys_MilliSeconds() % 1000;
		if (remainder <= 800)
		{
			return;
		}

		// Sony Icons
		if (std::strstr(str, "button_sony_09"))
		{
			updateButtonString(str, strMaxSize, "button_sony_09", "button_sony_17");
			if (std::strstr(str, "button_sony_10"))
				updateButtonString(str, strMaxSize, "button_sony_10", "button_sony_18");
		}
		else if (std::strstr(str, "button_sony_17"))
		{
			updateButtonString(str, strMaxSize, "button_sony_17", "button_sony_09");
			if (std::strstr(str, "button_sony_18"))
				updateButtonString(str, strMaxSize, "button_sony_18", "button_sony_10");
		}
		else if (std::strstr(str, "button_sony_10"))
		{
			updateButtonString(str, strMaxSize, "button_sony_10", "button_sony_18");
			if (std::strstr(str, "button_sony_10"))
				updateButtonString(str, strMaxSize, "button_sony_10", "button_sony_18");
		}
		else if (std::strstr(str, "button_sony_18"))
		{
			updateButtonString(str, strMaxSize, "button_sony_18", "button_sony_10");
			if (std::strstr(str, "button_sony_18"))
				updateButtonString(str, strMaxSize, "button_sony_18", "button_sony_10");
		}

		// Xbox Icons
		if (std::strstr(str, "button_xbox_09"))
		{
			updateButtonString(str, strMaxSize, "button_xbox_09", "button_xbox_17");
			if (std::strstr(str, "button_xbox_10"))
				updateButtonString(str, strMaxSize, "button_xbox_10", "button_xbox_18");
		}
		else if (std::strstr(str, "button_xbox_17"))
		{
			updateButtonString(str, strMaxSize, "button_xbox_17", "button_xbox_09");
			if (std::strstr(str, "button_xbox_18"))
				updateButtonString(str, strMaxSize, "button_xbox_18", "button_xbox_10");
		}
		else if (std::strstr(str, "button_xbox_10"))
		{
			updateButtonString(str, strMaxSize, "button_xbox_10", "button_xbox_18");
			if (std::strstr(str, "button_xbox_10"))
				updateButtonString(str, strMaxSize, "button_xbox_10", "button_xbox_18");
		}
		else if (std::strstr(str, "button_xbox_18"))
		{
			updateButtonString(str, strMaxSize, "button_xbox_18", "button_xbox_10");
			if (std::strstr(str, "button_xbox_18"))
				updateButtonString(str, strMaxSize, "button_xbox_18", "button_xbox_10");
		}
	}

	UI_SetActiveMenu_t UI_SetActiveMenu = UI_SetActiveMenu_t(0x567E00);

	void Key_SetBinding(int localClientNum /*eax*/, int keyNum /*ecx*/, const char* binding)
	{
		const static uint32_t Key_SetBinding_func = 0x4437B0;
		__asm
		{
			pushad;
			mov		ecx, keyNum;
			mov		eax, localClientNum;
			push	binding;
			call	Key_SetBinding_func;
			add		esp, 4;
			popad;
		}
	}

	void AimAssist_UpdateTweakables(const AimInput* input /*eax*/)
	{
		const static uint32_t AimAssist_UpdateTweakables_func = 0x401170;
		__asm
		{
			mov		eax, input;
			call	AimAssist_UpdateTweakables_func;
		}
	}

	void AimAssist_UpdateAdsLerp(const AimInput* input)
	{
		auto& aaGlob = Game::aaGlobArray[input->localClientNum];
		aaGlob.adsLerp = input->ps->fWeaponPosFrac;
		if ((input->ps->eFlags & 0x300) != 0 && (input->buttons & Game::CMD_BUTTON_ADS) != 0)
		{
			aaGlob.adsLerp = 1.0;
		}
	}

	AimAssist_ApplyAutoMelee_t AimAssist_ApplyAutoMelee = AimAssist_ApplyAutoMelee_t(0x402690);

	void AimAssist_ApplyMeleeCharge(const AimInput* input /*eax*/, const AimOutput* output)
	{
		const static uint32_t AimAssist_ApplyMeleeCharge_func = 0x402880;
		__asm
		{
			mov		eax, input;
			push	output;
			call	AimAssist_ApplyMeleeCharge_func;
			add		esp, 4;
		}
	}

	float GraphGetValueFromFraction(const int knotCount, const float(*knots)[2], const float fraction)
	{
		for (auto knotIndex = 1; knotIndex < knotCount; ++knotIndex)
		{
			if (knots[knotIndex][0] >= fraction)
			{
				const auto adjustedFraction = (fraction - knots[knotIndex - 1][0]) / (knots[knotIndex][0] - knots[knotIndex - 1][0]);

				return (knots[knotIndex][1] - knots[knotIndex - 1][1]) * adjustedFraction + knots[knotIndex - 1][1];
			}
		}

		return -1.0f;
	}

	float GraphFloat_GetValue(const GraphFloat* graph, const float fraction)
	{
		return GraphGetValueFromFraction(graph->knotCount, graph->knots, fraction) * graph->scale;
	}

	int BG_WeaponAmmo(int weaponIndex /*eax*/, const Game::playerState_s* playerState /*ecx*/)
	{
		int result;
		const static uint32_t BG_WeaponAmmo_func = 0x5BFA70;
		__asm
		{
			mov		eax, weaponIndex;
			mov		ecx, playerState;
			call	BG_WeaponAmmo_func;
			mov		result, eax;
		}

		return result;
	}

	void vectoangles(const float* vec /*esi*/, float* angles /*edi*/)
	{
		const static uint32_t vectoangles_func = 0x57BFE0;
		__asm
		{
			mov		esi, [vec];
			mov		edi, [angles];
			call	vectoangles_func;
		}
	}

	float AngleNormalize360(float angle)
	{
		return (360.0 / 65536) * ((int)(angle * (65536 / 360.0)) & 65535);
	}

	float AngleNormalize180(float angle) {
		angle = AngleNormalize360(angle);
		if (angle > 180.0) {
			angle -= 360.0;
		}
		return angle;
	}

	DiffTrackAngle_t DiffTrackAngle = DiffTrackAngle_t(0x57B670);
	AngleSubtract_t AngleSubtract = AngleSubtract_t(0x57DCB0);

	bool sub_55C330(int a1, int a2) {
		return a1 - 176 <= 0x18 && a2 - 161 <= 0x5D;
	}

	bool Taiwanese_ValidBig5Code(uint16_t uiCode) {
		return ((((uiCode >> 8) >= 0xA1 && (uiCode >> 8) <= 0xC6) || (((uiCode >> 8) + 55) <= 0x30)) && (((uiCode & 0xFF) >= 0x40 && (uiCode & 0xFF) <= 0x7E) || ((uiCode & 0xFF) >= 0xA1 && (uiCode & 0xFF) != 0xFF)));
	}

	bool sub_55C380(uint16_t a1, uint16_t a2) {
		return ((a1 >= 0x81 && a1 <= 0x9F) || a1 - 224 <= 0xF) && ((a2 >= 0x40 && a2 <= 0x7E) || a2 - 128 <= 0x7C);
	}

	bool sub_55C3B0(uint16_t a1) {
		return (static_cast<uint8_t>(a1 >> 8) + 127) <= 0x7D && (static_cast<uint8_t>(a1) > 0x40) && (static_cast<uint8_t>(a1) != 0xFF);
	}

	unsigned int SEH_DecodeLetter(byte firstChar, byte secondChar, int* usedCount)
	{
		int g_currentAsian = *(int*)0x128F9AC;
		unsigned int v7;

		if (g_currentAsian)
		{
			switch (Dvars::Functions::Dvar_FindVar("loc_language")->current.integer)
			{
			case 8:
				if (sub_55C330(firstChar, secondChar)) 
				{
					*usedCount = 2;
					return secondChar + (firstChar << 8);
				}
				break;
			case 9:
				v7 = secondChar + (firstChar << 8);
				if (Taiwanese_ValidBig5Code(v7)) 
				{
					*usedCount = 2;
					return v7;
				}
				break;
			case 10:
				if (sub_55C380(firstChar, secondChar)) 
				{
					*usedCount = 2;
					return secondChar + (firstChar << 8);
				}
				break;
			case 11:
				v7 = secondChar + (firstChar << 8);
				if (sub_55C3B0(v7)) 
				{
					*usedCount = 2;
					return v7;
				}
				break;
			}
		}

		*usedCount = 1;
		return firstChar;
	}

	unsigned int SEH_ReadCharFromString(const char** text)
	{
		int usedCount;
		unsigned int letter;
		letter = Game::SEH_DecodeLetter(*(unsigned char*)*text, *((unsigned char*)*text + 1), &usedCount);
		*text += usedCount;
		return letter;
	}

	Glyph* R_GetCharacterGlyph(Font_s* font, unsigned int letter /*edi*/)
	{
		Game::Glyph* result;
		const static uint32_t R_GetCharacterGlyph_func = 0x5D50D0;
		__asm
		{
			mov		edi, letter;
			push	font;
			call	R_GetCharacterGlyph_func;
			add		esp, 4;
			mov		result, eax;
		}
		return result;
	}


	CG_CreateWeaponViewModelXAnim_t CG_CreateWeaponViewModelXAnim = CG_CreateWeaponViewModelXAnim_t(0x430EA0);

	__declspec(naked) void RB_DrawStretchPicRotate(Material* /*material*/, float /*x*/, float /*y*/, float /*w*/, float /*h*/, float /*s0*/, float /*t0*/, float /*s1*/, float /*t1*/, float /*sinAngle*/, float /*cosAngle*/, unsigned int /*color*/)
	{
		__asm
		{
			pushad

			mov eax, [esp + 0x4 + 0x20] // material
			push[esp + 0x30 + 0x20] // color
			push[esp + 0x30 + 0x20] // cosAngle
			push[esp + 0x30 + 0x20] // sinAngle
			push[esp + 0x30 + 0x20] // t1
			push[esp + 0x30 + 0x20] // s1
			push[esp + 0x30 + 0x20] // t0
			push[esp + 0x30 + 0x20] // s0
			push[esp + 0x30 + 0x20] // h
			push[esp + 0x30 + 0x20] // w
			push[esp + 0x30 + 0x20] // y
			push[esp + 0x30 + 0x20] // x
			mov ebx, 0x5FB310
			call ebx
			add esp, 0x2C

			popad
			ret
		}
	}

	void Sys_SnapVector()
	{
		Game::g_clients->ps.velocity[0] = rint(Game::g_clients->ps.velocity[0]);
		Game::g_clients->ps.velocity[1] = rint(Game::g_clients->ps.velocity[1]);
		Game::g_clients->ps.velocity[2] = rint(Game::g_clients->ps.velocity[2]);
	}

	Game::vec_t Vec3Normalize(Game::vec3_t v) 
	{
		float length, ilength;

		length = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
		length = sqrt(length);

		if (length) {
			ilength = 1 / length;
			v[0] *= ilength;
			v[1] *= ilength;
			v[2] *= ilength;
		}

		return length;
	}
	
	
	IN_RecenterMouse_t IN_RecenterMouse = IN_RecenterMouse_t(0x594570);
	IN_MouseMove_t IN_MouseMove = IN_MouseMove_t(0x594730);
	IN_Init_t IN_Init = IN_Init_t(0x5947F0);
	UI_MouseEvent_t UI_MouseEvent = UI_MouseEvent_t(0x567D30);

	void UI_DrawHandlePic(const float* color /*ecx*/, Game::ScreenPlacement* ScrPlace /*edx*/, float x, float y, float w, float h, int horzAlign, int vertAlign, Game::Material* material)
	{
		const static uint32_t UI_DrawHandlePic_func = 0x55F3D0;
		__asm
		{
			pushad;
			push	material;
			push	vertAlign;
			push	horzAlign;

			sub     esp, 10h;

			fld		h;
			fstp	[esp + 0Ch];

			fld		w;
			fstp	[esp + 8h];

			fld		y;
			fstp	[esp + 4h];

			fld		x;
			fstp	[esp];

			mov		ecx, [color];
			mov		edx, [ScrPlace];

			call	UI_DrawHandlePic_func;
			add		esp, 1Ch;
			popad;
		}
	}
}