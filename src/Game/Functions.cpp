#include "STDInc.hpp"

namespace Game
{
	HWND__* hWnd = reinterpret_cast<HWND__*>(0x13E39A8);
	HWND__* hWndParent = reinterpret_cast<HWND__*>(0x13E39B0);
	HWND__* hwndBuffer = reinterpret_cast<HWND__*>(0x13E39B4);

	HANDLE DatabaseHandle = reinterpret_cast<HANDLE>(0xFC6308);
	HANDLE databaseCompletedEvent2 = reinterpret_cast<HANDLE>(0xFC6348);

	//d3d9
	IDirect3D9** d3d9 = reinterpret_cast<IDirect3D9**>(0x1623F84);
	IDirect3DDevice9** dx9_device_ptr = reinterpret_cast<IDirect3DDevice9**>(0x1623F88);

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

	//Print_Info
	Com_Printf_t Com_Printf = Com_Printf_t(0x532DB0);
	Com_Error_t Com_Error = Com_Error_t(0x533580);

	//LoadAssets
	DB_LoadXAssets_t DB_LoadXAssets = (DB_LoadXAssets_t)0x45B990;
	DB_EnumXAssets_t DB_EnumXAssets = DB_EnumXAssets_t(0x45A8C0);

	//Files Things
	FS_FreeFile_t FS_FreeFile = (FS_FreeFile_t)0x580310; //temp, not correct function and offset from 1.0 version :(
	FS_ReadFile_t FS_ReadFile = (FS_ReadFile_t)0x578C50; //usercall func

	//XAssets
	XAssetHeader* DB_XAssetPool = reinterpret_cast<XAssetHeader*>(0x6DF200);
	unsigned int* g_poolSize = reinterpret_cast<unsigned int*>(0x6DEFA0);
	const char** g_assetNames = (const char**)0x6DF440;

	DB_FindXAssetHeader_t DB_FindXAssetHeader = DB_FindXAssetHeader_t(0x45AD10);
	DB_GetXAssetSizeHandler_t* DB_GetXAssetSizeHandlers = reinterpret_cast<DB_GetXAssetSizeHandler_t*>(0x6DF5F0);
	DB_GetXAssetNameHandler_t* DB_GetXAssetNameHandlers = reinterpret_cast<DB_GetXAssetNameHandler_t*>(0x6DF4D8);

	XAssetHeader DB_ReallocXAssetPool(XAssetType type, unsigned int new_size)
	{
		const XAssetHeader pool_entry =
		{
			Utils::Memory::GetAllocator()->allocate(new_size * DB_GetXAssetSizeHandlers[type]())
		};

		DB_XAssetPool[type] = pool_entry;
		g_poolSize[type] = new_size;

		return pool_entry;
	}

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

	int	sys_timeBase;
	int Sys_MilliSeconds(void)
	{
		int	sys_curtime;
		static bool	initialized = false;

		if (!initialized) {
			sys_timeBase = timeGetTime();
			initialized = true;
		}
		sys_curtime = timeGetTime() - sys_timeBase;

		return sys_curtime;
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

	DB_BeginRecoverLostDevice_t DB_BeginRecoverLostDevice = DB_BeginRecoverLostDevice_t(0x45B180);
	DB_EndRecoverLostDevice_t DB_EndRecoverLostDevice = DB_EndRecoverLostDevice_t(0x45B210);

	DB_IsXAssetDefault_t DB_IsXAssetDefault = DB_IsXAssetDefault_t(0x45B040);


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

	//	Game entities
	Game::cg_s* cgs = reinterpret_cast<Game::cg_s*>(0x6FA590);
	Game::playerState_s* ps = reinterpret_cast<Game::playerState_s*>(0x714BA8);
	Game::gentity_s* g_entities = reinterpret_cast<Game::gentity_s*>(0xC81418);
	Game::gclient_s* g_clients = reinterpret_cast<Game::gclient_s*>(0xE0DA00);
	Game::pmove_t* pmove = reinterpret_cast<Game::pmove_t*>(0x7FDE88);
	Game::clipMap_t* cm = reinterpret_cast<Game::clipMap_t*>(0xF788C8);

	Game::localization_t* localization = reinterpret_cast<Game::localization_t*>(0x13E0700);

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

	bool Key_IsCatcherActive(int localClientNum, int mask)
	{
		int keyCathers = *(BYTE*)0x85BD74;
		return (mask & keyCathers) != 0;
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

	//Command which doesn't work in 1.5 version because __usercall
	//R_TextWidth_t R_TextWidth = R_TextWidth_t(0x5D5250);

	//sharedUiInfo_t* sharedUiInfo = reinterpret_cast<sharedUiInfo_t*>(0x129AD80);
	Game::uiInfo_s* uiInfo = reinterpret_cast<Game::uiInfo_s*>(0x1290F50);

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

	const char* g_languages = reinterpret_cast<const char*>(0x6DD360);
	const char* SEH_GetLanguageName(unsigned int iLanguage)
	{
		//if (iLanguage > 14)
		//	return *(const char**)g_languages;
		//else
		//	return *(const char**)&g_languages[8 * iLanguage];

		if (iLanguage > 14)
			return g_languages_new[0];
		else
			return g_languages_new[iLanguage];
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
			if (!stricmp(language, Game::g_languages_new[i]))
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
	//58F8B0

	BG_FindWeaponIndexForName_t BG_FindWeaponIndexForName = BG_FindWeaponIndexForName_t(0x5BEC90);


	int Sys_IsDatabaseReady2(void)
	{
		return WaitForSingleObject(Game::databaseCompletedEvent2, 0) == 0;
	}


	void Sys_CreateConsole/*ax*/(HINSTANCE hInstance /*edi*/)
	{
		const static uint32_t Sys_CreateConsole_func = 0x5962B0;
		__asm
		{
			mov		edi, hInstance;
			call	Sys_CreateConsole_func;
		}
	}
	

	void Sys_ShowConsole()
	{
		if (!Game::hWnd)
		{
			HMODULE ModuleHandleA = GetModuleHandleA(0);
			Game::Sys_CreateConsole(ModuleHandleA);
		}
		ShowWindow(Game::hWndParent, 1);
		SendMessageA(Game::hwndBuffer, 182, 0, 65535);
	}


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
}