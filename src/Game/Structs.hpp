#pragma once

// This allows us to compile our structures in IDA, for easier reversing :3
#ifndef __cplusplus
#define IDA
#endif

#ifndef IDA
namespace Game
{
	union GfxHullPointsPool;
	struct GfxPortal;
	struct GfxAabbTree;
#endif

	typedef float vec_t;
	typedef vec_t vec2_t[2];
	typedef vec_t vec3_t[3];
	typedef vec_t vec4_t[4];

	typedef unsigned __int16 scr_string_t;

	struct scr_entref_t
	{
		scr_string_t entnum;
		scr_string_t classnum;
	};

	typedef void(*xfunction_t)();
	typedef void (*xmethod_t)(scr_entref_t);

	typedef struct scr_function_s
	{
		struct scr_function_s* next;
		char* name;
		xfunction_t		function;
		bool		developer;
	} scr_function_t;

	typedef struct scr_method_s
	{
		struct scr_method_s* next;
		char* name;
		xmethod_t function;
		bool developer;
	} scr_method_t;

	enum XFILE_BLOCK_TYPES
	{
		XFILE_BLOCK_TEMP = 0x0,
		XFILE_BLOCK_RUNTIME_BEGIN = 0x1,
		XFILE_BLOCK_RUNTIME = 0x1,
		XFILE_BLOCK_LARGE_RUNTIME = 0x2,
		XFILE_BLOCK_PHYSICAL_RUNTIME = 0x3,
		XFILE_BLOCK_RUNTIME_END = 0x4,
		XFILE_BLOCK_VIRTUAL = 0x4,
		XFILE_BLOCK_LARGE = 0x5,
		XFILE_BLOCK_PHYSICAL = 0x6,
		MAX_XFILE_COUNT = 0x7,
	};

	struct XBlock
	{
		char* data;
		unsigned int size;
	};

	struct XZoneMemory
	{
		// <- rtx begin

		//XBlock blocks[MAX_XFILE_COUNT];
		XBlock blocks[9];

		// rtx end ->

		char* lockedVertexData;
		char* lockedIndexData;
		IDirect3DVertexBuffer9* vertexBuffer;
		IDirect3DIndexBuffer9* indexBuffer;
	};

	struct __declspec(align(4)) XZone
	{
		char name[64];
		int flags;
		int allocType;
		XZoneMemory mem;
		// <- rtx begin
		//int fileSize;
		//bool modZone;
		// rtx end ->
	}; // size 0xA0


	struct XFile
	{
		unsigned int size;
		unsigned int externalSize;
		unsigned int blockSize[MAX_XFILE_COUNT];
	};

	enum XAssetType
	{
		ASSET_TYPE_XMODELPIECES = 0x0,
		ASSET_TYPE_PHYSPRESET = 0x1,
		ASSET_TYPE_XANIMPARTS = 0x2,
		ASSET_TYPE_XMODEL = 0x3,
		ASSET_TYPE_MATERIAL = 0x4,
		ASSET_TYPE_TECHNIQUE_SET = 0x5,
		ASSET_TYPE_IMAGE = 0x6,
		ASSET_TYPE_SOUND = 0x7,
		ASSET_TYPE_SOUND_CURVE = 0x8,
		ASSET_TYPE_LOADED_SOUND = 0x9,
		ASSET_TYPE_CLIPMAP = 0xA,
		ASSET_TYPE_CLIPMAP_PVS = 0xB,
		ASSET_TYPE_COMWORLD = 0xC,
		ASSET_TYPE_GAMEWORLD_SP = 0xD,
		ASSET_TYPE_GAMEWORLD_MP = 0xE,
		ASSET_TYPE_MAP_ENTS = 0xF,
		ASSET_TYPE_GFXWORLD = 0x10,
		ASSET_TYPE_LIGHT_DEF = 0x11,
		ASSET_TYPE_UI_MAP = 0x12,
		ASSET_TYPE_FONT = 0x13,
		ASSET_TYPE_MENULIST = 0x14,
		ASSET_TYPE_MENU = 0x15,
		ASSET_TYPE_LOCALIZE_ENTRY = 0x16,
		ASSET_TYPE_WEAPON = 0x17,
		ASSET_TYPE_SNDDRIVER_GLOBALS = 0x18,
		ASSET_TYPE_FX = 0x19,
		ASSET_TYPE_IMPACT_FX = 0x1A,
		ASSET_TYPE_AITYPE = 0x1B,
		ASSET_TYPE_MPTYPE = 0x1C,
		ASSET_TYPE_CHARACTER = 0x1D,
		ASSET_TYPE_XMODELALIAS = 0x1E,
		ASSET_TYPE_RAWFILE = 0x1F,
		ASSET_TYPE_STRINGTABLE = 0x20,
		ASSET_TYPE_COUNT = 0x21,
		ASSET_TYPE_STRING = 0x21,
		ASSET_TYPE_ASSETLIST = 0x22,
	};

	//Sounds Structure
	typedef enum
	{
		SAT_UNKNOWN = 0x0,
		SAT_LOADED = 0x1,
		SAT_STREAMED = 0x2,
		SAT_PRIMED = 0x3,
		SAT_COUNT = 0x4,
	}snd_alias_type_t;

	struct SndCurve
	{
		const char* filename;
		int knotCount;
		float knots[8][2];
	};

	typedef struct SpeakerLevels_s
	{
		int speaker;
		int numLevels;
		float levels[2];
	}SpeakerLevels_t;

	typedef struct ChannelMap_s
	{
		int entryCount;	// how many entries are used
		SpeakerLevels_t speakers[6];
	}ChannelMap_t;

	typedef struct SpeakerMap_s
	{
		byte isDefault;
		byte pad[3];
		const char* name;
		ChannelMap_t channelMaps[2][2];
	}SpeakerMap_t;

	typedef struct _AILSOUNDINFO
	{
		signed int format;
		const void* data_ptr;
		unsigned int data_len;
		unsigned int rate;
		signed int bits;
		signed int channels;
		unsigned int samples;
		unsigned int block_size;
		const void* initial_ptr;
	}AILSOUNDINFO;

	struct MssSound
	{
		_AILSOUNDINFO info;
		char* data;
	};

	struct LoadedSound
	{
		const char* name;
		MssSound sound;
	};

	struct StreamFileNameRaw
	{
		const char* dir;
		const char* name;
	};

	union StreamFileInfo
	{
		StreamFileNameRaw raw;
	};

	struct StreamFileName
	{
		StreamFileInfo info;
	};

	struct StreamedSound
	{
		StreamFileName filename;
	};

	union SoundFileRef
	{
		LoadedSound* loadSnd;
		StreamedSound streamSnd;
	};

	typedef struct SoundFile_s
	{
		char type;
		char exists;
		SoundFileRef u;
	}SoundFile_t;

	struct snd_alias_t
	{
		const char* aliasName;
		const char* subtitle;
		const char* secondaryAliasName;
		const char* chainAliasName;
		SoundFile_t* soundFile;
		int sequence;
		float volMin;
		float volMax;
		float pitchMin;
		float pitchMax;
		float distMin;
		float distMax;
		int flags;
		float slavePercentage;
		float probability;
		float lfePercentage;
		float centerPercentage;
		int startDelay;
		struct SndCurve* volumeFalloffCurve;
		float envelopMin;
		float envelopMax;
		float envelopPercentage;
		SpeakerMap_t* speakerMap;
	};

	typedef struct snd_alias_list_t
	{
		const char* aliasName;
		struct snd_alias_t* head; //Can be multiple
		int count;
	} snd_alias_list_t;

	struct snd_volume_info_t
	{
		float volume;
		float goalvolume;
		float goalrate;
	};

	struct __declspec(align(4)) snd_channelvolgroup
	{
		snd_volume_info_t channelIndex[64];
		bool active;
	};

	struct snd_background_info_t
	{
		float goalvolume;
		float goalrate;
	};

	struct __declspec(align(4)) snd_local_info_t
	{
		unsigned __int8 buffer[16384];
		int size;
		bool compress;
	};

	struct __declspec(align(4)) snd_enveffect
	{
		int roomtype;
		float drylevel;
		float drygoal;
		float dryrate;
		float wetlevel;
		float wetgoal;
		float wetrate;
		bool active;
	};

	struct orientation_t
	{
		float origin[3];
		float axis[3][3];
	};

	struct __declspec(align(4)) snd_listener
	{
		orientation_t orient;
		int clientNum;
		bool active;
	};

	struct snd_amplifier
	{
		snd_listener* listener;
		int minRadius;
		int maxRadius;
		float falloffExp;
		float minVol;
		float maxVol;
	};

	struct snd_entchannel_info_t
	{
		char name[64];
		int priority;
		bool is3d;
		bool isRestricted;
		bool isPausable;
		int maxVoices;
		int voiceCount;
	};

	enum SndFileLoadingState
	{
		SFLS_UNLOADED = 0x0,
		SFLS_LOADING = 0x1,
		SFLS_LOADED = 0x2,
	};

	struct SndFileSpecificChannelInfo
	{
		SndFileLoadingState loadingState;
		int srcChannelCount;
		int baserate;
		int endtime;
	};

	union SndEntHandle
	{
		struct
		{
			unsigned int entIndex;
		}field;
		int handle;
	};

	enum SndLengthId
	{
		SndLengthNotify_Script = 0x0,
		SndLengthNotify_Subtitle = 0x1,
		SndLengthNotifyCount = 0x2,
	};

	struct sndLengthNotifyInfo
	{
		SndLengthId id[4];
		void* data[4];
		int count;
	};

	enum snd_alias_system_t
	{
		SASYS_UI = 0x0,
		SASYS_CGAME = 0x1,
		SASYS_GAME = 0x2,
		SASYS_COUNT = 0x3,
	};

	struct snd_channel_info_t
	{
		SndFileSpecificChannelInfo soundFileInfo;
		SndEntHandle sndEnt;
		int entchannel;
		int startDelay;
		int startTime;
		int looptime;
		int totalMsec;
		int playbackId;
		sndLengthNotifyInfo lengthNotifyInfo;
		float basevolume;
		float pitch;
		const snd_alias_t* alias0;
		const snd_alias_t* alias1;
		int saveIndex0;
		int saveIndex1;
		float lerp;
		float org[3];
		float offset[3];
		bool paused;
		bool master;
		bool timescale;
		snd_alias_system_t system;
	};

	struct snd_local_t
	{
		bool Initialized2d;
		bool Initialized3d;
		bool paused;
		int playbackIdCounter;
		unsigned int playback_rate;
		//int playback_bits;
		int playback_channels;
		float timescale;
		int pausetime;
		int cpu;
		snd_local_info_t restore;
		float volume;
		snd_volume_info_t mastervol;
		snd_channelvolgroup channelVolGroups[5];
		snd_channelvolgroup *channelvol;
		snd_background_info_t background[5];
		int ambient_track;
		float slaveLerp;
		snd_enveffect envEffects[3];
		snd_enveffect* effect;
		bool defaultPauseSettings[64];
		bool pauseSettings[64];
		snd_listener listeners[2];
		int time;
		int looptime;
		snd_amplifier amplifier;
		snd_entchannel_info_t entchaninfo[64];
		int entchannel_count;
		snd_channel_info_t chaninfo[53];
		int max_2D_channels;
		int max_3D_channels;
		int max_stream_channels;
	};

	typedef struct StringTable
	{
		const char* name;
		int columnCount;
		int rowCount;
		const char** values;
	} StringTable;

	struct CmdArgs
	{
		int nesting;
		int localClientNum[8];
		int controllerIndex[8];
		int argc[8];
		const char** argv[8];
	};

	struct CmdArgsPrivate
	{
		char textPool[8192];
		const char* argvPool[512];
		int usedTextPool[8];
		int totalUsedArgvPool;
		int totalUsedTextPool;
	};

	union DvarLimits
	{
		struct
		{
			int stringCount;
			const char** strings;
		} enumeration;

		struct
		{
			int min;
			int max;
		} integer;

		struct
		{
			float min;
			float max;
		} value;

		struct
		{
			float min;
			float max;
		} vector;
	};

	union DvarValue
	{
		bool enabled;
		int integer;
		unsigned int unsignedInt;
		float value;
		float vector[4];
		const char* string;
		char color[4];
	};

	enum DvarType
	{
		DVAR_TYPE_BOOL = 0x0,
		DVAR_TYPE_FLOAT = 0x1,
		DVAR_TYPE_FLOAT_2 = 0x2,
		DVAR_TYPE_FLOAT_3 = 0x3,
		DVAR_TYPE_FLOAT_4 = 0x4,
		DVAR_TYPE_INT = 0x5,
		DVAR_TYPE_ENUM = 0x6,
		DVAR_TYPE_STRING = 0x7,
		DVAR_TYPE_COLOR = 0x8,
		DVAR_TYPE_COUNT = 0x9,
	};

	enum class dvar_type : std::int8_t
	{
		boolean = 0,
		value = 1,
		vec2 = 2,
		vec3 = 3,
		vec4 = 4,
		integer = 5,
		enumeration = 6,
		string = 7,
		color = 8,
		rgb = 9 // Color without alpha
	};

	enum dvar_flags : std::uint16_t
	{
		none = 0x0,
		saved = 0x1,
		user_info = 0x2, // sent to server on connect or change
		server_info = 0x4, // sent in response to front end requests
		replicated = 0x8,
		write_protected = 0x10,
		latched = 0x20,
		read_only = 0x40,
		cheat_protected = 0x80,
		temp = 0x100,
		saved_flag = 0x1000,
		no_restart = 0x400, // do not clear when a cvar_restart is issued
		user_created = 0x4000, // created by a set command
	};

	struct dvar_s
	{
		const char* name; //0x00
		const char* description; //0x04
		dvar_flags flags; //0x08
		dvar_type type; //0x0C
		bool modified; //0xD
		DvarValue current;
		DvarValue latched;
		DvarValue reset;
		DvarLimits domain;
		bool(__cdecl* domainFunc)(dvar_s*, DvarValue);
		dvar_s* hashNext;
	};

	typedef void(__cdecl* xcommand_t)(void);

	struct cmd_function_s
	{
		cmd_function_s* next;
		const char* name;
		const char* autoCompleteDir;
		const char* autoCompleteExt;
		void(__cdecl* function)();
	};

	enum usercmdButtonBits
	{
		CMD_BUTTON_ATTACK = 1 << 0,
		CMD_BUTTON_SPRINT = 1 << 1,
		CMD_BUTTON_MELEE = 1 << 2,
		CMD_BUTTON_ACTIVATE = 1 << 3,
		CMD_BUTTON_RELOAD = 1 << 4,
		CMD_BUTTON_USE_RELOAD = 1 << 5,
		CMD_BUTTON_LEAN_LEFT = 1 << 6,
		CMD_BUTTON_LEAN_RIGHT = 1 << 7,
		CMD_BUTTON_PRONE = 1 << 8,
		CMD_BUTTON_CROUCH = 1 << 9,
		CMD_BUTTON_UP = 1 << 10,
		CMD_BUTTON_ADS = 1 << 11,
		CMD_BUTTON_DOWN = 1 << 12,
		CMD_BUTTON_BREATH = 1 << 13,
		CMD_BUTTON_FRAG = 1 << 14,
		CMD_BUTTON_OFFHAND_SECONDARY = 1 << 15,
		CMD_BUTTON_ADS_PC = 1 << 19, //This works with ADS when mode on toggle or hold
	};

	struct usercmd_s
	{
		int serverTime;
		int buttons;
		int angles[3];
		char weapon;
		char offHandIndex;
		char forwardmove; //buttonsVertical
		char rightmove; //buttonsHorizontal
		char upmove;
		char pitchmove;
		char yawmove;
		float gunPitch;
		float gunYaw;
		float gunXOfs;
		float gunYOfs;
		float gunZOfs;
		float meleeChargeYaw;
		char meleeChargeDist;
		char selectedLocation[2];
	};

	enum entity_event_t
	{
		EV_NONE = 0x0,
		EV_FOLIAGE_SOUND = 0x1,
		EV_STOP_WEAPON_SOUND = 0x2,
		EV_SOUND_ALIAS = 0x3,
		EV_SOUND_ALIAS_AS_MASTER = 0x4,
		EV_STOPSOUNDS = 0x5,
		EV_STANCE_FORCE_STAND = 0x6,
		EV_STANCE_FORCE_CROUCH = 0x7,
		EV_STANCE_FORCE_PRONE = 0x8,
		EV_ITEM_PICKUP = 0x9,
		EV_AMMO_PICKUP = 0xA,
		EV_NOAMMO = 0xB,
		EV_EMPTYCLIP = 0xC,
		EV_EMPTY_OFFHAND = 0xD,
		EV_RESET_ADS = 0xE,
		EV_RELOAD = 0xF,
		EV_RELOAD_FROM_EMPTY = 0x10,
		EV_RELOAD_START = 0x11,
		EV_RELOAD_END = 0x12,
		EV_RELOAD_START_NOTIFY = 0x13,
		EV_RELOAD_ADDAMMO = 0x14,
		EV_RAISE_WEAPON = 0x15,
		EV_FIRST_RAISE_WEAPON = 0x16,
		EV_PUTAWAY_WEAPON = 0x17,
		EV_WEAPON_ALT = 0x18,
		EV_PULLBACK_WEAPON = 0x19,
		EV_FIRE_WEAPON = 0x1A,
		EV_FIRE_WEAPON_LASTSHOT = 0x1B,
		EV_RECHAMBER_WEAPON = 0x1C,
		EV_EJECT_BRASS = 0x1D,
		EV_MELEE_SWIPE = 0x1E,
		EV_FIRE_MELEE = 0x1F,
		EV_PREP_OFFHAND = 0x20,
		EV_USE_OFFHAND = 0x21,
		EV_SWITCH_OFFHAND = 0x22,
		EV_MELEE_HIT = 0x23,
		EV_MELEE_MISS = 0x24,
		EV_MELEE_BLOOD = 0x25,
		EV_FIRE_WEAPON_MG42 = 0x26,
		EV_FIRE_QUADBARREL_1 = 0x27,
		EV_FIRE_QUADBARREL_2 = 0x28,
		EV_BULLET_TRACER = 0x29,
		EV_SOUND_ALIAS_NOTIFY = 0x2A,
		EV_SOUND_ALIAS_NOTIFY_AS_MASTER = 0x2B,
		EV_SOUND_ALIAS_ADD_NOTIFY = 0x2C,
		EV_BULLET_HIT = 0x2D,
		EV_BULLET_HIT_CLIENT_SMALL = 0x2E,
		EV_BULLET_HIT_CLIENT_LARGE = 0x2F,
		EV_GRENADE_BOUNCE = 0x30,
		EV_GRENADE_EXPLODE = 0x31,
		EV_ROCKET_EXPLODE = 0x32,
		EV_ROCKET_EXPLODE_NOMARKS = 0x33,
		EV_FLASHBANG_EXPLODE = 0x34,
		EV_CUSTOM_EXPLODE = 0x35,
		EV_CUSTOM_EXPLODE_NOMARKS = 0x36,
		EV_CHANGE_TO_DUD = 0x37,
		EV_DUD_EXPLODE = 0x38,
		EV_DUD_IMPACT = 0x39,
		EV_BULLET = 0x3A,
		EV_PLAY_FX = 0x3B,
		EV_PLAY_FX_ON_TAG = 0x3C,
		EV_PHYS_EXPLOSION_SPHERE = 0x3D,
		EV_PHYS_EXPLOSION_CYLINDER = 0x3E,
		EV_PHYS_EXPLOSION_JOLT = 0x3F,
		EV_PHYS_JITTER = 0x40,
		EV_EARTHQUAKE = 0x41,
		EV_GRENADE_SUICIDE = 0x42,
		EV_DETONATE = 0x43,
		EV_NIGHTVISION_WEAR = 0x44,
		EV_NIGHTVISION_REMOVE = 0x45,
		EV_PLAY_RUMBLE_ON_ENT = 0x46,
		EV_PLAY_RUMBLE_ON_POS = 0x47,
		EV_PLAY_RUMBLELOOP_ON_ENT = 0x48,
		EV_PLAY_RUMBLELOOP_ON_POS = 0x49,
		EV_STOP_RUMBLE = 0x4A,
		EV_STOP_ALL_RUMBLES = 0x4B,
		EV_NO_FRAG_GRENADE_HINT = 0x4C,
		EV_NO_SPECIAL_GRENADE_HINT = 0x4D,
		EV_TARGET_TOO_CLOSE_HINT = 0x4E,
		EV_TARGET_NOT_ENOUGH_CLEARANCE = 0x4F,
		EV_LOCKON_REQUIRED_HINT = 0x50,
		EV_FOOTSTEP_SPRINT = 0x51,
		EV_FOOTSTEP_RUN = 0x52,
		EV_FOOTSTEP_WALK = 0x53,
		EV_FOOTSTEP_PRONE = 0x54,
		EV_JUMP = 0x55,
		EV_LANDING_FIRST = 0x56,
		EV_LANDING_LAST = 0x72,
		EV_LANDING_PAIN_FIRST = 0x73,
		EV_LANDING_PAIN_LAST = 0x8F,
		EV_MAX_EVENTS = 0x90,
	};

	enum XZONE_FLAGS
	{
		XZONE_ZERO = 0x0,

		XZONE_LOC_POST_GFX = 0x0,
		XZONE_LOC_POST_GFX_FREE = 0x0,

		XZONE_LOC_COMMON = 0x1,
		XZONE_LOC_COMMON_FREE = 0x0,

		XZONE_POST_GFX = 0x2,
		XZONE_POST_GFX_FREE = 0x0,

		XZONE_COMMON = 0x4,
		XZONE_COMMON_FREE = 0x0,

		XZONE_UI = 0x8,
		XZONE_UI_FREE = 0x0,

		XZONE_MAP = 0x8,
		XZONE_MAP_FREE = 0x8,

		XZONE_MOD = 0x10,
		XZONE_MOD_FREE = 0x0,

		XZONE_DEBUG = 0x40,
		XZONE_DEBUG_FREE = 0x40,

		XZONE_LOAD = 0x20,
		XZONE_LOAD_FREE = 0x60,

		XZONE_UI_FREE_INGAME = XZONE_LOAD_FREE | XZONE_UI //(0x68)
	};

	struct XZoneInfo
	{
		const char* name;
		int allocFlags;
		int freeFlags;
	};

	enum FF_DIR
	{
		FFD_DEFAULT = 0x0,
		FFD_MOD_DIR = 0x1,
		FFD_USER_MAP = 0x2,
	};

	struct XZoneName
	{
		char name[64];
		int flags;
		int fileSize;
		FF_DIR dir;
		bool loaded;
	};

	// material stuff
	enum MapType
	{
		MAPTYPE_NONE = 0x0,
		MAPTYPE_INVALID1 = 0x1,
		MAPTYPE_INVALID2 = 0x2,
		MAPTYPE_2D = 0x3,
		MAPTYPE_3D = 0x4,
		MAPTYPE_CUBE = 0x5,
		MAPTYPE_COUNT = 0x6,
	};

	struct Picmip
	{
		char platform[2];
	};

	struct CardMemory
	{
		int platform[2];
	};

	struct GfxImageLoadDef
	{
		char levelCount;
		char flags;
		__int16 dimensions[3];
		int format;
		int resourceSize;
		char data[1];
	};

	union GfxTexture
	{
		IDirect3DBaseTexture9* basemap;
		IDirect3DTexture9* map;
		IDirect3DVolumeTexture9* volmap;
		IDirect3DCubeTexture9* cubemap;
		GfxImageLoadDef* loadDef;
		const char* data;
	};

	struct GfxImage
	{
		MapType mapType;
		GfxTexture texture;
		Picmip picmip;
		bool noPicmip;
		char semantic;
		char track;
		CardMemory cardMemory;
		unsigned __int16 width;
		unsigned __int16 height;
		unsigned __int16 depth;
		char category;
		bool delayLoadPixels;
		const char* name;
	};

	union MaterialTextureDefInfo
	{
		GfxImage* image;	// MaterialTextureDef->semantic != TS_WATER_MAP
		int water;
		//water_t* water;		// MaterialTextureDef->semantic == TS_WATER_MAP
	};

	struct MaterialTextureDef
	{
		unsigned int nameHash;
		char nameStart;
		char nameEnd;
		char samplerState;
		char semantic;
		MaterialTextureDefInfo u;
	};

	struct VertexDecl
	{
		const char* name;
		int unknown;
		char pad[28];
		IDirect3DVertexDeclaration9* declarations[16];
	};

	struct PixelShader
	{
		const char* name;
		IDirect3DPixelShader9* shader;
		DWORD* bytecode;
		int flags;
	};

	struct VertexShader
	{
		const char* name;
		IDirect3DVertexShader9* shader;
		DWORD* bytecode;
		int flags;
	};

	struct MaterialArgumentCodeConst
	{
		unsigned __int16 index;
		char firstRow;
		char rowCount;
	};

	union MaterialArgumentDef
	{
		const float* literalConst;
		MaterialArgumentCodeConst codeConst;
		unsigned int codeSampler;
		unsigned int nameHash;
	};

	struct MaterialShaderArgument
	{
		unsigned __int16 type;
		unsigned __int16 dest;
		MaterialArgumentDef u;
	};

	struct MaterialPass
	{
		VertexDecl* vertexDecl;
		VertexShader* vertexShader;
		PixelShader* pixelShader;
		char perPrimArgCount;
		char perObjArgCount;
		char stableArgCount;
		char customSamplerFlags;
		MaterialShaderArgument* args;
	};

	struct MaterialTechnique
	{
		const char* name;
		unsigned __int16 flags;
		unsigned __int16 passCount;
		MaterialPass passArray[1];	// count = passCount
	};

	struct GfxDrawSurfFields
	{
		unsigned __int64 objectId : 16;
		unsigned __int64 reflectionProbeIndex : 8;
		unsigned __int64 customIndex : 5;
		unsigned __int64 materialSortedIndex : 11;
		unsigned __int64 prepass : 2;
		unsigned __int64 primaryLightIndex : 8;
		unsigned __int64 surfType : 4;
		unsigned __int64 primarySortKey : 6;
		unsigned __int64 unused : 4;
	};

	union GfxDrawSurf
	{
		GfxDrawSurfFields fields;
		unsigned long long packed;
	};


#pragma pack(push, 4)
	struct MaterialInfo
	{
		const char* name;
		char gameFlags;
		char sortKey;
		char textureAtlasRowCount;
		char textureAtlasColumnCount;
		GfxDrawSurf drawSurf;
		unsigned int surfaceTypeBits;
		unsigned __int16 hashIndex;
	};
#pragma pack(pop)

	enum MaterialWorldVertexFormat : char
	{
		MTL_WORLDVERT_TEX_1_NRM_1 = 0x0,
		MTL_WORLDVERT_TEX_2_NRM_1 = 0x1,
		MTL_WORLDVERT_TEX_2_NRM_2 = 0x2,
		MTL_WORLDVERT_TEX_3_NRM_1 = 0x3,
		MTL_WORLDVERT_TEX_3_NRM_2 = 0x4,
		MTL_WORLDVERT_TEX_3_NRM_3 = 0x5,
		MTL_WORLDVERT_TEX_4_NRM_1 = 0x6,
		MTL_WORLDVERT_TEX_4_NRM_2 = 0x7,
		MTL_WORLDVERT_TEX_4_NRM_3 = 0x8,
		MTL_WORLDVERT_TEX_5_NRM_1 = 0x9,
		MTL_WORLDVERT_TEX_5_NRM_2 = 0xA,
		MTL_WORLDVERT_TEX_5_NRM_3 = 0xB,
	};

	struct MaterialTechniqueSet
	{
		char* name;
		MaterialWorldVertexFormat worldVertFormat;
		bool hasBeenUploaded;
		char unused[1];
		MaterialTechniqueSet* remappedTechniqueSet;
		MaterialTechnique* techniques[34];
	};

	struct MaterialConstantDef
	{
		int nameHash;
		char name[12];
		vec4_t literal;
	};

	struct GfxStateBits
	{
		unsigned int loadBits[2];
	};


	struct Material
	{
		MaterialInfo info;
		char stateBitsEntry[34];
		char textureCount;
		char constantCount;
		char stateBitsCount;
		char stateFlags;
		char cameraRegion;
		MaterialTechniqueSet* techniqueSet;
		MaterialTextureDef* textureTable;
		MaterialConstantDef* constantTable;
		GfxStateBits* stateBitsTable;
	};

	struct Glyph
	{
		uint16_t letter;
		char x0;
		char y0;
		char dx;
		char pixelWidth;
		char pixelHeight;
		float s0;
		float t0;
		float s1;
		float t1;
	};

	struct Font_s
	{
		const char* fontName;
		int pixelHeight;
		int glyphCount;
		Material* material;
		Material* glowMaterial;
		Glyph* glyphs;
	};

	//stock IW3
	//struct ScreenPlacement
	//{
	//	float scaleVirtualToReal[2];
	//	float scaleVirtualToFull[2];
	//	float scaleRealToVirtual[2];
	//	float virtualViewableMin[2];
	//	float virtualViewableMax[2];
	//	float realViewportSize[2];
	//	float realViewableMin[2];
	//	float realViewableMax[2];
	//	float subScreenLeft;
	//};

	//IW3 but with IW4 elements
	struct ScreenPlacement
	{
		float scaleVirtualToReal[2];
		float scaleVirtualToFull[2];
		float scaleRealToVirtual[2];
		float virtualViewableMin[2];
		float virtualViewableMax[2];
		float realViewportSize[2];
		float realViewableMin[2];
		float realViewableMax[2];
		float subScreenLeft;
		float realViewportPosition[2];
		float virtualAdjustableMin[2];
		float virtualAdjustableMax[2];
		float realAdjustableMin[2];
		float realAdjustableMax[2];
	};


	enum EMemTrack
	{
		TRACK_DEBUG = 0x0,
		TRACK_HUNK = 0x1,
		TRACK_BINARIES = 0x2,
		TRACK_MISC_SWAP = 0x3,
		TRACK_DELIMITER1 = 0x4,
		TRACK_AI = 0x5,
		TRACK_AI_NODES = 0x6,
		TRACK_SCRIPT = 0x7,
		TRACK_FX = 0x8,
		TRACK_GLASS = 0x9,
		TRACK_NETWORK_ENTITY = 0xA,
		TRACK_MISC = 0xB,
		TRACK_FASTFILE = 0xC,
		TRACK_ANIMATION = 0xD,
		TRACK_WORLD_GLOBALS = 0xE,
		TRACK_SOUND_GLOBALS = 0xF,
		TRACK_CLIENT_ANIMSCRIPT = 0x10,
		TRACK_SOUND = 0x11,
		TRACK_DELIMITER2 = 0x12,
		TRACK_RENDERER_GLOBALS = 0x13,
		TRACK_RENDERER_IMAGES = 0x14,
		TRACK_RENDERER_WORLD = 0x15,
		TRACK_RENDERER_MODELS = 0x16,
		TRACK_RENDERER_MISC = 0x17,
		TRACK_CINEMATICS = 0x18,
		TRACK_DELIMITER3 = 0x19,
		TRACK_COLLISION_MISC = 0x1A,
		TRACK_COLLISION_BRUSH = 0x1B,
		TRACK_COLLISION_MODEL_TRI = 0x1C,
		TRACK_COLLISION_TERRAIN = 0x1D,
		TRACK_PHYSICS = 0x1E,
		TRACK_MAP_ENTS = 0x1F,
		TRACK_TEMP = 0x20,
		TRACK_DELIMITER4 = 0x21,
		TRACK_LOCALIZATION = 0x22,
		TRACK_FLAME = 0x23,
		TRACK_UI = 0x24,
		TRACK_TL = 0x25,
		TRACK_ZMEM = 0x26,
		TRACK_FIREMANAGER = 0x27,
		TRACK_PROFILE = 0x28,
		TRACK_WATERSIM = 0x29,
		TRACK_CLIENT = 0x2A,
		TRACK_RECORDER = 0x2B,
		TRACK_RSTREAM = 0x2C,
		TRACK_RENDERER_STREAMBUFFER = 0x2D,
		TRACK_RENDERER_STREAMBUFFER_EXTRA = 0x2E,
		TRACK_GEOSTREAM = 0x2F,
		TRACK_DDL = 0x30,
		TRACK_ONLINE = 0x31,
		TRACK_EMBLEM = 0x32,
		TRACK_MINSPEC_IMAGES = 0x33,
		TRACK_DELIMITER5 = 0x34,
		TRACK_NONE = 0x35,
		TRACK_COUNT = 0x36,
	};

	//new
	union PackedUnitVec
	{
		unsigned int packed;
		char array[4];
	};

	struct WeaponDef_s;

	struct RawFile
	{
		const char* name;
		int len;
		const char* buffer;
	};

	struct LocalizeEntry
	{
		const char* value;
		const char* name;
	};

	//New 05.10.2022
	typedef enum ItemFontType_t {
		UI_FONT_DEFAULT = 0, /* Auto-choose betwen big/reg/small */
		UI_FONT_NORMAL,
		UI_FONT_BIG,
		UI_FONT_SMALL,
		UI_FONT_BOLD,
		UI_FONT_CONSOLE,
		UI_FONT_OBJECTIVE,
		UI_FONT_HUDBIGFONT
	} ItemFontType_t;

	typedef enum ItemDefType_t {
		ITEM_TYPE_TEXT = 0,            /* Simple text. */
		ITEM_TYPE_BUTTON,              /* Button, basically text with a border. */
		ITEM_TYPE_RADIOBUTTON,         /* Toggle button, may be grouped. */
		ITEM_TYPE_CHECKBOX,            /* Check box. */
		ITEM_TYPE_EDITFIELD,           /* Editable text, associated with a dvar. */
		ITEM_TYPE_COMBO,               /* Drop down list. */
		ITEM_TYPE_LISTBOX,             /* Scrollable list. */
		ITEM_TYPE_MODEL,               /* Model. */
		ITEM_TYPE_OWNERDRAW,           /* Owner draw, name specs what it is. */
		ITEM_TYPE_NUMERICFIELD,        /* Editable text, associated with a dvar. */
		ITEM_TYPE_SLIDER,              /* Mouse speed, volume, etc.. */
		ITEM_TYPE_YESNO,               /* Yes no dvar setting. */
		ITEM_TYPE_MULTI,               /* Multiple list setting, enumerated. */
		ITEM_TYPE_DVARENUM,            /* Multiple list setting, enumerated from a dvar. */
		ITEM_TYPE_BIND,                /* Bind. */
		ITEM_TYPE_MENUMODEL,           /* Special menu model. */
		ITEM_TYPE_VALIDFILEFIELD,      /* Text must be valid for use in a dos filename. */
		ITEM_TYPE_DECIMALFIELD,        /* Editable text, associated with a dvar, which allows decimal input. */
		ITEM_TYPE_UPREDITFIELD,        /* Editable text, associated with a dvar. */
		ITEM_TYPE_GAME_MESSAGE_WINDOW  /* Game message window. */
	} ItemDefType_t;

	struct rectDef_s
	{
		float x;
		float y;
		float w;
		float h;
		int horzAlign;
		int vertAlign;
	};

	struct windowDef_t
	{
		const char* name;
		rectDef_s rect;
		rectDef_s rectClient;
		const char* group;
		int style;
		int border;
		int ownerDraw;
		int ownerDrawFlags;
		float borderSize;
		int staticFlags;
		int dynamicFlags[1];
		int nextTime;
		float foreColor[4];
		float backColor[4];
		float borderColor[4];
		float outlineColor[4];
		Material* background;
	};

	union operandInternalDataUnion
	{
		int intVal;
		float floatVal;
		const char* string;
	};

	enum expDataType
	{
		VAL_INT = 0x0,
		VAL_FLOAT = 0x1,
		VAL_STRING = 0x2,
	};

	struct Operand
	{
		expDataType dataType;
		operandInternalDataUnion internals;
	};

	enum operationEnum
	{
		OP_NOOP = 0,
		OP_RIGHTPAREN,
		OP_MULTIPLY,
		OP_DIVIDE,
		OP_MODULUS,
		OP_ADD,
		OP_SUBTRACT,
		OP_NOT,
		OP_LESSTHAN,
		OP_LESSTHANEQUALTO,
		OP_GREATERTHAN,
		OP_GREATERTHANEQUALTO,
		OP_EQUALS,
		OP_NOTEQUAL,
		OP_AND,
		OP_OR,
		OP_LEFTPAREN,
		OP_COMMA,
		OP_BITWISEAND,
		OP_BITWISEOR,
		OP_BITWISENOT,
		OP_BITSHIFTLEFT,
		OP_BITSHIFTRIGHT,
		OP_SIN,
		OP_COS,
		OP_MIN,
		OP_MAX,
		OP_MILLISECONDS,
		OP_DVARINT,
		OP_DVARBOOL,
		OP_DVARFLOAT,
		OP_DVARSTRING,
		OP_STAT,
		OP_UI_ACTIVE,
		OP_FLASHBANGED,
		OP_SCOPED,
		OP_SCOREBOARD_VISIBLE,
		OP_INKILLCAM,
		OP_PLAYER,
		OP_SELECTING_LOCATION,
		OP_TEAM,
		OP_OTHERTEAM,
		OP_MARINES,
		OP_OPFOR,
		OP_MENUISOPEN,
		OP_WRITINGDATA,
		OP_INLOBBY,
		OP_INPRIVATEPARTY,
		OP_PRIVATEPARTYHOST,
		OP_PRIVATEPARTYHOSTINLOBBY,
		OP_ALONEINPARTY,
		OP_ADSJAVELIN,
		OP_WEAPLOCKBLINK,
		OP_WEAPATTACKTOP,
		OP_WEAPATTACKDIRECT,
		OP_SECONDSASTIME,
		OP_TABLELOOKUP,
		OP_LOCSTRING,
		OP_LOCALVARINT,
		OP_LOCALVARBOOL,
		OP_LOCALVARFLOAT,
		OP_LOCALVARSTRING,
		OP_TIMELEFT,
		OP_SECONDSASCOUNTDOWN,
		OP_GAMEMSGWNDACTIVE,
		OP_INT,
		OP_STRING,
		OP_FLOAT,
		OP_GAMETYPENAME,
		OP_GAMETYPE,
		OP_GAMETYPEDESCRIPTION,
		OP_SCOREATRANK,
		OP_FRIENDSONLINE,
		OP_SPECTATINGCLIENT,
		OP_STATRANGEANYBITSSET,
		OP_KEYBINDING,
		OP_ACTIONSLOTUSABLE,
		OP_HUDFADE,
		OP_MAXRECOMMENDEDPLAYERS,
		OP_ACCEPTINGINVITE,
		OP_ISINTERMISSION,
		OP_JERRYTESTOP,
		NUM_OPERATIONS
	};

	union entryInternalData
	{
		operationEnum op;
		Operand operand;
	};

	struct expressionEntry
	{
		int type;
		entryInternalData data;
	};

	struct OperandList
	{
		Operand operands[10];
		int operandCount;
	};

	struct OperatorStack
	{
		operationEnum stack[60];
		int numOperators;
	};

	struct OperandStack
	{
		OperandList stack[60];
		int numOperandLists;
	};

	struct statement_s
	{
		int numEntries;
		expressionEntry** entries;
	};

	struct ItemKeyHandler
	{
		int key;
		const char* action;
		ItemKeyHandler* next;
	};

	struct columnInfo_s
	{
		int pos;
		int width;
		int maxChars;
		int alignment;
	};

	struct listBoxDef_s
	{
		int mousePos;
		int startPos[1];
		int endPos[1];
		int drawPadding;
		float elementWidth;
		float elementHeight;
		int elementStyle;
		int numColumns;
		columnInfo_s columnInfo[16];
		const char* doubleClick;
		int notselectable;
		int noScrollBars;
		int usePaging;
		float selectBorder[4];
		float disableColor[4];
		Material* selectIcon;
	};

	struct editFieldDef_s
	{
		float minVal;
		float maxVal;
		float defVal;
		float range;
		int maxChars;
		int maxCharsGotoNext;
		int maxPaintChars;
		int paintOffset;
	};

	struct multiDef_s
	{
		const char* dvarList[32];
		const char* dvarStr[32];
		float dvarValue[32];
		int count;
		int strDef;
	};

	union itemDefData_t
	{
		listBoxDef_s* listBox;
		editFieldDef_s* editField;
		multiDef_s* multi;
		const char* enumDvarName;
		void* data;
	};

	struct itemDef_s;

	struct menuDef_t
	{
		windowDef_t window;
		const char* font;
		int fullScreen;
		int itemCount;
		int fontIndex;
		int cursorItem[1];
		int fadeCycle;
		float fadeClamp;
		float fadeAmount;
		float fadeInAmount;
		float blurRadius;
		const char* onOpen;
		const char* onClose;
		const char* onESC;
		ItemKeyHandler* onKey;
		statement_s visibleExp;
		const char* allowedBinding;
		const char* soundName; //soundloop
		int imageTrack;
		float focusColor[4];
		float disableColor[4];
		statement_s rectXExp;
		statement_s rectYExp;
		itemDef_s** items;
	};

	struct itemDef_s
	{
		windowDef_t window;
		rectDef_s textRect[1];
		ItemDefType_t type;
		ItemDefType_t dataType;
		int alignment;
		ItemFontType_t textFont;
		int textAlignMode;
		float textalignx;
		float textaligny;
		float textscale;
		int textStyle;
		int gameMsgWindowIndex;
		int gameMsgWindowMode;
		const char* text;
		int textSavegame; //original name: itemFlags
		menuDef_t* parent;
		const char* mouseEnterText;
		const char* mouseExitText;
		const char* mouseEnter;
		const char* mouseExit;
		const char* action;
		const char* onAccept;
		const char* onFocus;
		const char* leaveFocus;
		const char* dvar;
		const char* dvarTest;
		ItemKeyHandler* onKey;
		const char* enableDvar;
		int dvarFlags;
		snd_alias_list_t* focusSound;
		int feeder; //float feeder; //original name: special
		int cursorPos[1];
		itemDefData_t typeData;
		int imageTrack;
		statement_s visibleExp;
		statement_s textExp;
		statement_s materialExp;
		statement_s rectXExp;
		statement_s rectYExp;
		statement_s rectWExp;
		statement_s rectHExp;
		statement_s forecolorAExp;
	};

	struct XModel;

	union XAssetHeader
	{
		void* data;
		XModel* model;
		LocalizeEntry* localize;
		Font_s* font;
		snd_alias_list_t* sound;
		WeaponDef_s* weapon;
		RawFile* rawfile;
		Material* material;
		StringTable* stringTable;
		GfxImage* image;
		menuDef_t* menu;
	};

	struct XAsset
	{
		XAssetType type;
		XAssetHeader header;
	};

	struct XAssetEntry
	{
		XAsset asset;		// 0
		char zoneIndex;		// 8
		bool inuse;			// 9
		unsigned __int16 nextHash;	// 10
		unsigned __int16 nextOverride;
		unsigned __int16 usageFrame;
	};

	typedef struct
	{
		scr_string_t _;
		scr_string_t active;
		scr_string_t aim_bone;
		scr_string_t aim_highest_bone;
		scr_string_t aim_vis_bone;
		scr_string_t all;
		scr_string_t allies;
		scr_string_t axis;
		scr_string_t bad_path;
		scr_string_t begin_firing;
		scr_string_t cancel_location;
		scr_string_t confirm_location;
		scr_string_t crouch;
		scr_string_t current;
		scr_string_t damage;
		scr_string_t dead;
		scr_string_t death;
		scr_string_t detonate;
		scr_string_t direct;
		scr_string_t dlight;
		scr_string_t done;
		scr_string_t empty;
		scr_string_t end_firing;
		scr_string_t entity;
		scr_string_t explode;
		scr_string_t failed;
		scr_string_t free;
		scr_string_t fraction;
		scr_string_t goal;
		scr_string_t goal_changed;
		scr_string_t goal_yaw;
		scr_string_t grenade;
		scr_string_t grenadedanger;
		scr_string_t grenade_fire;
		scr_string_t grenade_pullback;
		scr_string_t info_notnull;
		scr_string_t invisible;
		scr_string_t key1;
		scr_string_t key2;
		scr_string_t killanimscript;
		scr_string_t left;
		scr_string_t light;
		scr_string_t movedone;
		scr_string_t noclass;
		scr_string_t none;
		scr_string_t normal;
		scr_string_t player;
		scr_string_t position;
		scr_string_t projectile_impact;
		scr_string_t prone;
		scr_string_t right;
		scr_string_t reload;
		scr_string_t reload_start;
		scr_string_t rocket;
		scr_string_t rotatedone;
		scr_string_t script_brushmodel;
		scr_string_t script_model;
		scr_string_t script_origin;
		scr_string_t snd_enveffectsprio_level;
		scr_string_t snd_enveffectsprio_shellshock;
		scr_string_t snd_channelvolprio_holdbreath;
		scr_string_t snd_channelvolprio_pain;
		scr_string_t snd_channelvolprio_shellshock;
		scr_string_t stand;
		scr_string_t suppression;
		scr_string_t suppression_end;
		scr_string_t surfacetype;
		scr_string_t tag_aim;
		scr_string_t tag_aim_animated;
		scr_string_t tag_brass;
		scr_string_t tag_butt;
		scr_string_t tag_clip;
		scr_string_t tag_flash;
		scr_string_t tag_flash_11;
		scr_string_t tag_flash_2;
		scr_string_t tag_flash_22;
		scr_string_t tag_flash_3;
		scr_string_t tag_fx;
		scr_string_t tag_inhand;
		scr_string_t tag_knife_attach;
		scr_string_t tag_knife_fx;
		scr_string_t tag_laser;
		scr_string_t tag_origin;
		scr_string_t tag_weapon;
		scr_string_t tag_player;
		scr_string_t tag_camera;
		scr_string_t tag_weapon_right;
		scr_string_t tag_gasmask;
		scr_string_t tag_gasmask2;
		scr_string_t tag_sync;
		scr_string_t target_script_trigger;
		scr_string_t tempEntity;
		scr_string_t top;
		scr_string_t touch;
		scr_string_t trigger;
		scr_string_t trigger_use;
		scr_string_t trigger_use_touch;
		scr_string_t trigger_damage;
		scr_string_t trigger_lookat;
		scr_string_t truck_cam;
		scr_string_t weapon_change;
		scr_string_t weapon_fired;
		scr_string_t worldspawn;
		scr_string_t flashbang;
		scr_string_t flash;
		scr_string_t smoke;
		scr_string_t night_vision_on;
		scr_string_t night_vision_off;
		scr_string_t mod_unknown;
		scr_string_t mod_pistol_bullet;
		scr_string_t mod_rifle_bullet;
		scr_string_t mod_grenade;
		scr_string_t mod_grenade_splash;
		scr_string_t mod_projectile;
		scr_string_t mod_projectile_splash;
		scr_string_t mod_melee;
		scr_string_t mod_head_shot;
		scr_string_t mod_crush;
		scr_string_t mod_telefrag;
		scr_string_t mod_falling;
		scr_string_t mod_suicide;
		scr_string_t mod_trigger_hurt;
		scr_string_t mod_explosive;
		scr_string_t mod_impact;
		scr_string_t script_vehicle;
		scr_string_t script_vehicle_collision;
		scr_string_t script_vehicle_collmap;
		scr_string_t script_vehicle_corpse;
		scr_string_t turret_fire;
		scr_string_t turret_on_target;
		scr_string_t turret_not_on_target;
		scr_string_t turret_on_vistarget;
		scr_string_t turret_no_vis;
		scr_string_t turret_rotate_stopped;
		scr_string_t turret_deactivate;
		scr_string_t turretstatechange;
		scr_string_t turretownerchange;
		scr_string_t reached_end_node;
		scr_string_t reached_wait_node;
		scr_string_t reached_wait_speed;
		scr_string_t near_goal;
		scr_string_t veh_collision;
		scr_string_t veh_predictedcollision;
		scr_string_t _custom;
		scr_string_t always;
		scr_string_t auto_ai;
		scr_string_t auto_nonai;
		scr_string_t angle_deltas;
		scr_string_t back_left;
		scr_string_t back_right;
		scr_string_t begin_custom_anim;
		scr_string_t bullethit;
		scr_string_t bulletwhizby;
		scr_string_t count;
		scr_string_t corner_approach;
		scr_string_t damage_notdone;
		scr_string_t deathplant;
		scr_string_t enemy;
		scr_string_t enemy_visible;
		scr_string_t face_angle;
		scr_string_t face_current;
		scr_string_t face_default;
		scr_string_t face_direction;
		scr_string_t face_enemy;
		scr_string_t face_enemy_or_motion;
		scr_string_t face_goal;
		scr_string_t face_motion;
		scr_string_t face_point;
		scr_string_t front_left;
		scr_string_t front_right;
		scr_string_t gravity;
		scr_string_t grenade_return_hand_tag;
		scr_string_t groundEntChanged;
		scr_string_t gunshot;
		scr_string_t high_priority;
		scr_string_t info_player_deathmatch;
		scr_string_t infinite_energy;
		scr_string_t low_priority;
		scr_string_t obstacle;
		scr_string_t manual;
		scr_string_t manual_ai;
		scr_string_t max_time;
		scr_string_t menuresponse;
		scr_string_t middle_left;
		scr_string_t middle_right;
		scr_string_t min_energy;
		scr_string_t min_time;
		scr_string_t movemode;
		scr_string_t neutral;
		scr_string_t never;
		scr_string_t noclip;
		scr_string_t node_out_of_range;
		scr_string_t node_relinquished;
		scr_string_t node_taken;
		scr_string_t node_not_safe;
		scr_string_t nogravity;
		scr_string_t nophysics;
		scr_string_t pain;
		scr_string_t pickup;
		scr_string_t receiver;
		scr_string_t run;
		scr_string_t runto_arrived;
		scr_string_t silenced_shot;
		scr_string_t sound_blend;
		scr_string_t spawned;
		scr_string_t start_move;
		scr_string_t stop;
		scr_string_t stop_soon;
		scr_string_t tag_eye;
		scr_string_t tag_wheel_front_left;
		scr_string_t tag_wheel_front_right;
		scr_string_t tag_wheel_back_left;
		scr_string_t tag_wheel_back_right;
		scr_string_t tag_wheel_middle_left;
		scr_string_t tag_wheel_middle_right;
		scr_string_t tag_detach;
		scr_string_t tag_popout;
		scr_string_t tag_body;
		scr_string_t tag_turret;
		scr_string_t tag_turret_base;
		scr_string_t tag_barrel;
		scr_string_t tag_weapon_left;
		scr_string_t walk;
		scr_string_t world;
		scr_string_t zonly_physics;
		scr_string_t human;
		scr_string_t dog;
	}scr_const_t;

	struct LerpEntityStateTurret
	{
		float gunAngles[3];
	};

	struct LerpEntityStateLoopFx
	{
		float cullDist;
		int period;
	};

	struct __declspec(align(4)) LerpEntityStateActor
	{
		float visionFov;
		float visionDist;
		unsigned __int8 species;
	};

	struct LerpEntityStatePrimaryLight
	{
		unsigned __int8 colorAndExp[4];
		float intensity;
		float radius;
		float cosHalfFovOuter;
		float cosHalfFovInner;
	};

	struct LerpEntityStatePlayer
	{
		float leanf;
	};

	struct LerpEntityStateVehicle
	{
		float bodyPitch;
		float bodyRoll;
		float steerYaw;
		int materialTime;
		float gunPitch;
		float gunYaw;
	};

	struct LerpEntityStateMissile
	{
		int launchTime;
	};

	struct LerpEntityStateSoundBlend
	{
		float lerp;
		float volumeScale;
	};

	struct LerpEntityStateBulletTracer
	{
		float end[3];
	};

	struct LerpEntityStateBulletHit
	{
		float start[3];
	};

	struct LerpEntityStateEarthquake
	{
		float scale;
		float radius;
		int duration;
	};

	struct LerpEntityStateCustomExplode
	{
		int startTime;
	};

	struct LerpEntityStateExplosion
	{
		float innerRadius;
		float magnitude;
	};

	struct LerpEntityStateExplosionJolt
	{
		float innerRadius;
		float impulse[3];
	};

	struct LerpEntityStatePhysicsJitter
	{
		float innerRadius;
		float minDisplacement;
		float maxDisplacement;
	};

	union LerpEntityStateTypeUnion
	{
		LerpEntityStateTurret turret;
		LerpEntityStateLoopFx loopFx;
		LerpEntityStateActor actor;
		LerpEntityStatePrimaryLight primaryLight;
		LerpEntityStatePlayer player;
		LerpEntityStateVehicle vehicle;
		LerpEntityStateMissile missile;
		LerpEntityStateSoundBlend soundBlend;
		LerpEntityStateBulletTracer bulletTracer;
		LerpEntityStateBulletHit bulletHit;
		LerpEntityStateEarthquake earthquake;
		LerpEntityStateCustomExplode customExplode;
		LerpEntityStateExplosion explosion;
		LerpEntityStateExplosionJolt explosionJolt;
		LerpEntityStatePhysicsJitter physicsJitter;
	};

	enum trType_t
	{
		TR_STATIONARY = 0x0,
		TR_INTERPOLATE = 0x1,
		TR_LINEAR = 0x2,
		TR_LINEAR_STOP = 0x3,
		TR_SINE = 0x4,
		TR_GRAVITY = 0x5,
		TR_ACCELERATE = 0x6,
		TR_DECELERATE = 0x7,
		TR_PHYSICS = 0x8,
		TR_FIRST_RAGDOLL = 0x9,
		TR_RAGDOLL = 0x9,
		TR_RAGDOLL_GRAVITY = 0xA,
		TR_RAGDOLL_INTERPOLATE = 0xB,
		TR_LAST_RAGDOLL = 0xB,
	};

	struct trajectory_t
	{
		trType_t trType;
		int trTime;
		int trDuration;
		float trBase[3];
		float trDelta[3];
	};

	struct LerpEntityState
	{
		int eFlags;
		trajectory_t pos;
		trajectory_t apos;
		LerpEntityStateTypeUnion u;
		int useCount;
	};

	union compassUnion
	{
		unsigned __int8 scale;
		unsigned __int8 eventParm2;
		unsigned __int8 vehicleCompassType;
	};

	struct entityState_s
	{
		char eType;
		char surfType;
		char weapon;
		char weaponModel;
		compassUnion un1;
		LerpEntityState lerp;
		unsigned int eventParm;
		unsigned __int16 loopSound;
		unsigned __int16 number;
		unsigned __int16 otherEntityNum;
		unsigned __int16 groundEntityNum;
		//char index[2];
		std::int16_t index;
		int time2;
		int solid;
		int eventSequence;
		unsigned __int8 events[4];
		unsigned int eventParms[4];
		char partBits[4];
	};

	struct EntHandle
	{
		unsigned __int16 number;
		unsigned __int16 infoIndex;
	};

	struct entityShared_t {
		char linked;
		char bmodel;
		char svFlags;
		char eventType;
		char inuse;
		float mins[3];
		float maxs[3];
		int contents;
		float absmin[3];
		float absmax[3];
		float currentOrigin[3];
		float currentAngles[3];
		EntHandle ownerNum;
		int eventTime;
	};

	struct CEntActorInfo
	{
		int proneType;
		float pitch;
		float roll;
		float height;
	};

	struct CEntTurretAngles
	{
		float pitch;
		float yaw;
		float barrelPitch;
	};

	struct CEntTurretInfo
	{
		union
		{
			CEntTurretAngles angles;
			const float* viewAngles;
		}u0;
		bool playerUsing;
		char tag_aim;
		char tag_aim_animated;
		char tag_flash;
	};

	struct __declspec(align(4)) CEntVehicleInfo
	{
		__int16 pitch;
		__int16 yaw;
		__int16 roll;
		__int16 barrelPitch;
		__int16 steerYaw;
		float time;
		unsigned __int16 wheelFraction[6];
		char wheelBoneIndex[6];
		char tag_body;
		char tag_turret;
		char tag_barrel;
	};

	struct CEntFx
	{
		int triggerTime;
		int effect; //FxEffect*
	};

	struct cpose_t
	{
		unsigned __int16 lightingHandle;
		char eType;
		char eTypeUnion;
		bool isRagdoll;
		int ragdollHandle;
		int physObjId;
		int cullIn;
		float origin[3];
		float angles[3];
		union
		{
			CEntActorInfo actor;
			CEntTurretInfo turret;
			CEntVehicleInfo vehicle;
			CEntFx fx;
		}u9;
	};

	struct centity_s
	{
		cpose_t pose;
		LerpEntityState currentState;
		entityState_s nextState;
		bool nextValid;
		bool bMuzzleFlash;
		bool bTrailMade;
		char oldEType;
		int previousEventSequence;
		float lightingOrigin[3];
	};

	typedef enum
	{
		PLAYER_OFFHAND_SECONDARY_SMOKE = 0x0,
		PLAYER_OFFHAND_SECONDARY_FLASH = 0x1,
		PLAYER_OFFHAND_SECONDARIES_TOTAL = 0x2,
	}OffhandSecondaryClass_t;

	typedef enum
	{
		PLAYERVIEWLOCK_NONE = 0x0,
		PLAYERVIEWLOCK_FULL = 0x1,
		PLAYERVIEWLOCK_WEAPONJITTER = 0x2,
		PLAYERVIEWLOCKCOUNT = 0x3,
	}ViewLockTypes_t;

	typedef enum
	{
		ACTIONSLOTTYPE_DONOTHING = 0x0,
		ACTIONSLOTTYPE_SPECIFYWEAPON = 0x1,
		ACTIONSLOTTYPE_ALTWEAPONTOGGLE = 0x2,
		ACTIONSLOTTYPE_NIGHTVISION = 0x3,
		ACTIONSLOTTYPECOUNT = 0x4,
	}ActionSlotType_t;

	typedef struct {
		int	sprintButtonUpRequired;
		int	sprintDelay;
		int	lastSprintStart;
		int	lastSprintEnd;
		int	sprintStartMaxLength;
	}sprintState_t;

	typedef struct {
		float	yaw;
		int	timer;
		int	transIndex;
		int	flags;
	}mantleState_t;

	typedef struct
	{
		unsigned int index;
	}ActionSlotParam_SpecifyWeapon_t;


	typedef struct
	{
		ActionSlotParam_SpecifyWeapon_t specifyWeapon;
	}ActionSlotParam_t;

	typedef enum
	{
		HE_TYPE_FREE = 0x0,
		HE_TYPE_TEXT = 0x1,
		HE_TYPE_VALUE = 0x2,
		HE_TYPE_MATERIAL = 0x3,
		HE_TYPE_TIMER_DOWN = 0x4,
		HE_TYPE_TIMER_UP = 0x5,
		HE_TYPE_TENTHS_TIMER_DOWN = 0x6,
		HE_TYPE_TENTHS_TIMER_UP = 0x7,
		HE_TYPE_CLOCK_DOWN = 0x8,
		HE_TYPE_CLOCK_UP = 0x9,
		HE_TYPE_WAYPOINT = 0xA,
		HE_TYPE_COUNT = 0xB,
	}he_type_t;

	typedef struct
	{
		char r;
		char g;
		char b;
		char a;
	}hudelem_colorsplit_t;

	/* 6854 */
	typedef union
	{
		hudelem_colorsplit_t split;
		int rgba;
	}hudelem_color_t;

	typedef struct hudelem_s
	{
		he_type_t type;
		float x;
		float y;
		float z;
		int targetEntNum;
		float fontScale;
		float fromFontScale;
		int fontScaleStartTime;
		int fontScaleTime;
		int font;
		int alignOrg;
		int alignScreen;
		hudelem_color_t color;
		hudelem_color_t fromColor;
		int fadeStartTime;
		int fadeTime;
		int label;
		int width;
		int height;
		int materialIndex;
		int offscreenMaterialIdx;
		int fromWidth;
		int fromHeight;
		int scaleStartTime;
		int scaleTime;
		float fromX;
		float fromY;
		int fromAlignOrg;
		int fromAlignScreen;
		int moveStartTime;
		int moveTime;
		int time;
		int duration;
		float value;
		int text;
		float sort;
		hudelem_color_t glowColor;
		int fxBirthTime;
		int fxLetterTime;
		int fxDecayStartTime;
		int fxDecayDuration;
		int soundID;
		int flags;
	}hudelem_t;

	typedef struct hudElemState_s
	{
		hudelem_t elem[256];
	}hudElemState_t;

	enum fieldtype_t
	{
		F_INT = 0x0,
		F_SHORT = 0x1,
		F_BYTE = 0x2,
		F_FLOAT = 0x3,
		F_STRING = 0x4,
		F_VECTOR = 0x5,
		F_ENTITY = 0x6,
		F_ENTHANDLE = 0x7,
		F_ACTOR = 0x8,
		F_SENTIENT = 0x9,
		F_SENTIENTHANDLE = 0xA,
		F_CLIENT = 0xB,
		F_PATHNODE = 0xC,
		F_VECTORHACK = 0xD,
		F_MODEL = 0xE,
		F_ACTORGROUP = 0xF,
	};

	struct game_hudelem_s
	{
		hudelem_s elem;
	};

	struct game_hudelem_field_t
	{
		const char* name;
		int ofs;
		fieldtype_t type;
		int mask;
		int shift;
		void(__cdecl* setter)(game_hudelem_s*, int);
		void(__cdecl* getter)(game_hudelem_s*, int);
	};

	enum
	{
		PMF_MANTLE = 1 << 2,
		PMF_FROZEN = 1 << 11,
		PMF_SPRINTING = 1 << 15,
		PMF_SHELLSHOCKED = 1 << 16,
	};

	struct playerState_s {
		int commandTime;
		int pm_type;
		int bobCycle;
		int pm_flags;
		int weapFlags;
		int otherFlags;
		int pm_time;
		vec3_t origin;
		vec3_t velocity;
		vec2_t oldVelocity;
		int weaponTime;
		int weaponDelay;
		int grenadeTimeLeft;
		int throwBackGrenadeOwner;
		int throwBackGrenadeTimeLeft;
		int weaponRestrictKickTime;
		int foliageSoundTime;
		int gravity;
		float leanf;
		int speed;
		float delta_angles[3];
		int groundEntityNum;
		vec3_t vLadderVec;
		int jumpTime;
		float jumpOriginZ;
		int movementDir;
		int eFlags;
		int eventSequence;
		int events[4];
		unsigned int eventParms[4];
		int oldEventSequence;
		int clientNum;
		int offHandIndex;
		OffhandSecondaryClass_t offhandSecondary;
		unsigned int weapon;
		int weaponstate;
		unsigned int weaponShotCount;
		float fWeaponPosFrac;
		int adsDelayTime;
		int spreadOverride;
		int spreadOverrideState;
		int viewmodelIndex;
		vec3_t viewangles;
		int viewHeightTarget;
		float viewHeightCurrent;
		int viewHeightLerpTime;
		int viewHeightLerpTarget;
		int viewHeightLerpDown;
		vec2_t viewAngleClampBase;
		vec2_t viewAngleClampRange;
		int damageEvent;
		int damageYaw;
		int damagePitch;
		int damageCount;
		int stats[4];
		int ammo[128];
		int ammoclip[128];
		unsigned int weapons[4];
		unsigned int weaponold[4];
		unsigned int weaponrechamber[4];
		float proneDirection;
		float proneDirectionPitch;
		float proneTorsoPitch;
		ViewLockTypes_t viewlocked;
		int viewlocked_entNum;
		int vehicleType;
		float linkAngles[3];
		float linkWeaponAngles[3];
		int cursorHint;
		int cursorHintString;
		int cursorHintEntIndex;
		int locationSelectionInfo;
		sprintState_t sprintState;
		float fTorsoPitch;
		float fWaistPitch;
		float holdBreathScale;
		int holdBreathTimer;
		float moveSpeedScaleMultiplier;
		mantleState_t mantleState;
		float meleeChargeYaw;
		int meleeChargeDist;
		int meleeChargeTime;
		int weapLockFlags;
		int weapLockedEntnum;
		unsigned int forcedViewAnimWeaponIdx;
		int forcedViewAnimWeaponState;
		unsigned int forcedViewAnimOriginalWeaponIdx;
		ActionSlotType_t actionSlotType[4];
		ActionSlotParam_t actionSlotParam[4];
		int entityEventSequence;
		int weapAnim;
		float aimSpreadScale;
		int shellshockIndex;
		int shellshockTime;
		int shellshockDuration;
		float dofNearStart;
		float dofNearEnd;
		float dofFarStart;
		float dofFarEnd;
		float dofNearBlur;
		float dofFarBlur;
		float dofViewmodelStart;
		float dofViewmodelEnd;
		int hudElemLastAssignedSoundID;
		unsigned __int8 weaponmodels[128];
		char hud[0xAC00];
	};

	enum clientConnected_t
	{
		CON_DISCONNECTED = 0x0,
		CON_CONNECTING = 0x1,
		CON_CONNECTED = 0x2,
	};

	struct clientPersistent_t
	{
		clientConnected_t connected;
		usercmd_s cmd;
		usercmd_s oldcmd;
		int maxHealth;
		float moveSpeedScaleMultiplier;
	};

	struct gclient_s 
	{
		playerState_s ps; //playerState_s predictedPlayerState;
		clientPersistent_t pers;
		int noclip;
		int ufo;
		int bFrozen;
		int buttons;
		int oldbuttons;
		int latched_buttons;
		int buttonsSinceLastFrame;
		float fGunPitch;
		float fGunYaw;
		float fGunXOfs;
		float fGunYOfs;
		float fGunZOfs;
		int damage_blood;
		float damage_from[3];
		int damage_fromWorld;
		int respawnTime;
		float currentAimSpreadScale;
		int pHitHitEnt;
		EntHandle pLookatEnt;
		float prevLinkedInvQuat[4];
		bool prevLinkAnglesSet;
		bool link_rotationMovesEyePos;
		bool link_doCollision;
		bool link_useTagAnglesForViewAngles;
		bool linkAnglesLocked;
		float linkAnglesFrac;
		float linkAnglesMinClamp[2];
		float linkAnglesMaxClamp[2];
		int inControlTime;
		int lastTouchTime;
		EntHandle useHoldEntity;
		int useHoldTime;
		int useButtonDone;
		int bDisableAutoPickup;
		int invulnerableExpireTime;
		bool invulnerableActivated;
		bool invulnerableEnabled;
		bool playerMoved;
		float playerLOSCheckPos[2];
		float playerLOSCheckDir[2];
		int playerLOSPosTime;
		int playerADSTargetTime;
		unsigned int lastWeapon;
		bool previouslyFiring;
		bool previouslyUsingNightVision;
		int groundTiltEntNum;
	};

	struct scr_vehicle_s;

	struct item_ent_t
	{
		int ammoCount;
		int clipAmmoCount;
		int index;
	};

	struct sentient_t
	{
		int ent;
		int eTeam;
	};

	struct SentientHandle
	{
		unsigned __int16 number;
		unsigned __int16 infoIndex;
	};

	enum team_t
	{
		TEAM_FREE = 0x0,
		TEAM_BAD = 0x0,
		TEAM_AXIS = 0x1,
		TEAM_ALLIES = 0x2,
		TEAM_NEUTRAL = 0x3,
		TEAM_DEAD = 0x4,
		TEAM_NUM_TEAMS = 0x5,
	};

	struct TurretInfo
	{
		bool inuse;
		int state;
		int flags;
		int fireTime;
		EntHandle manualTarget;
		EntHandle target;
		float targetPos[3];
		int targetTime;
		float missOffsetNormalized[3];
		float arcmin[2];
		float arcmax[2];
		float initialYawmin;
		float initialYawmax;
		float forwardAngleDot;
		float dropPitch;
		int convergenceTime[2];
		int suppressTime;
		float maxRangeSquared;
		SentientHandle detachSentient;
		int stance;
		int prevStance;
		int fireSndDelay;
		float accuracy;
		float userOrigin[3];
		int prevSentTarget;
		float aiSpread;
		float playerSpread;
		team_t eTeam;
		float originError[3];
		float anglesError[3];
		float pitchCap;
		int triggerDown;
		unsigned __int16 fireSnd;
		unsigned __int16 fireSndPlayer;
		unsigned __int16 stopSnd;
		unsigned __int16 stopSndPlayer;
	};

	struct spawner_ent_t
	{
		int team;
		int timestamp;
	};

	struct __declspec(align(4)) trigger_ent_t
	{
		int threshold;
		int accumulate;
		int timestamp;
		bool requireLookAt;
	};

	struct mover_ent_t
	{
		float decelTime;
		float aDecelTime;
		float speed;
		float aSpeed;
		float midTime;
		float aMidTime;
		float pos1[3];
		float pos2[3];
		float pos3[3];
		float apos1[3];
		float apos2[3];
		float apos3[3];
	};

	enum MissileStage
	{
		MISSILESTAGE_SOFTLAUNCH = 0x0,
		MISSILESTAGE_ASCENT = 0x1,
		MISSILESTAGE_DESCENT = 0x2,
	};

	enum MissileFlightMode
	{
		MISSILEFLIGHTMODE_TOP = 0x0,
		MISSILEFLIGHTMODE_DIRECT = 0x1,
	};

	struct missile_ent_t
	{
		float predictLandPos[3];
		int predictLandTime;
		int timestamp;
		float time;
		int timeOfBirth;
		float travelDist;
		float surfaceNormal[3];
		team_t team;
		int thrownBack;
		float curvature[3];
		float targetOffset[3];
		MissileStage stage;
		MissileFlightMode flightMode;
	};


	union $148CB041F1C051EDB799F9C7708D3EB8
	{
		item_ent_t item[2];
		spawner_ent_t spawner;
		trigger_ent_t trigger;
		mover_ent_t mover;
		missile_ent_t missile;
	};

	struct soundEntity
	{
		unsigned __int16 notifyString;
		unsigned __int16 index;
		unsigned __int8 stoppable;
		int basetime;
		int duration;
	};

	struct gentity_s;

	struct tagInfo_s
	{
		gentity_s* parent;
		gentity_s* next;
		unsigned __int16 name;
		int index;
		float axis[4][3];
		float parentInvAxis[4][3];
	};

	struct animscripted_s
	{
		float axis[4][3];
		float originError[3];
		float anglesError[3];
		unsigned __int16 anim;
		unsigned __int16 root;
		unsigned __int8 bStarted;
		unsigned __int8 mode;
		float fHeightOfs;
		float fEndPitch;
		float fEndRoll;
		float fOrientLerp;
	};

	struct XAnimTree_s;

	struct gentity_s
	{
		entityState_s s;
		entityShared_t r;
		gclient_s* client;
		int actor; //wip
		sentient_t* sentient;
		scr_vehicle_s* scr_vehicle;
		TurretInfo* pTurretInfo;
		unsigned __int8 physicsObject;
		unsigned __int8 takedamage;
		unsigned __int8 active;
		unsigned __int8 nopickup;
		unsigned __int16 model;
		unsigned __int8 handler;
		unsigned __int16 classname;
		unsigned __int16 script_linkName;
		unsigned __int16 script_noteworthy;
		unsigned __int16 target;
		unsigned __int16 targetname;
		unsigned int attachIgnoreCollision;
		int spawnflags;
		int flags;
		int clipmask;
		int processedFrame;
		EntHandle parent;
		int nextthink;
		int health;
		int maxHealth;
		int nexteq;
		int damage;
		int count;
		gentity_s* chain;
		gentity_s* activator;
		$148CB041F1C051EDB799F9C7708D3EB8 ___u32;
		EntHandle missileTargetEnt;
		unsigned __int16 lookAtText0;
		unsigned __int16 lookAtText1;
		soundEntity snd_wait;
		tagInfo_s* tagInfo;
		gentity_s* tagChildren;
		animscripted_s* scripted;
		unsigned __int16 attachModelNames[31];
		unsigned __int16 attachTagNames[31];
		unsigned __int16 disconnectedLinks;
		int iDisconnectTime;
		float angleLerpRate;
		XAnimTree_s* pAnimTree;
		gentity_s* nextFree;
	};

	enum TraceHitType
	{
		TRACE_HITTYPE_NONE = 0x0,
		TRACE_HITTYPE_ENTITY = 0x1,
		TRACE_HITTYPE_DYNENT_MODEL = 0x2,
		TRACE_HITTYPE_DYNENT_BRUSH = 0x3,
	};

	struct __declspec(align(4)) trace_t
	{
		float fraction;
		vec3_t normal;
		int surfaceFlags;
		int contents;
		const char* material;
		TraceHitType hitType;
		unsigned __int16 hitId;
		unsigned __int16 modelIndex;
		unsigned __int16 partName;
		unsigned __int16 partGroup;
		bool allsolid;
		bool startsolid;
		bool walkable;
	};

	struct pml_t
	{
		vec3_t forward;
		vec3_t right;
		vec3_t up;
		float frametime;
		int msec;
		int walking;
		int groundPlane;
		trace_t groundTrace;
		float impactSpeed;
		vec3_t previous_origin;
		vec3_t previous_velocity;
	};

	struct pmove_t
	{
		playerState_s* ps;
		usercmd_s cmd;
		usercmd_s oldcmd;
		int tracemask;
		int numtouch;
		int touchents[32];
		float mins[3];
		float maxs[3];
		float xyspeed;
		int viewChangeTime;
		float viewChange;
		char handler;
	};

	struct CachedAssets_t
	{
		Material* scrollBarArrowUp;
		Material* scrollBarArrowDown;
		Material* scrollBarArrowLeft;
		Material* scrollBarArrowRight;
		Material* scrollBar;
		Material* scrollBarThumb;
		Material* sliderBar;
		Material* sliderThumb;
		Material* whiteMaterial;
		Material* cursor;
		Font_s* bigFont;
		Font_s* smallFont;
		Font_s* consoleFont;
		Font_s* boldFont;
		Font_s* textFont;
		Font_s* extraBigFont;
		Font_s* objectiveFont;
	};

	struct sharedUiInfo_t
	{
		CachedAssets_t assets;
	};

	struct GfxBspPreTessDrawSurf
	{
		unsigned __int16 baseSurfIndex;
		unsigned __int16 totalTriCount;
	};

	struct GfxReadCmdBuf
	{
		const unsigned int* primDrawSurfPos;
	};

	//From Bogdan
	union GfxColor
	{
		unsigned int packed;
		char array[4];
	};

	struct __declspec(align(4)) GfxLightImage
	{
		GfxImage* image;
		char samplerState;
	};

	struct GfxLightDef
	{	
		const char* name;
		GfxLightImage attenuation;
		int lmapLookupStart;
	};

	struct GfxLight
	{
		char type;
		char canUseShadowMap;
		char unused[2];
		float color[3];
		float dir[3];
		float origin[3];
		float radius;
		float cosHalfFovOuter;
		float cosHalfFovInner;
		int exponent;
		unsigned int spotShadowIndex;
		GfxLightDef* def;
	};

	struct GfxPlacement
	{
		float quat[4];
		float origin[3];
	};

	struct GfxMatrix
	{
		float m[4][4];
	};

	struct GfxScaledPlacement
	{
		GfxPlacement base;
		float scale;
	};

	struct DObjAnimMat
	{
		float quat[4];
		float trans[3];
		float transWeight;
	};

	struct XSurfaceVertexInfo
	{
		short vertCount[4];
		unsigned short* vertsBlend;
	};

	struct GfxPointVertex
	{
		float xyz[3];
		char color[4];
	};

	union PackedTexCoords
	{
		unsigned int packed;
	};

	struct GfxPackedVertex
	{
		float xyz[3];
		float binormalSign;
		GfxColor color;
		PackedTexCoords texCoord;
		PackedUnitVec normal;
		PackedUnitVec tangent;
	};

	struct __declspec(align(4)) GfxModelSurfaceInfo
	{
		DObjAnimMat* baseMat;
		char boneIndex;
		char boneCount;
		unsigned __int16 gfxEntIndex;
		unsigned __int16 lightingHandle;
	};

	union $178D1D161B34F636C03EBC0CA3007D75
	{
		GfxPackedVertex* skinnedVert;
		int oldSkinnedCachedOffset;
	};

	struct XSurfaceCollisionAabb
	{
		unsigned short mins[3];
		unsigned short maxs[3];
	};

	struct XSurfaceCollisionNode
	{
		XSurfaceCollisionAabb aabb;
		unsigned short childBeginIndex;
		unsigned short childCount;
	};

	struct XSurfaceCollisionLeaf
	{
		unsigned short triangleBeginIndex;
	};

	struct XSurfaceCollisionTree
	{
		float trans[3];
		float scale[3];
		unsigned int nodeCount;
		XSurfaceCollisionNode* nodes;
		unsigned int leafCount;
		XSurfaceCollisionLeaf* leafs;
	};

	struct XRigidVertList
	{
		unsigned short boneOffset;
		unsigned short vertCount;
		unsigned short triOffset;
		unsigned short triCount;
		XSurfaceCollisionTree* collisionTree;
	};

	struct XSurface
	{
		char tileMode;
		bool deformed;
		unsigned __int16 vertCount;
		unsigned __int16 triCount;
		char zoneHandle;
		unsigned __int16 baseTriIndex;
		unsigned __int16 baseVertIndex;
		unsigned __int16* triIndices;
		XSurfaceVertexInfo vertInfo;
		GfxPackedVertex* verts0;
		unsigned int vertListCount;
		XRigidVertList* vertList;
		int partBits[3]; // was 4
		IDirect3DVertexBuffer9* custom_vertexbuffer;
	}; STATIC_ASSERT_SIZE(XSurface, 0x38);

	struct GfxModelSkinnedSurface
	{
		int skinnedCachedOffset;
		XSurface* xsurf;
		GfxModelSurfaceInfo info;
		$178D1D161B34F636C03EBC0CA3007D75 u;
	};

	struct GfxModelRigidSurface
	{
		GfxModelSkinnedSurface surf;
		GfxScaledPlacement placement;
	};

	struct GfxParticleCloud
	{
		GfxScaledPlacement placement;
		float endpos[3];
		GfxColor color;
		float radius[2];
		unsigned int pad[2];
	};

	struct GfxViewParms
	{
		GfxMatrix viewMatrix;
		GfxMatrix projectionMatrix;
		GfxMatrix viewProjectionMatrix;
		GfxMatrix inverseViewProjectionMatrix;
		float origin[4];
		float axis[3][3];
		float depthHackNearClip;
		float zNear;
		float zFar;
	};

	struct FxCodeMeshData
	{
		unsigned int triCount;
		unsigned __int16* indices;
		unsigned __int16 argOffset;
		unsigned __int16 argCount;
		unsigned int pad;
	};

	struct GfxVertexBufferState
	{
		volatile int used;
		int total;
		//int IDirect3DVertexBuffer9; // 
		IDirect3DVertexBuffer9* buffer;
		char* verts;
	};

	struct GfxMeshData
	{
		unsigned int indexCount;
		unsigned int totalIndexCount;
		unsigned __int16* indices;
		GfxVertexBufferState vb;
		unsigned int vertSize;
	};

	union PackedLightingCoords
	{
		unsigned int packed;
		char array[4];
	};

	struct GfxSModelCachedVertex
	{
		float xyz[3];
		GfxColor color;
		PackedTexCoords texCoord;
		PackedUnitVec normal;
		PackedUnitVec tangent;
		PackedLightingCoords baseLighting;
	};

	struct GfxModelLightingPatch
	{
		unsigned __int16 modelLightingIndex;
		char primaryLightWeight;
		char colorsCount;
		char groundLighting[4];
		unsigned __int16 colorsWeight[8];
		unsigned __int16 colorsIndex[8];
	};

	struct GfxBackEndPrimitiveData
	{
		int hasSunDirChanged;
	};

	struct GfxEntity
	{
		unsigned int renderFxFlags;
		float materialTime;
	};

	struct GfxLodRamp
	{
		float scale;
		float bias;
	};

	struct __declspec(align(4)) GfxLodParms
	{
		float origin[3];
		GfxLodRamp ramp[2];
	};

	struct FxMarkMeshData
	{
		unsigned int triCount;
		unsigned __int16* indices;
		unsigned __int16 modelIndex;
		char modelTypeAndSurf;
		char pad0;
		unsigned int pad1;
	};

	struct GfxFog
	{
		int startTime;
		int finishTime;
		GfxColor color;
		float fogStart;
		float density;
	};

	struct GfxCmdHeader
	{
		unsigned __int16 id;
		unsigned __int16 byteCount;
	};

	struct GfxCmdArray
	{
		char* cmds;
		int usedTotal;
		int usedCritical;
		GfxCmdHeader* lastCmd;
	};

	struct GfxViewInfo;

	struct GfxDebugPoly
	{
		float color[4];
		int firstVert;
		int vertCount;
	};

	struct GfxDebugPlume
	{
		float origin[3];
		float color[4];
		int score;
		int startTime;
		int duration;
	};

	struct trDebugString_t
	{
		float xyz[3];
		float color[4];
		float scale;
		char text[96];
	};

	struct trDebugLine_t
	{
		float start[3];
		float end[3];
		float color[4];
		int depthTest;
	};

	struct DebugGlobals
	{
		float(*verts)[3];
		int vertCount;
		int vertLimit;
		GfxDebugPoly* polys;
		int polyCount;
		int polyLimit;
		trDebugString_t* strings;
		int stringCount;
		int stringLimit;
		trDebugString_t* externStrings;
		int externStringCount;
		int externMaxStringCount;
		trDebugLine_t* lines;
		int lineCount;
		int lineLimit;
		trDebugLine_t* externLines;
		int externLineCount;
		int externMaxLineCount;
		GfxDebugPlume* plumes;
		int plumeCount;
		int plumeLimit;
	};

#pragma warning( push )
#pragma warning( disable : 4324 )
	struct __declspec(align(16)) GfxBackEndData // align by 16
	{
		char surfsBuffer[131072];
		FxCodeMeshData codeMeshes[2048];
		unsigned int primDrawSurfsBuf[65536];
		GfxViewParms viewParms[28]; // GfxViewParms either has pad or zFar
		char primaryLightTechType[13][256];
		float codeMeshArgs[256][4];
		GfxParticleCloud clouds[256];
		GfxDrawSurf drawSurfs[32768];
		GfxMeshData codeMesh;
		GfxSModelCachedVertex smcPatchVerts[8192];
		unsigned __int16 smcPatchList[256];
		unsigned int smcPatchCount;
		unsigned int smcPatchVertsUsed;
		GfxModelLightingPatch modelLightingPatchList[4096];
		volatile int modelLightingPatchCount;
		GfxBackEndPrimitiveData prim;
		unsigned int shadowableLightHasShadowMap[8];
		unsigned int frameCount;
		int drawSurfCount;
		volatile int surfPos;
		volatile int gfxEntCount;
		GfxEntity gfxEnts[128];
		volatile int cloudCount;
		volatile int codeMeshCount;
		volatile int codeMeshArgsCount;
		volatile int markMeshCount;
		FxMarkMeshData markMeshes[1536];
		GfxMeshData markMesh;
		GfxVertexBufferState* skinnedCacheVb;
		int IDirect3DQuery9; //IDirect3DQuery9 *endFence
		char* tempSkinBuf;
		volatile int tempSkinPos;
		int IDirect3DIndexBuffer9; // IDirect3DIndexBuffer9 *preTessIb
		int viewParmCount;
		GfxFog fogSettings;
		GfxCmdArray* commands;
		unsigned int viewInfoIndex;
		unsigned int viewInfoCount;
		GfxViewInfo* viewInfo;
		const void* cmds;
		GfxLight sunLight;
		int hasApproxSunDirChanged;
		volatile int primDrawSurfPos;
		unsigned int* staticModelLit;
		DebugGlobals debugGlobals;
		unsigned int drawType;
	};
	STATIC_ASSERT_OFFSET(GfxBackEndData, codeMesh, 0xB0000);
	STATIC_ASSERT_OFFSET(GfxBackEndData, markMesh, 0x11E670);
#pragma warning( pop )

	struct GfxDynamicIndices
	{
		volatile int used;
		int total;
		unsigned __int16* indices;
	};

	struct GfxIndexBufferState
	{
		volatile int used;
		int total;
		IDirect3DIndexBuffer9* buffer;
		unsigned __int16* indices;
	};

	struct GfxPackedVertexNormal
	{
		PackedUnitVec normal;
		PackedUnitVec tangent;
	};

	struct __declspec(align(4)) GfxBuffers
	{
		GfxDynamicIndices smodelCache;
		IDirect3DVertexBuffer9* smodelCacheVb;
		GfxIndexBufferState preTessIndexBufferPool[2];
		GfxIndexBufferState* preTessIndexBuffer;
		int preTessBufferFrame;
		GfxIndexBufferState dynamicIndexBufferPool[1];
		GfxIndexBufferState* dynamicIndexBuffer;
		GfxVertexBufferState skinnedCacheVbPool[2];
		char* skinnedCacheLockAddr;
		GfxVertexBufferState dynamicVertexBufferPool[1];
		GfxVertexBufferState* dynamicVertexBuffer;
		IDirect3DVertexBuffer9* particleCloudVertexBuffer;
		IDirect3DIndexBuffer9* particleCloudIndexBuffer;
		int dynamicBufferFrame;
		GfxPackedVertexNormal skinnedCacheNormals[2][147456];
		GfxPackedVertexNormal* skinnedCacheNormalsAddr;
		GfxPackedVertexNormal* oldSkinnedCacheNormalsAddr;
		unsigned int skinnedCacheNormalsFrameCount;
		bool fastSkin;
		bool skinCache;
	};

	struct DSkelPartBits
	{
		int anim[4];
		int control[4];
		int skel[4];
	};

	struct DSkel
	{
		DSkelPartBits partBits;
		int timeStamp;
		DObjAnimMat* mat;
	};

	struct XModelSurfs
	{
		const char* name;
		XSurface* surfs;
		unsigned __int16 numsurfs;
		int partBits[6];
	};

	// < rtx begin
	/*struct XModelLodInfo
	{
		float dist;
		unsigned __int16 numsurfs;
		unsigned __int16 surfIndex;
		XModelSurfs* modelSurfs;
		int partBits[6];
		XSurface* surfs;
		char lod;
		char smcBaseIndexPlusOne;
		char smcSubIndexMask;
		char smcBucket;
	};*/

	struct XModelLodInfo
	{
		float dist;
		unsigned __int16 numsurfs;
		unsigned __int16 surfIndex;
		int partBits[4];
		char lod;
		char smcIndexPlusOne;
		char smcAllocBits;
		char unused;
	};

	// rtx end >

	struct XModelCollTri_s
	{
		float plane[4];
		float svec[4];
		float tvec[4];
	};

	struct Bounds
	{
		vec3_t midPoint;
		vec3_t halfSize;
	};

	struct XModelCollSurf_s
	{
		XModelCollTri_s* collTris;
		int numCollTris;
		Bounds bounds;
		int boneIdx;
		int contents;
		int surfFlags;
	};

	struct XBoneInfo
	{
		Bounds bounds;
		float radiusSquared;
	};

#pragma pack(push, 4)
	struct PhysPreset
	{
		const char* name;
		int type;
		float mass;
		float bounce;
		float friction;
		float bulletForceScale;
		float explosiveForceScale;
		const char* sndAliasPrefix;
		float piecesSpreadFraction;
		float piecesUpwardVelocity;
		char tempDefaultToCylinder;
	};
#pragma pack(pop)

	struct cplane_s
	{
		float normal[3];
		float dist;
		char type;
		char pad[3];
	};

	struct cbrushside_t
	{
		cplane_s* plane;
		unsigned __int16 materialNum;
		char firstAdjacentSideOffset;
		char edgeCount;
	};


#pragma pack(push, 16)
	struct cbrush_t
	{
		float mins[3];
		int contents;
		float maxs[3];
		unsigned int numsides;
		cbrushside_t* sides;
		__int16 axialMaterialNum[2][3];
		char* baseAdjacentSide;
		__int16 firstAdjacentSideOffsets[2][3];
		char edgeCount[2][3];
		__int16 colorCounter;
		__int16 cmBrushIndex;
		//float distFromCam;
		__int16 cmSubmodelIndex;
		bool isSubmodel;
		bool pad;
	};
#pragma pack(pop)

	struct BrushWrapper
	{
		Bounds bounds;
		cbrush_t brush;
		int totalEdgeCount;
		cplane_s* planes;
	};


	struct PhysGeomInfo
	{
		BrushWrapper* brushWrapper;
		int type;
		float orientation[3][3];
		Bounds bounds;
	};

	struct PhysMass
	{
		float centerOfMass[3];
		float momentsOfInertia[3];
		float productsOfInertia[3];
	};

	struct PhysCollmap
	{
		const char* name;
		unsigned int count;
		PhysGeomInfo* geoms;
		PhysMass mass;
	};

	struct XModelDrawInfo
	{
		char hasGfxEntIndex;
		char lod;
		unsigned __int16 surfId;
	};

	struct XModelHighMipBounds
	{
		float mins[3];
		float maxs[3];
	};

	struct XModelStreamInfo
	{
		XModelHighMipBounds* highMipBounds;
	};

	struct XModel
	{
		const char* name;
		char numBones;
		char numRootBones;
		unsigned char numsurfs;
		char lodRampType;
		unsigned __int16* boneNames;
		char* parentList;
		__int16* quats;
		float* trans;
		char* partClassification;
		DObjAnimMat* baseMat;
		XSurface* surfs;
		Material** materialHandles;
		XModelLodInfo lodInfo[4];
		XModelCollSurf_s* collSurfs;
		int numCollSurfs;
		int contents;
		XBoneInfo* boneInfo;
		float radius;
		float mins[3];
		float maxs[3];
		__int16 numLods;
		__int16 collLod;
		XModelStreamInfo streamInfo;
		int memUsage;
		char flags;
		bool bad;
		PhysPreset* physPreset;
		PhysCollmap* physGeoms;
	};


	enum weapType_t
	{
		WEAPTYPE_BULLET = 0x0,
		WEAPTYPE_GRENADE = 0x1,
		WEAPTYPE_PROJECTILE = 0x2,
		WEAPTYPE_BINOCULARS = 0x3,
		WEAPTYPE_NUM = 0x4,
	};

	enum weapClass_t
	{
		WEAPCLASS_RIFLE = 0x0,
		WEAPCLASS_MG = 0x1,
		WEAPCLASS_SMG = 0x2,
		WEAPCLASS_SPREAD = 0x3,
		WEAPCLASS_PISTOL = 0x4,
		WEAPCLASS_GRENADE = 0x5,
		WEAPCLASS_ROCKETLAUNCHER = 0x6,
		WEAPCLASS_TURRET = 0x7,
		WEAPCLASS_NON_PLAYER = 0x8,
		WEAPCLASS_ITEM = 0x9,
		WEAPCLASS_NUM = 0xA,
	};

	enum PenetrateType
	{
		PENETRATE_TYPE_NONE = 0x0,
		PENETRATE_TYPE_SMALL = 0x1,
		PENETRATE_TYPE_MEDIUM = 0x2,
		PENETRATE_TYPE_LARGE = 0x3,
		PENETRATE_TYPE_COUNT = 0x4,
	};

	enum ImpactType
	{
		IMPACT_TYPE_NONE = 0x0,
		IMPACT_TYPE_BULLET_SMALL = 0x1,
		IMPACT_TYPE_BULLET_LARGE = 0x2,
		IMPACT_TYPE_BULLET_AP = 0x3,
		IMPACT_TYPE_SHOTGUN = 0x4,
		IMPACT_TYPE_GRENADE_BOUNCE = 0x5,
		IMPACT_TYPE_GRENADE_EXPLODE = 0x6,
		IMPACT_TYPE_ROCKET_EXPLODE = 0x7,
		IMPACT_TYPE_PROJECTILE_DUD = 0x8,
		IMPACT_TYPE_COUNT = 0x9,
	};

	enum weapInventoryType_t
	{
		WEAPINVENTORY_PRIMARY = 0x0,
		WEAPINVENTORY_OFFHAND = 0x1,
		WEAPINVENTORY_ITEM = 0x2,
		WEAPINVENTORY_ALTMODE = 0x3,
		WEAPINVENTORYCOUNT = 0x4,
	};

	enum weapFireType_t
	{
		WEAPON_FIRETYPE_FULLAUTO = 0x0,
		WEAPON_FIRETYPE_SINGLESHOT = 0x1,
		WEAPON_FIRETYPE_BURSTFIRE2 = 0x2,
		WEAPON_FIRETYPE_BURSTFIRE3 = 0x3,
		WEAPON_FIRETYPE_BURSTFIRE4 = 0x4,
		WEAPON_FIRETYPECOUNT = 0x5,
	};

	enum OffhandClass
	{
		OFFHAND_CLASS_NONE = 0x0,
		OFFHAND_CLASS_FRAG_GRENADE = 0x1,
		OFFHAND_CLASS_SMOKE_GRENADE = 0x2,
		OFFHAND_CLASS_FLASH_GRENADE = 0x3,
		OFFHAND_CLASS_COUNT = 0x4,
	};

	enum weapStance_t
	{
		WEAPSTANCE_STAND = 0x0,
		WEAPSTANCE_DUCK = 0x1,
		WEAPSTANCE_PRONE = 0x2,
		WEAPSTANCE_NUM = 0x3,
	};

	enum activeReticleType_t
	{
		VEH_ACTIVE_RETICLE_NONE = 0x0,
		VEH_ACTIVE_RETICLE_PIP_ON_A_STICK = 0x1,
		VEH_ACTIVE_RETICLE_BOUNCING_DIAMOND = 0x2,
		VEH_ACTIVE_RETICLE_COUNT = 0x3,
	};

	enum weaponIconRatioType_t
	{
		WEAPON_ICON_RATIO_1TO1 = 0x0,
		WEAPON_ICON_RATIO_2TO1 = 0x1,
		WEAPON_ICON_RATIO_4TO1 = 0x2,
		WEAPON_ICON_RATIO_COUNT = 0x3,
	};

	enum ammoCounterClipType_t
	{
		AMMO_COUNTER_CLIP_NONE = 0x0,
		AMMO_COUNTER_CLIP_MAGAZINE = 0x1,
		AMMO_COUNTER_CLIP_SHORTMAGAZINE = 0x2,
		AMMO_COUNTER_CLIP_SHOTGUN = 0x3,
		AMMO_COUNTER_CLIP_ROCKET = 0x4,
		AMMO_COUNTER_CLIP_BELTFED = 0x5,
		AMMO_COUNTER_CLIP_ALTWEAPON = 0x6,
		AMMO_COUNTER_CLIP_COUNT = 0x7,
	};

	enum weapOverlayReticle_t
	{
		WEAPOVERLAYRETICLE_NONE = 0x0,
		WEAPOVERLAYRETICLE_CROSSHAIR = 0x1,
		WEAPOVERLAYRETICLE_NUM = 0x2,
	};

	enum WeapOverlayInteface_t
	{
		WEAPOVERLAYINTERFACE_NONE = 0x0,
		WEAPOVERLAYINTERFACE_JAVELIN = 0x1,
		WEAPOVERLAYINTERFACE_TURRETSCOPE = 0x2,
		WEAPOVERLAYINTERFACECOUNT = 0x3,
	};

	enum weapProjExposion_t
	{
		WEAPPROJEXP_GRENADE = 0x0,
		WEAPPROJEXP_ROCKET = 0x1,
		WEAPPROJEXP_FLASHBANG = 0x2,
		WEAPPROJEXP_NONE = 0x3,
		WEAPPROJEXP_DUD = 0x4,
		WEAPPROJEXP_SMOKE = 0x5,
		WEAPPROJEXP_HEAVY = 0x6,
		WEAPPROJEXP_NUM = 0x7,
	};

	enum WeapStickinessType
	{
		WEAPSTICKINESS_NONE = 0x0,
		WEAPSTICKINESS_ALL = 0x1,
		WEAPSTICKINESS_GROUND = 0x2,
		WEAPSTICKINESS_GROUND_WITH_YAW = 0x3,
		WEAPSTICKINESS_COUNT = 0x4,
	};

	enum guidedMissileType_t
	{
		MISSILE_GUIDANCE_NONE = 0x0,
		MISSILE_GUIDANCE_SIDEWINDER = 0x1,
		MISSILE_GUIDANCE_HELLFIRE = 0x2,
		MISSILE_GUIDANCE_JAVELIN = 0x3,
		MISSILE_GUIDANCE_COUNT = 0x4,
	};

	struct FxSpawnDefLooping
	{
		int intervalMsec;
		int count;
	};

	struct FxIntRange
	{
		int base;
		int amplitude;
	};

	struct FxSpawnDefOneShot
	{
		FxIntRange count;
	};

	union FxSpawnDef
	{
		FxSpawnDefLooping looping;
		FxSpawnDefOneShot oneShot;
	};

	struct FxFloatRange
	{
		float base;
		float amplitude;
	};

	struct FxElemAtlas
	{
		char behavior;
		char index;
		char fps;
		char loopCount;
		char colIndexBits;
		char rowIndexBits;
		__int16 entryCount;
	};

	struct FxElemVec3Range
	{
		float base[3];
		float amplitude[3];
	};

	struct FxElemVelStateInFrame
	{
		FxElemVec3Range velocity;
		FxElemVec3Range totalDelta;
	};

	const struct FxElemVelStateSample
	{
		FxElemVelStateInFrame local;
		FxElemVelStateInFrame world;
	};

	struct FxElemVisualState
	{
		char color[4];
		float rotationDelta;
		float rotationTotal;
		float size[2];
		float scale;
	};

	const struct FxElemVisStateSample
	{
		FxElemVisualState base;
		FxElemVisualState amplitude;
	};

	struct FxEffectDef;

	union FxEffectDefRef
	{
		FxEffectDef* handle;
		const char* name;
	};

	union FxElemVisuals
	{
		const void* anonymous;
		Material* material;
		XModel* model;
		FxEffectDefRef effectDef;
		const char* soundName;
	};

	struct FxElemMarkVisuals
	{
		Material* materials[2];
	};

	union FxElemDefVisuals
	{
		FxElemMarkVisuals* markArray;
		FxElemVisuals* array;
		FxElemVisuals instance;
	};

	struct FxTrailVertex
	{
		float pos[2];
		float normal[2];
		float texCoord;
	};

	struct FxTrailDef
	{
		int scrollTimeMsec;
		int repeatDist;
		int splitDist;
		int vertCount;
		FxTrailVertex* verts;
		int indCount;
		unsigned __int16* inds;
	};

	const struct FxElemDef
	{
		int flags;
		FxSpawnDef spawn;
		FxFloatRange spawnRange;
		FxFloatRange fadeInRange;
		FxFloatRange fadeOutRange;
		float spawnFrustumCullRadius;
		FxIntRange spawnDelayMsec;
		FxIntRange lifeSpanMsec;
		FxFloatRange spawnOrigin[3];
		FxFloatRange spawnOffsetRadius;
		FxFloatRange spawnOffsetHeight;
		FxFloatRange spawnAngles[3];
		FxFloatRange angularVelocity[3];
		FxFloatRange initialRotation;
		FxFloatRange gravity;
		FxFloatRange reflectionFactor;
		FxElemAtlas atlas;
		char elemType;
		char visualCount;
		char velIntervalCount;
		char visStateIntervalCount;
		FxElemVelStateSample* velSamples;
		FxElemVisStateSample* visSamples;
		FxElemDefVisuals visuals;
		float collMins[3];
		float collMaxs[3];
		FxEffectDefRef effectOnImpact;
		FxEffectDefRef effectOnDeath;
		FxEffectDefRef effectEmitted;
		FxFloatRange emitDist;
		FxFloatRange emitDistVariance;
		FxTrailDef* trailDef;
		char sortOrder;
		char lightingFrac;
		char useItemClip;
		char unused[1];
	};

	struct FxEffectDef
	{
		const char* name;
		int flags;
		int totalSize;
		int msecLoopingLife;
		int elemDefCountLooping;
		int elemDefCountOneShot;
		int elemDefCountEmission;
		FxElemDef* elemDefs;
	};

	struct FxImpactEntry
	{
		const FxEffectDef* nonflesh[29];
		const FxEffectDef* flesh[4];
	};

	struct FxImpactTable
	{
		const char* name;
		FxImpactEntry* table;
	};

	enum weaponstate_t
	{
		WEAPON_READY = 0x0,
		WEAPON_RAISING = 0x1,
		WEAPON_RAISING_ALTSWITCH = 0x2,
		WEAPON_DROPPING = 0x3,
		WEAPON_DROPPING_QUICK = 0x4,
		WEAPON_FIRING = 0x5,
		WEAPON_RECHAMBERING = 0x6,
		WEAPON_RELOADING = 0x7,
		WEAPON_RELOADING_INTERUPT = 0x8,
		WEAPON_RELOAD_START = 0x9,
		WEAPON_RELOAD_START_INTERUPT = 0xA,
		WEAPON_RELOAD_END = 0xB,
		WEAPON_MELEE_INIT = 0xC,
		WEAPON_MELEE_FIRE = 0xD,
		WEAPON_MELEE_END = 0xE,
		WEAPON_OFFHAND_INIT = 0xF,
		WEAPON_OFFHAND_PREPARE = 0x10,
		WEAPON_OFFHAND_HOLD = 0x11,
		WEAPON_OFFHAND_START = 0x12,
		WEAPON_OFFHAND = 0x13,
		WEAPON_OFFHAND_END = 0x14,
		WEAPON_DETONATING = 0x15,
		WEAPON_SPRINT_RAISE = 0x16,
		WEAPON_SPRINT_LOOP = 0x17,
		WEAPON_SPRINT_DROP = 0x18,
		WEAPON_NIGHTVISION_WEAR = 0x19,
		WEAPON_NIGHTVISION_REMOVE = 0x1A,
	};

	enum weapAnimFiles_t
	{
		WEAP_ANIM_ROOT = 0x0,
		WEAP_ANIM_IDLE = 0x1,
		WEAP_ANIM_EMPTY_IDLE = 0x2,
		WEAP_ANIM_FIRE = 0x3,
		WEAP_ANIM_HOLD_FIRE = 0x4,
		WEAP_ANIM_LASTSHOT = 0x5,
		WEAP_ANIM_RECHAMBER = 0x6,
		WEAP_ANIM_MELEE = 0x7,
		WEAP_ANIM_MELEE_CHARGE = 0x8,
		WEAP_ANIM_RELOAD = 0x9,
		WEAP_ANIM_RELOAD_EMPTY = 0xA,
		WEAP_ANIM_RELOAD_START = 0xB,
		WEAP_ANIM_RELOAD_END = 0xC,
		WEAP_ANIM_RAISE = 0xD,
		WEAP_ANIM_FIRST_RAISE = 0xE,
		WEAP_ANIM_DROP = 0xF,
		WEAP_ANIM_ALT_RAISE = 0x10,
		WEAP_ANIM_ALT_DROP = 0x11,
		WEAP_ANIM_QUICK_RAISE = 0x12,
		WEAP_ANIM_QUICK_DROP = 0x13,
		WEAP_ANIM_EMPTY_RAISE = 0x14,
		WEAP_ANIM_EMPTY_DROP = 0x15,
		WEAP_ANIM_SPRINT_IN = 0x16,
		WEAP_ANIM_SPRINT_LOOP = 0x17,
		WEAP_ANIM_SPRINT_OUT = 0x18,
		WEAP_ANIM_DETONATE = 0x19,
		WEAP_ANIM_NIGHTVISION_WEAR = 0x1A,
		WEAP_ANIM_NIGHTVISION_REMOVE = 0x1B,
		WEAP_ANIM_ADS_FIRE = 0x1C,
		WEAP_ANIM_ADS_LASTSHOT = 0x1D,
		WEAP_ANIM_ADS_RECHAMBER = 0x1E,
		WEAP_ANIM_ADS_UP = 0x1F,
		WEAP_ANIM_ADS_DOWN = 0x20,
		NUM_WEAP_ANIMS = 0x21,
	};

	struct WeaponDef_s
	{
		const char* szInternalName;
		const char* szDisplayName;
		const char* szOverlayName;
		XModel* gunXModel[16];
		XModel* handXModel;
		const char* szXAnims[33];
		const char* szModeName;
		unsigned __int16 hideTags[8];
		unsigned __int16 notetrackSoundMapKeys[16];
		unsigned __int16 notetrackSoundMapValues[16];
		int playerAnimType;
		weapType_t weapType;
		weapClass_t weapClass;
		PenetrateType penetrateType;
		ImpactType impactType;
		weapInventoryType_t inventoryType;
		weapFireType_t fireType;
		OffhandClass offhandClass;
		weapStance_t stance;
		FxEffectDef* viewFlashEffect;
		FxEffectDef* worldFlashEffect;
		snd_alias_list_t* pickupSound;
		snd_alias_list_t* pickupSoundPlayer;
		snd_alias_list_t* ammoPickupSound;
		snd_alias_list_t* ammoPickupSoundPlayer;
		snd_alias_list_t* projectileSound;
		snd_alias_list_t* pullbackSound;
		snd_alias_list_t* pullbackSoundPlayer;
		snd_alias_list_t* fireSound;
		snd_alias_list_t* fireSoundPlayer;
		snd_alias_list_t* fireLoopSound;
		snd_alias_list_t* fireLoopSoundPlayer;
		snd_alias_list_t* fireStopSound;
		snd_alias_list_t* fireStopSoundPlayer;
		snd_alias_list_t* fireLastSound;
		snd_alias_list_t* fireLastSoundPlayer;
		snd_alias_list_t* emptyFireSound;
		snd_alias_list_t* emptyFireSoundPlayer;
		snd_alias_list_t* meleeSwipeSound;
		snd_alias_list_t* meleeSwipeSoundPlayer;
		snd_alias_list_t* meleeHitSound;
		snd_alias_list_t* meleeMissSound;
		snd_alias_list_t* rechamberSound;
		snd_alias_list_t* rechamberSoundPlayer;
		snd_alias_list_t* reloadSound;
		snd_alias_list_t* reloadSoundPlayer;
		snd_alias_list_t* reloadEmptySound;
		snd_alias_list_t* reloadEmptySoundPlayer;
		snd_alias_list_t* reloadStartSound;
		snd_alias_list_t* reloadStartSoundPlayer;
		snd_alias_list_t* reloadEndSound;
		snd_alias_list_t* reloadEndSoundPlayer;
		snd_alias_list_t* detonateSound;
		snd_alias_list_t* detonateSoundPlayer;
		snd_alias_list_t* nightVisionWearSound;
		snd_alias_list_t* nightVisionWearSoundPlayer;
		snd_alias_list_t* nightVisionRemoveSound;
		snd_alias_list_t* nightVisionRemoveSoundPlayer;
		snd_alias_list_t* altSwitchSound;
		snd_alias_list_t* altSwitchSoundPlayer;
		snd_alias_list_t* raiseSound;
		snd_alias_list_t* raiseSoundPlayer;
		snd_alias_list_t* firstRaiseSound;
		snd_alias_list_t* firstRaiseSoundPlayer;
		snd_alias_list_t* putawaySound;
		snd_alias_list_t* putawaySoundPlayer;
		snd_alias_list_t** bounceSound;
		FxEffectDef* viewShellEjectEffect;
		FxEffectDef* worldShellEjectEffect;
		FxEffectDef* viewLastShotEjectEffect;
		FxEffectDef* worldLastShotEjectEffect;
		Material* reticleCenter;
		Material* reticleSide;
		int iReticleCenterSize;
		int iReticleSideSize;
		int iReticleMinOfs;
		activeReticleType_t activeReticleType;
		float vStandMove[3];
		float vStandRot[3];
		float vDuckedOfs[3];
		float vDuckedMove[3];
		float vDuckedRot[3];
		float vProneOfs[3];
		float vProneMove[3];
		float vProneRot[3];
		float fPosMoveRate;
		float fPosProneMoveRate;
		float fStandMoveMinSpeed;
		float fDuckedMoveMinSpeed;
		float fProneMoveMinSpeed;
		float fPosRotRate;
		float fPosProneRotRate;
		float fStandRotMinSpeed;
		float fDuckedRotMinSpeed;
		float fProneRotMinSpeed;
		XModel* worldModel[16];
		XModel* worldClipModel;
		XModel* rocketModel;
		XModel* knifeModel;
		XModel* worldKnifeModel;
		Material* hudIcon;
		weaponIconRatioType_t hudIconRatio;
		Material* ammoCounterIcon;
		weaponIconRatioType_t ammoCounterIconRatio;
		ammoCounterClipType_t ammoCounterClip;
		int iStartAmmo;
		const char* szAmmoName;
		int iAmmoIndex;
		const char* szClipName;
		int iClipIndex;
		int iMaxAmmo;
		int iClipSize;
		int shotCount;
		const char* szSharedAmmoCapName;
		int iSharedAmmoCapIndex;
		int iSharedAmmoCap;
		int damage;
		int playerDamage;
		int iMeleeDamage;
		int iDamageType;
		int iFireDelay;
		int iMeleeDelay;
		int meleeChargeDelay;
		int iDetonateDelay;
		int iFireTime;
		int iRechamberTime;
		int iRechamberBoltTime;
		int iHoldFireTime;
		int iDetonateTime;
		int iMeleeTime;
		int meleeChargeTime;
		int iReloadTime;
		int reloadShowRocketTime;
		int iReloadEmptyTime;
		int iReloadAddTime;
		int iReloadStartTime;
		int iReloadStartAddTime;
		int iReloadEndTime;
		int iDropTime;
		int iRaiseTime;
		int iAltDropTime;
		int iAltRaiseTime;
		int quickDropTime;
		int quickRaiseTime;
		int iFirstRaiseTime;
		int iEmptyRaiseTime;
		int iEmptyDropTime;
		int sprintInTime;
		int sprintLoopTime;
		int sprintOutTime;
		int nightVisionWearTime;
		int nightVisionWearTimeFadeOutEnd;
		int nightVisionWearTimePowerUp;
		int nightVisionRemoveTime;
		int nightVisionRemoveTimePowerDown;
		int nightVisionRemoveTimeFadeInStart;
		int fuseTime;
		int aiFuseTime;
		int requireLockonToFire;
		int noAdsWhenMagEmpty;
		int avoidDropCleanup;
		float autoAimRange;
		float aimAssistRange;
		float aimAssistRangeAds;
		float aimPadding;
		float enemyCrosshairRange;
		int crosshairColorChange;
		float moveSpeedScale;
		float adsMoveSpeedScale;
		float sprintDurationScale;
		float fAdsZoomFov;
		float fAdsZoomInFrac;
		float fAdsZoomOutFrac;
		Material* overlayMaterial;
		Material* overlayMaterialLowRes;
		weapOverlayReticle_t overlayReticle;
		WeapOverlayInteface_t overlayInterface;
		float overlayWidth;
		float overlayHeight;
		float fAdsBobFactor;
		float fAdsViewBobMult;
		float fHipSpreadStandMin;
		float fHipSpreadDuckedMin;
		float fHipSpreadProneMin;
		float hipSpreadStandMax;
		float hipSpreadDuckedMax;
		float hipSpreadProneMax;
		float fHipSpreadDecayRate;
		float fHipSpreadFireAdd;
		float fHipSpreadTurnAdd;
		float fHipSpreadMoveAdd;
		float fHipSpreadDuckedDecay;
		float fHipSpreadProneDecay;
		float fHipReticleSidePos;
		int iAdsTransInTime;
		int iAdsTransOutTime;
		float fAdsIdleAmount;
		float fHipIdleAmount;
		float adsIdleSpeed;
		float hipIdleSpeed;
		float fIdleCrouchFactor;
		float fIdleProneFactor;
		float fGunMaxPitch;
		float fGunMaxYaw;
		float swayMaxAngle;
		float swayLerpSpeed;
		float swayPitchScale;
		float swayYawScale;
		float swayHorizScale;
		float swayVertScale;
		float swayShellShockScale;
		float adsSwayMaxAngle;
		float adsSwayLerpSpeed;
		float adsSwayPitchScale;
		float adsSwayYawScale;
		float adsSwayHorizScale;
		float adsSwayVertScale;
		int bRifleBullet;
		int armorPiercing;
		int bBoltAction;
		int aimDownSight;
		int bRechamberWhileAds;
		float adsViewErrorMin;
		float adsViewErrorMax;
		int bCookOffHold;
		int bClipOnly;
		int adsFireOnly;
		int cancelAutoHolsterWhenEmpty;
		int suppressAmmoReserveDisplay;
		int enhanced;
		int laserSightDuringNightvision;
		Material* killIcon;
		weaponIconRatioType_t killIconRatio;
		int flipKillIcon;
		Material* dpadIcon;
		weaponIconRatioType_t dpadIconRatio;
		int bNoPartialReload;
		int bSegmentedReload;
		int iReloadAmmoAdd;
		int iReloadStartAdd;
		const char* szAltWeaponName;
		unsigned int altWeaponIndex;
		int iDropAmmoMin;
		int iDropAmmoMax;
		int blocksProne;
		int silenced;
		int iExplosionRadius;
		int iExplosionRadiusMin;
		int iExplosionInnerDamage;
		int iExplosionOuterDamage;
		float damageConeAngle;
		int iProjectileSpeed;
		int iProjectileSpeedUp;
		int iProjectileSpeedForward;
		int iProjectileActivateDist;
		float projLifetime;
		float timeToAccelerate;
		float projectileCurvature;
		XModel* projectileModel;
		weapProjExposion_t projExplosion;
		FxEffectDef* projExplosionEffect;
		int projExplosionEffectForceNormalUp;
		FxEffectDef* projDudEffect;
		snd_alias_list_t* projExplosionSound;
		snd_alias_list_t* projDudSound;
		int bProjImpactExplode;
		WeapStickinessType stickiness;
		int hasDetonator;
		int timedDetonation;
		int rotate;
		int holdButtonToThrow;
		int freezeMovementWhenFiring;
		float lowAmmoWarningThreshold;
		float parallelBounce[29];
		float perpendicularBounce[29];
		FxEffectDef* projTrailEffect;
		float vProjectileColor[3];
		guidedMissileType_t guidedMissileType;
		float maxSteeringAccel;
		int projIgnitionDelay;
		FxEffectDef* projIgnitionEffect;
		snd_alias_list_t* projIgnitionSound;
		float fAdsAimPitch;
		float fAdsCrosshairInFrac;
		float fAdsCrosshairOutFrac;
		int adsGunKickReducedKickBullets;
		float adsGunKickReducedKickPercent;
		float fAdsGunKickPitchMin;
		float fAdsGunKickPitchMax;
		float fAdsGunKickYawMin;
		float fAdsGunKickYawMax;
		float fAdsGunKickAccel;
		float fAdsGunKickSpeedMax;
		float fAdsGunKickSpeedDecay;
		float fAdsGunKickStaticDecay;
		float fAdsViewKickPitchMin;
		float fAdsViewKickPitchMax;
		float fAdsViewKickYawMin;
		float fAdsViewKickYawMax;
		float fAdsViewKickCenterSpeed;
		float fAdsViewScatterMin;
		float fAdsViewScatterMax;
		float fAdsSpread;
		int hipGunKickReducedKickBullets;
		float hipGunKickReducedKickPercent;
		float fHipGunKickPitchMin;
		float fHipGunKickPitchMax;
		float fHipGunKickYawMin;
		float fHipGunKickYawMax;
		float fHipGunKickAccel;
		float fHipGunKickSpeedMax;
		float fHipGunKickSpeedDecay;
		float fHipGunKickStaticDecay;
		float fHipViewKickPitchMin;
		float fHipViewKickPitchMax;
		float fHipViewKickYawMin;
		float fHipViewKickYawMax;
		float fHipViewKickCenterSpeed;
		float fHipViewScatterMin;
		float fHipViewScatterMax;
		float fightDist;
		float maxDist;
		const char* accuracyGraphName[2];
		float(*accuracyGraphKnots[2])[2];
		float(*originalAccuracyGraphKnots[2])[2];
		int accuracyGraphKnotCount[2];
		int originalAccuracyGraphKnotCount[2];
		int iPositionReloadTransTime;
		float leftArc;
		float rightArc;
		float topArc;
		float bottomArc;
		float accuracy;
		float aiSpread;
		float playerSpread;
		float minTurnSpeed[2];
		float maxTurnSpeed[2];
		float pitchConvergenceTime;
		float yawConvergenceTime;
		float suppressTime;
		float maxRange;
		float fAnimHorRotateInc;
		float fPlayerPositionDist;
		const char* szUseHintString;
		const char* dropHintString;
		int iUseHintStringIndex;
		int dropHintStringIndex;
		float horizViewJitter;
		float vertViewJitter;
		const char* szScript;
		float fOOPosAnimLength[2];
		int minDamage;
		int minPlayerDamage;
		float fMaxDamageRange;
		float fMinDamageRange;
		float destabilizationRateTime;
		float destabilizationCurvatureMax;
		int destabilizeDistance;
		float locationDamageMultipliers[19];
		const char* fireRumble;
		const char* meleeImpactRumble;
		float adsDofStart;
		float adsDofEnd;
	};

	typedef uint16_t ScriptString;

	typedef union
	{
		char* _1;
		uint16_t* _2;
		void* data;
	}XAnimIndices;

	typedef union
	{
		char(*_1)[3];
		uint16_t(*_2)[3];
	}XAnimDynamicFrames;

	typedef union
	{
		char _1[1];
		uint16_t _2[1];
	}XAnimDynamicIndices;

	typedef struct
	{
		float mins[3];
		float size[3];
		XAnimDynamicFrames frames;
		XAnimDynamicIndices indices;
	}XAnimPartTransFrames;

	typedef union
	{
		XAnimPartTransFrames frames;
		float frame0[3];
	}XAnimPartTransData;

	typedef struct
	{
		uint16_t size;
		char smallTrans;
		char pad;
		XAnimPartTransData u;
	}XAnimPartTrans;

	typedef struct
	{
		int16_t(*frames)[2];
		XAnimDynamicIndices indices;
	}XAnimDeltaPartQuatDataFrames;

	typedef union
	{
		XAnimDeltaPartQuatDataFrames frames;
		int16_t frame0[2];
	}XAnimDeltaPartQuatData;

	typedef struct
	{
		uint16_t size;
		short pad;
		XAnimDeltaPartQuatData u;
	}XAnimDeltaPartQuat;

	typedef struct
	{
		XAnimPartTrans* trans;
		XAnimDeltaPartQuat* quat;
	}XAnimDeltaPart;

	typedef struct
	{
		ScriptString name;
		short pad;
		float time;
	}XAnimNotifyInfo;

	struct XAnimNotify_s
	{
		const char* name;
		unsigned int type;
		float timeFrac;
	};

	/* 6966 */
#pragma pack(push, 4)
	typedef struct
	{
		const char* name;
		uint16_t dataByteCount;
		uint16_t dataShortCount;
		uint16_t dataIntCount;
		uint16_t randomDataByteCount;
		uint16_t randomDataIntCount;
		uint16_t numframes;
		char boneCount[12];
		char notifyCount;
		char assetType;
		short pad;
		unsigned int randomDataShortCount;
		unsigned int indexCount;
		float framerate;
		float frequency;
		uint16_t* names;
		char* dataByte;
		int16_t* dataShort;
		int* dataInt;
		int16_t* randomDataShort;
		char* randomDataByte;
		int* randomDataInt;
		XAnimIndices indices;
		XAnimNotifyInfo* notify;
		XAnimDeltaPart* deltaPart;
	}XAnimParts;
#pragma pack(pop)


	/* 7069 */
	typedef struct
	{
		uint16_t flags;
		uint16_t children;
	}XAnimParent;

	/* 7070 */
	typedef union
	{
		XAnimParts* parts;
		XAnimParent animParent;
	}XAnimEntryUnion;

	/* 7071 */
	typedef struct
	{
		uint16_t numAnims;
		uint16_t parent;
		XAnimEntryUnion u;
	}XAnimEntry;

	typedef struct  //Dynamic size
	{
		const char* debugName; // 0
		unsigned int size; // 4
		const char** debugAnimNames; //8
		XAnimEntry entries[1]; //Attention: Dynamic array size
	}XAnim_s;


	struct __declspec(align(4)) XAnimTree_s
	{
		XAnim_s* anims;
		int info_usage;
		volatile int calcRefCount;
		volatile int modifyRefCount;
		unsigned __int16 children;
	};

#pragma pack(push, 4)
	typedef struct
	{
		float time;
		float oldTime;
		int16_t cycleCount;
		int16_t oldCycleCount;
		float goalTime;
		float goalWeight;
		float weight;
		float rate;
		byte instantWeightChange;
		byte pad[3];
	}XAnimState;
#pragma pack(pop)

	typedef union
	{
		XAnimParts* parts;
		XAnimParent animParent;
	}XAnimInfoUnion;

	/* 7354 */
	typedef struct
	{
		uint16_t notifyChild;
		int16_t notifyIndex;
		uint16_t notifyName;
		uint16_t notifyType;
		uint16_t prev;
		uint16_t next;
		uint16_t children;
		uint16_t parent;
		uint16_t animIndex;
		uint16_t animToModel;
		XAnimInfoUnion u;
		XAnimState state;
	}XAnimInfo;

	typedef enum
	{
		IT_BAD = 0x0,
		IT_WEAPON = 0x1,
	}itemType_t;

	typedef struct
	{
		itemType_t giType;
	}gitem_s;

	struct DObj_s
	{
		XAnimTree_s* tree;
		unsigned __int16 duplicateParts;
		unsigned __int16 entnum;
		char duplicatePartsSize;
		char numModels;
		char numBones;
		unsigned int ignoreCollision;
		volatile int locked;
		DSkel skel;
		float radius;
		unsigned int hidePartBits[4];
		XModel** models;
	};

	struct weaponInfo_s
	{
		DObj_s* viewModelDObj;
		XModel* handModel;
		XModel* gogglesModel;
		XModel* rocketModel;
		XModel* knifeModel;
		char weapModelIdx;
		int partBits[4];
		int iPrevAnim;
		int hasAnimTree;
		XAnimTree_s* tree;
		int registered;
		gitem_s* item;
		const char* translatedDisplayName;
		const char* translatedModename;
		const char* translatedAIOverlayDescription;
	}weaponInfo_t;


	struct GfxSceneModel
	{
		XModelDrawInfo info;
		XModel* model;
		DObj_s* obj;
		GfxScaledPlacement placement;
		unsigned __int32 gfxEntIndex : 7;
		unsigned __int32 entnum : 12;
		unsigned __int32 renderFxFlags : 13;
		float radius;
		unsigned __int16* cachedLightingHandle;
		float lightingOrigin[3];
		char reflectionProbeIndex;
		char lod;
	};


#pragma pack(push, 4)
	struct __declspec(align(4)) RefString
	{
		union
		{
			struct
			{
				unsigned int refCount : 16;
				unsigned int user : 8;
				unsigned int byteLen : 8;
			};
			volatile int data;
		};
		char str[1];
	};
#pragma pack(pop)

	enum DemoType
	{
		DEMO_TYPE_NONE = 0x0,
		DEMO_TYPE_CLIENT = 0x1,
		DEMO_TYPE_SERVER = 0x2,
	};

	enum CubemapShot
	{
		CUBEMAPSHOT_NONE = 0x0,
		CUBEMAPSHOT_RIGHT = 0x1,
		CUBEMAPSHOT_LEFT = 0x2,
		CUBEMAPSHOT_BACK = 0x3,
		CUBEMAPSHOT_FRONT = 0x4,
		CUBEMAPSHOT_UP = 0x5,
		CUBEMAPSHOT_DOWN = 0x6,
		CUBEMAPSHOT_COUNT = 0x7,
	};

	struct snapshot_s
	{
		int snapFlags;
		//int ping;
		int serverTime;
		playerState_s ps;
		int numEntities;
		int numClients;
		entityState_s entities[512];
		//clientState_s clients[64];
		int serverCommandSequence;
	};

	typedef struct
	{
		Material* whiteMaterial;
		Material* friendlyFireMaterial;
		Material* tracerMaterial;
		Material* laserMaterial;
		Material* laserLightMaterial;
		Material* hintMaterials[133];
		Material* stanceMaterials[4];
		Material* objectiveMaterials[2];
		Material* damageMaterial;
		Material* mantleHint;
		Font_s* smallDevFont;
		Font_s* bigDevFont;
		snd_alias_list_t* landDmgSound;
		snd_alias_list_t* grenadeExplodeSound[29];
		snd_alias_list_t* rocketExplodeSound[29];
		snd_alias_list_t* bulletHitSmallSound[29];
		snd_alias_list_t* bulletHitLargeSound[29];
		snd_alias_list_t* bulletHitAPSound[29];
		snd_alias_list_t* shotgunHitSound[29];
		snd_alias_list_t* bulletExitSmallSound[29];
		snd_alias_list_t* bulletExitLargeSound[29];
		snd_alias_list_t* bulletExitAPSound[29];
		snd_alias_list_t* shotgunExitSound[29];
		snd_alias_list_t* stepSprintSound[29];
		snd_alias_list_t* stepSprintSoundPlayer[29];
		snd_alias_list_t* stepRunSound[29];
		snd_alias_list_t* stepRunSoundPlayer[29];
		snd_alias_list_t* stepWalkSound[29];
		snd_alias_list_t* stepWalkSoundPlayer[29];
		snd_alias_list_t* stepProneSound[29];
		snd_alias_list_t* stepProneSoundPlayer[29];
		snd_alias_list_t* landSound[29];
		snd_alias_list_t* landSoundPlayer[29];
		snd_alias_list_t* sprintingEquipmentSound;
		snd_alias_list_t* sprintingEquipmentSoundPlayer;
		snd_alias_list_t* runningEquipmentSound;
		snd_alias_list_t* runningEquipmentSoundPlayer;
		snd_alias_list_t* walkingEquipmentSound;
		snd_alias_list_t* walkingEquipmentSoundPlayer;
		snd_alias_list_t* foliageMovement;
		snd_alias_list_t* bulletWhizby;
		snd_alias_list_t* meleeHit;
		snd_alias_list_t* meleeHitOther;
		snd_alias_list_t* meleeKnifeHit;
		snd_alias_list_t* meleeKnifeHitOther;
		snd_alias_list_t* nightVisionOn;
		snd_alias_list_t* nightVisionOff;
		snd_alias_list_t* playerHeartBeatSound;
		snd_alias_list_t* playerBreathInSound;
		snd_alias_list_t* playerBreathOutSound;
		snd_alias_list_t* playerBreathGaspSound;
		snd_alias_list_t* playerSwapOffhand;
		snd_alias_list_t* physCollisionSound[50][29];
		Material* checkbox_active;
		Material* checkbox_current;
		Material* checkbox_done;
		Material* checkbox_fail;
		Material* compassping_friendlyfiring;
		Material* compassping_friendlyyelling;
		Material* compassping_enemyfiring;
		Material* compassping_enemyyelling;
		Material* compassping_grenade;
		Material* compassping_explosion;
		Material* grenadeIconFrag;
		Material* grenadeIconFlash;
		Material* grenadeIconThrowBack;
		Material* grenadePointer;
		Material* offscreenObjectivePointer;
		FxImpactTable* fx;
		const FxEffectDef* fxNoBloodFleshHit;
		const FxEffectDef* fxKnifeBlood;
		const FxEffectDef* fxKnifeNoBlood;
		Material* vehCenterCircle;
		Material* vehMovingCircle;
		Material* vehHudLine;
		Material* vehBouncingDiamondReticle;
		Material* compassFovMaterial;
		XModel* nightVisionGoggles;
		Material* nightVisionOverlay;
		Material* hudIconNVG;
		Material* hudDpadArrow;
		Material* ammoCounterBullet;
		Material* ammoCounterBeltBullet;
		Material* ammoCounterRifleBullet;
		Material* ammoCounterRocket;
		Material* ammoCounterShotgunShell;
		Material* textDecodeCharacters;
		Material* textDecodeCharactersGlow;
	}cgMedia_t;

	/*
	struct cg_s
	{
		int clientNum;
		int localClientNum;
		int nextState;
		DemoType demoType;
		CubemapShot cubemapShot;
		int renderScreen;
		int latestSnapshotNum;
		int latestSnapshotTime;
		snapshot_s* snap;
		snapshot_s* nextSnap;
		snapshot_s activeSnapshots[2];
		float frameInterpolation;
		int frametime;
		int time;
		int oldTime;
		int physicsTime;
		int mapRestart;
		//int renderingThirdPerson;
		playerState_s predictedPlayerState;
	};
	*/

	struct playerEntity_t
	{
		float fLastWeaponPosFrac;
		int bPositionToADS;
		float fOOPositionBlendTime;
		float vPositionBlendOrg[3];
		float vPositionBlendAng[3];
		float vPositionLastOrg[3];
		float vPositionLastAng[3];
		float fLastIdleFactor;
		float vLastMoveOrg[3];
		float vLastMoveAng[3];
	};

	struct GfxDepthOfField
	{
		float viewModelStart;
		float viewModelEnd;
		float nearStart;
		float nearEnd;
		float farStart;
		float farEnd;
		float nearBlur;
		float farBlur;
	};

	struct GfxFilm
	{
		bool enabled;
		float brightness;
		float contrast;
		float desaturation;
		bool invert;
		float tintDark[3];
		float tintLight[3];
	};

	struct GfxGlow
	{
		bool enabled;
		float bloomCutoff;
		float bloomDesaturation;
		float bloomIntensity;
		float radius;
	};

	struct GfxViewport
	{
		int x;
		int y;
		int width;
		int height;
	};

	struct refdef_s
	{
		unsigned int x;
		unsigned int y;
		unsigned int width;
		unsigned int height;
		float tanHalfFovX;
		float tanHalfFovY;
		float vieworg[3];
		float viewaxis[3][3];
		float viewOffset[3];
		int time;
		float zNear;
		float blurRadius;
		GfxDepthOfField dof;
		GfxFilm film;
		GfxGlow glow;
		GfxLight primaryLights[255];
		GfxViewport scissorViewport;
		bool useScissorViewport;
		int localClientNum;
	};

	struct viewDamage_t
	{
		int time;
		int duration;
		float yaw;
	};

	enum objectiveState_t
	{
		OBJST_EMPTY = 0x0,
		OBJST_ACTIVE = 0x1,
		OBJST_INVISIBLE = 0x2,
		OBJST_DONE = 0x3,
		OBJST_CURRENT = 0x4,
		OBJST_FAILED = 0x5,
		OBJST_NUMSTATES = 0x6,
	};

	//from cod3sp
	struct objectiveInfo_t
	{
		objectiveState_t state;
		float origin[8][3];
		char string[1024];
		int ringTime;
		int ringToggle;
		int icon;
	};

	struct targetInfo_t
	{
		int entNum;
		float offset[3];
		int materialIndex;
		int offscreenMaterialIndex;
		int flags;
	};

	enum ShockViewTypes
	{
		SHELLSHOCK_VIEWTYPE_BLURRED = 0x0,
		SHELLSHOCK_VIEWTYPE_FLASHED = 0x1,
		SHELLSHOCK_VIEWTYPE_NONE = 0x2,
	};

	struct shellshock_screenBlend_t
	{
		int blurredFadeTime;
		int blurredEffectTime;
		int flashWhiteFadeTime;
		int flashShotFadeTime;
		ShockViewTypes type;
	};

	struct shellshock_view_t
	{
		int fadeTime;
		float kickRate;
		float kickRadius;
	};

	struct __declspec(align(4)) shellshock_sound_t
	{
		byte affect;
		byte pad[3];
		char loop[64];
		char loopSilent[64];
		char end[64];
		char endAbort[64];
		int fadeInTime;
		int fadeOutTime;
		float drylevel;
		float wetlevel;
		char roomtype[16];
		float channelvolume[64];
		int modEndDelay;
		int loopFadeTime;
		int loopEndDelay;
	};

	struct shellshock_movement_t
	{
		byte affect;
		byte pad[3];
	};

	struct __declspec(align(4)) shellshock_lookControl_t
	{
		byte affect;
		byte pad[3];
		int fadeTime;
		float mouseSensitivity;
		float maxPitchSpeed;
		float maxYawSpeed;
	};

	struct __declspec(align(4)) shellshock_parms_t
	{
		struct shellshock_screenBlend_t screenBlend;
		struct shellshock_view_t view;
		struct shellshock_sound_t sound;
		struct shellshock_lookControl_t lookControl;
		struct shellshock_movement_t movement;
	};

	struct shellshock_t
	{
		const shellshock_parms_t* parms;
		int startTime;
		int duration;
		int loopEndTime;
		float sensitivity;
		float viewDelta[2];
		int hasSavedScreen;
	};

	struct cgShockinfo_t
	{
		int time;
		int duration;
	};

	struct visionSetVars_t
	{
		bool glowEnable;
		float glowBloomCutoff;
		float glowBloomDesaturation;
		float glowBloomIntensity0;
		float glowBloomIntensity1;
		float glowRadius0;
		float glowRadius1;
		float glowSkyBleedIntensity0;
		float glowSkyBleedIntensity1;
		bool filmEnable;
		float filmBrightness;
		float filmContrast;
		float filmDesaturation;
		bool filmInvert;
		float filmLightTint[3];
		float filmDarkTint[3];
	};

	enum visionSetLerpStyle_t
	{
		VISIONSETLERP_UNDEFINED = 0x0,
		VISIONSETLERP_NONE = 0x1,
		VISIONSETLERP_TO_LINEAR = 0x2,
		VISIONSETLERP_TO_SMOOTH = 0x3,
		VISIONSETLERP_BACKFORTH_LINEAR = 0x4,
		VISIONSETLERP_BACKFORTH_SMOOTH = 0x5,
	};

	struct visionSetLerpData_t
	{
		int timeStart;
		int timeDuration;
		visionSetLerpStyle_t style;
	};

	struct hudElemSoundInfo_t
	{
		int lastPlayedTime;
	};

	struct cg_s
	{
		int clientNum;
		int localClientNum;
		DemoType demoType;
		CubemapShot cubemapShot;
		int cubemapSize;
		int serverCommandSequence;
		int latestSnapshotNum;
		int latestSnapshotTime;
		int loaded;
		snapshot_s* snap;
		snapshot_s* nextSnap;
		snapshot_s activeSnapshots[2];
		int createdNextSnap;
		float frameInterpolation;
		int frametime;
		int animFrametime;
		int time;
		int oldTime;
		int physicsTime;
		playerState_s predictedPlayerState;
		centity_s predictedPlayerEntity;
		char gap0[12];
		playerEntity_t playerEntity;
		int validPPS;
		int predictedErrorTime;
		float predictedError[3];
		float landChange;
		int landTime;
		float heightToCeiling;
		refdef_s refdef;
		float refdefViewAngles[3];
		float lastVieworg[3];
		float swayViewAngles[3];
		float swayAngles[3];
		float swayOffset[3];
		float zoomSensitivity;
		int vehicleInitView;
		float prevVehicleInvAxis[3][3];
		bool vehicleViewLocked;
		float vehicleViewLockedAngles[3];
		int showScores;
		int scoreFadeTime;
		int timeScaleTimeStart;
		int timeScaleTimeEnd;
		float timeScaleStart;
		float timeScaleEnd;
		int deadquoteStartTime;
		int cursorHintIcon;
		int cursorHintTime;
		int cursorHintFade;
		int cursorHintString;
		int lastClipFlashTime;
		int invalidCmdHintType;
		int invalidCmdHintTime;
		int lastHealthPulseTime;
		int lastHealthLerpDelay;
		int lastHealthClient;
		float lastHealth;
		float healthOverlayFromAlpha;
		float healthOverlayToAlpha;
		int healthOverlayPulseTime;
		int healthOverlayPulseDuration;
		int healthOverlayPulsePhase;
		bool healthOverlayHurt;
		int proneBlockedEndTime;
		int lastStance;
		int lastStanceChangeTime;
		int lastStanceFlashTime;
		int voiceTime;
		unsigned int weaponSelect;
		int weaponSelectTime;
		unsigned int weaponLatestPrimaryIdx;
		int prevViewmodelWeapon;
		int equippedOffHand;
		viewDamage_t viewDamage[8];
		int damageTime;
		float damageX;
		float damageY;
		float damageValue;
		int weapIdleTime;
		int nomarks;
		int v_dmg_time;
		float v_dmg_pitch;
		float v_dmg_roll;
		float fBobCycle;
		float xyspeed;
		float kickAVel[3];
		float kickAngles[3];
		float gunPitch;
		float gunYaw;
		float gunXOfs;
		float gunYOfs;
		float gunZOfs;
		float vGunOffset[3];
		float vGunSpeed[3];
		float vAngOfs[3];
		float viewModelAxis[4][3];
		bool hideViewModel;
		float rumbleScale;
		float selectedLocation[2];
		float compassNorthYaw;
		float compassNorth[2];
		int compassMapMaterial;
		float compassMapUpperLeft[2];
		float compassMapWorldSize[2];
		int compassFadeTime;
		int healthFadeTime;
		char gap1[4];
		int ammoFadeTime;
		int stanceFadeTime;
		int sprintFadeTime;
		int offhandFadeTime;
		int offhandFlashTime;
		objectiveInfo_t objectives[16];
		targetInfo_t targets[32];
		shellshock_t shellshock;
		cgShockinfo_t testShock;
		int holdBreathTime;
		int holdBreathInTime;
		int holdBreathDelay;
		float holdBreathFrac;
		int bloodLastTime;
		float vehReticleOffset[2];
		float vehReticleVel[2];
		int vehReticleLockOnStartTime;
		int vehReticleLockOnDuration;
		int vehReticleLockOnEntNum;
		cpose_t viewModelPose;
		visionSetVars_t visionSetPreLoaded[4];
		char visionSetPreLoadedName[4][64];
		visionSetVars_t visionSetFrom[2];
		visionSetVars_t visionSetTo[2];
		visionSetVars_t visionSetCurrent[2];
		visionSetLerpData_t visionSetLerpData[2];
		char visionNameNaked[64];
		char visionNameNight[64];
		int extraButtons;
		int lastActionSlotTime;
		bool playerTeleported;
		int stepViewStart;
		float stepViewChange;
		float zNear;
		hudElemSoundInfo_t hudElemSound[32];
		char gap2[16];
	};

	enum svscmd_type
	{
		SV_CMD_CAN_IGNORE = 0x0,
		SV_CMD_RELIABLE = 0x1,
	};

	struct cStaticModelWritable
	{
		unsigned __int16 nextModelInWorldSector;
	};

	struct cStaticModel_s
	{
		cStaticModelWritable writable;
		XModel* xmodel;
		float origin[3];
		float invScaledAxis[3][3];
		float absmin[3];
		float absmax[3];
	};

	struct dmaterial_t
	{
		char material[64];
		int surfaceFlags;
		int contentFlags;
	};

	struct cNode_t
	{
		cplane_s* plane;
		__int16 children[2];
	};

#pragma pack(push, 4)
	struct cLeaf_t
	{
		unsigned __int16 firstCollAabbIndex;
		unsigned __int16 collAabbCount;
		int brushContents;
		int terrainContents;
		float mins[3];
		float maxs[3];
		int leafBrushNode;
		__int16 cluster;
	};
#pragma pack(pop)

	struct cLeafBrushNodeLeaf_t
	{
		unsigned __int16* brushes;
	};

	struct cLeafBrushNodeChildren_t
	{
		float dist;
		float range;
		unsigned __int16 childOffset[2];
	};

	union cLeafBrushNodeData_t
	{
		cLeafBrushNodeLeaf_t leaf;
		cLeafBrushNodeChildren_t children;
	};

	struct cLeafBrushNode_s
	{
		char axis;
		__int16 leafBrushCount;
		int contents;
		cLeafBrushNodeData_t data;
	};

	struct CollisionBorder
	{
		float distEq[3];
		float zBase;
		float zSlope;
		float start;
		float length;
	};

	struct CollisionPartition
	{
		char triCount;
		char borderCount;
		int firstTri;
		CollisionBorder* borders;
	};

	union CollisionAabbTreeIndex
	{
		int firstChildIndex;
		int partitionIndex;
	};

	struct CollisionAabbTree
	{
		float origin[3];
		float halfSize[3];
		unsigned __int16 materialIndex;
		unsigned __int16 childCount;
		CollisionAabbTreeIndex u;
	};

	/* 860 */
	struct cmodel_t
	{
		float mins[3];
		float maxs[3];
		float radius;
		cLeaf_t leaf;
	};

	//		/* 861 */
	//#pragma pack(push, 16)
	//		struct cbrush_t
	//		{
	//			float mins[3];
	//			int contents;
	//			float maxs[3];
	//			unsigned int numsides;
	//			cbrushside_t *sides;
	//			__int16 axialMaterialNum[2][3];
	//			char *baseAdjacentSide;
	//			__int16 firstAdjacentSideOffsets[2][3];
	//			char edgeCount[2][3];
	//			char pad[8];
	//		};
	//#pragma pack(pop)


	//
	//		/* 861 */
	//#pragma pack(push, 16)
	//		struct cbrush_collision_t
	//		{
	//			float mins[3];
	//			int contents;
	//			float maxs[3];
	//			unsigned int numsides;
	//			cbrushside_t *sides;
	//			__int16 axialMaterialNum[2][3];
	//			char *baseAdjacentSide;
	//			__int16 firstAdjacentSideOffsets[2][3];
	//			char edgeCount[2][3];
	//			int colorCounter;
	//			char pad[4];
	//		};
	//#pragma pack(pop)

	struct TriggerModel
	{
		int contents;
		unsigned __int16 hullCount;
		unsigned __int16 firstHull;
	};

	/* 2376 */
	struct TriggerHull
	{
		Bounds bounds;
		int contents;
		unsigned __int16 slabCount;
		unsigned __int16 firstSlab;
	};

	/* 2377 */
	struct TriggerSlab
	{
		float dir[3];
		float midPoint;
		float halfSize;
	};

	/* 2378 */
	struct MapTriggers
	{
		unsigned int count;
		TriggerModel* models;
		unsigned int hullCount;
		TriggerHull* hulls;
		unsigned int slabCount;
		TriggerSlab* slabs;
	};

	struct MapEnts
	{
		const char* name;
		const char* entityString;
		int numEntityChars;
		MapTriggers trigger;
		// this goes on for a while but we don't need any of it
	};

	struct FxEffectDef_Placeholder
	{
		const char* name;
	};

	struct DynEntityDef
	{
		int type;
		GfxPlacement pose;
		XModel* xModel;
		unsigned __int16 brushModel;
		unsigned __int16 physicsBrushModel;
		FxEffectDef_Placeholder* destroyFx;
		/*XModelPieces*/ void* destroyPieces;
		PhysPreset* physPreset;
		int health;
		PhysMass mass;
		int contents;
	};

	struct clipMap_t
	{
		const char* name;
		int isInUse;
		int planeCount;
		cplane_s* planes;
		unsigned int numStaticModels;
		cStaticModel_s* staticModelList;
		unsigned int numMaterials;
		dmaterial_t* materials;
		unsigned int numBrushSides;
		cbrushside_t* brushsides;
		unsigned int numBrushEdges;
		char* brushEdges;
		unsigned int numNodes;
		cNode_t* nodes;
		unsigned int numLeafs;
		cLeaf_t* leafs;
		unsigned int leafbrushNodesCount;
		cLeafBrushNode_s* leafbrushNodes;
		unsigned int numLeafBrushes;
		unsigned __int16* leafbrushes;
		unsigned int numLeafSurfaces;
		unsigned int* leafsurfaces;
		unsigned int vertCount;
		float(*verts)[3];
		int triCount;
		unsigned __int16* triIndices;
		char* triEdgeIsWalkable;
		int borderCount;
		CollisionBorder* borders;
		int partitionCount;
		CollisionPartition* partitions;
		int aabbTreeCount;
		CollisionAabbTree* aabbTrees;
		unsigned int numSubModels;
		cmodel_t* cmodels;
		unsigned __int16 numBrushes;
		cbrush_t* brushes;
		int numClusters;
		int clusterBytes;
		char* visibility;
		int vised;
		MapEnts* mapEnts;
		cbrush_t* box_brush;
		cmodel_t box_model;
		unsigned __int16 dynEntCount[2];
		DynEntityDef* dynEntDefList[2];
		/*DynEntityPose*/ void* dynEntPoseList[2];
		/*DynEntityClient*/ void* dynEntClientList[2];
		/*DynEntityColl*/ void* dynEntCollList[2];
		unsigned int checksum;
	};


	struct localization_t
	{
		const char* language;
		const char* strings;
	};

	struct fileInIwd_s
	{
		unsigned int pos;
		char* name;
		fileInIwd_s* next;
	};

	struct iwd_t
	{
		char iwdFilename[256];
		char iwdBasename[256];
		char iwdGamename[256];
		unsigned __int8* handle;
		int checksum;
		int pure_checksum;
		volatile int hasOpenFile;
		int numFiles;
		unsigned __int8 referenced;
		unsigned int hashSize;
		fileInIwd_s** hashTable;
		fileInIwd_s* buildBuffer;
	};

	struct directory_t
	{
		char path[256];
		char gamedir[256];
	};

	struct searchpath_s
	{
		searchpath_s* next;
		iwd_t* iwd;
		directory_t* dir;
		int bLocalized;
		int ignore;
		int ignorePureCheck;
		int language;
	};

	struct qtime_s
	{
		int tm_sec;
		int tm_min;
		int tm_hour;
		int tm_mday;
		int tm_mon;
		int tm_year;
		int tm_wday;
		int tm_yday;
		int tm_isdst;
	};

	struct SavegameInfo
	{
		const char* savegameFile;
		const char* savegameName;
		const char* imageName;
		const char* mapName;
		const char* savegameInfoText;
		const char* time;
		const char* date;
		qtime_s tm;
	};


	struct CursorPos
	{
		float x;
		float y;
	};

	enum UILocalVarType
	{
		UILOCALVAR_INT = 0x0,
		UILOCALVAR_FLOAT = 0x1,
		UILOCALVAR_STRING = 0x2,
	};

	union UILocalVarHandle
	{
		int integer;
		float value;
		const char* string;
	};

	struct UILocalVar
	{
		UILocalVarType type;
		const char* name;
		UILocalVarHandle u;
	};

	struct UILocalVarContext
	{
		UILocalVar table[256];
	};

	struct UiContext
	{
		int localClientNum;
		float bias;
		int realTime;
		int frameTime;
		CursorPos cursor;
		int isCursorVisible;
		int screenWidth;
		int screenHeight;
		float screenAspect;
		float FPS;
		float blurRadiusOut;
		menuDef_t* Menus[640];
		int menuCount;
		menuDef_t* menuStack[16];
		int openMenuCount;
		UILocalVarContext localVars;
	};

	struct savegameStatus_s
	{
		int sortKey;
		int sortDir;
		int displaySavegames[256];
	};

	struct uiInfo_s
	{
		UiContext uiDC;
		SavegameInfo savegameList[512];
		int savegameCount;
		savegameStatus_s savegameStatus;
		int timeIndex;
		int previousTimes[4];
		bool allowScriptMenuResponse;
		char savegameName[64];
		char savegameInfo[256];
		Material* sshotImage;
		char sshotImageName[64];
	};

	enum GfxDrawSceneMethod
	{
		GFX_DRAW_SCENE_NONE = 0x0,
		GFX_DRAW_SCENE_FULLBRIGHT = 0x1,
		GFX_DRAW_SCENE_DEBUGSHADER = 0x2,
		GFX_DRAW_SCENE_STANDARD = 0x3,
	};

	enum MaterialTechniqueType
	{
		TECHNIQUE_DEPTH_PREPASS = 0x0,
		TECHNIQUE_BUILD_FLOAT_Z = 0x1,
		TECHNIQUE_BUILD_SHADOWMAP_DEPTH = 0x2,
		TECHNIQUE_BUILD_SHADOWMAP_COLOR = 0x3,
		TECHNIQUE_UNLIT = 0x4,
		TECHNIQUE_EMISSIVE = 0x5,
		TECHNIQUE_EMISSIVE_SHADOW = 0x6,
		TECHNIQUE_LIT_BEGIN = 0x7,
		TECHNIQUE_LIT = 0x7,
		TECHNIQUE_LIT_SUN = 0x8,
		TECHNIQUE_LIT_SUN_SHADOW = 0x9,
		TECHNIQUE_LIT_SPOT = 0xA,
		TECHNIQUE_LIT_SPOT_SHADOW = 0xB,
		TECHNIQUE_LIT_OMNI = 0xC,
		TECHNIQUE_LIT_OMNI_SHADOW = 0xD,
		TECHNIQUE_LIT_END = 0xE,
		TECHNIQUE_LIGHT_SPOT = 0xE,
		TECHNIQUE_LIGHT_OMNI = 0xF,
		TECHNIQUE_LIGHT_SPOT_SHADOW = 0x10,
		TECHNIQUE_FAKELIGHT_NORMAL = 0x11,
		TECHNIQUE_FAKELIGHT_VIEW = 0x12,
		TECHNIQUE_SUNLIGHT_PREVIEW = 0x13,
		TECHNIQUE_CASE_TEXTURE = 0x14,
		TECHNIQUE_WIREFRAME_SOLID = 0x15,
		TECHNIQUE_WIREFRAME_SHADED = 0x16,
		TECHNIQUE_SHADOWCOOKIE_CASTER = 0x17,
		TECHNIQUE_SHADOWCOOKIE_RECEIVER = 0x18,
		TECHNIQUE_DEBUG_BUMPMAP = 0x19,
		TECHNIQUE_COUNT = 0x1A,
		TECHNIQUE_TOTAL_COUNT = 0x1B,
		TECHNIQUE_NONE = 0x1C,
	};

	struct GfxDrawMethod
	{
		GfxDrawSceneMethod drawScene;
		MaterialTechniqueType baseTechType;
		MaterialTechniqueType emissiveTechType;
		unsigned __int8 litTechType[8][7];
	};

	enum HeFont
	{
		HE_FONT_DEFAULT = 0x0,
		HE_FONT_BIGFIXED = 0x1,
		HE_FONT_SMALLFIXED = 0x2,
		HE_FONT_OBJECTIVE = 0x3,
		HE_FONT_BIG = 0x4,
		HE_FONT_SMALL = 0x5,
		HE_FONT_CONSOLE = 0x6,
		HE_FONT_COUNT = 0x7,
	};

	struct VariableStackBuffer
	{
		const char* pos;
		unsigned __int16 size;
		unsigned __int16 bufLen;
		unsigned __int16 localId;
		char time;
		char buf[1];
	};

	enum
	{
		VAR_UNDEFINED = 0x0,
		VAR_BEGIN_REF = 0x1,
		VAR_POINTER = 0x1,
		VAR_STRING = 0x2,
		VAR_ISTRING = 0x3,
		VAR_VECTOR = 0x4,
		VAR_END_REF = 0x5,
		VAR_FLOAT = 0x5,
		VAR_INTEGER = 0x6,
		VAR_CODEPOS = 0x7,
		VAR_PRECODEPOS = 0x8,
		VAR_FUNCTION = 0x9,
		VAR_STACK = 0xA,
		VAR_ANIMATION = 0xB,
		VAR_DEVELOPER_CODEPOS = 0xC,
		VAR_INCLUDE_CODEPOS = 0xD,
		VAR_THREAD = 0xE,
		VAR_NOTIFY_THREAD = 0xF,
		VAR_TIME_THREAD = 0x10,
		VAR_CHILD_THREAD = 0x11,
		VAR_OBJECT = 0x12,
		VAR_DEAD_ENTITY = 0x13,
		VAR_ENTITY = 0x14,
		VAR_ARRAY = 0x15,
		VAR_DEAD_THREAD = 0x16,
		VAR_COUNT = 0x17,
		VAR_THREAD_LIST = 0x18,
		VAR_ENDON_LIST = 0x19,
	};

	enum
	{
		FIRST_OBJECT = 0xE,
		FIRST_CLEARABLE_OBJECT = 0x12,
		LAST_NONENTITY_OBJECT = 0x12,
		FIRST_ENTITY_OBJECT = 0x14,
		FIRST_NONFIELD_OBJECT = 0x15,
		FIRST_DEAD_OBJECT = 0x16,
	};

	union VariableUnion
	{
		int intValue;
		float floatValue;
		unsigned int stringValue;
		const float* vectorValue;
		const char* codePosValue;
		unsigned int pointerValue;
		VariableStackBuffer* stackValue;
		unsigned int entityOffset;
	};

	struct VariableValue
	{
		VariableUnion u;
		int type;
	};

	struct function_stack_t
	{
		const char* pos;
		unsigned int localId;
		unsigned int localVarCount;
		VariableValue* top;
		VariableValue* startTop;
	};

	struct function_frame_t
	{
		function_stack_t fs;
		int topType;
	};

	struct scrVmPub_t
	{
		unsigned int* localVars;
		VariableValue* maxstack;
		int function_count;
		function_frame_t* function_frame;
		VariableValue* top;
		bool debugCode;
		bool abort_on_error;
		bool terminal_error;
		unsigned int inparamcount;
		unsigned int outparamcount;
		unsigned int breakpointOutparamcount;
		bool showError;
		function_frame_t function_frame_start[32];
		VariableValue stack[2048];
	};

	enum conChannel_t
	{
		CON_CHANNEL_DONT_FILTER = 0x0,
		CON_CHANNEL_ERROR = 0x1,
		CON_CHANNEL_GAMENOTIFY = 0x2,
		CON_CHANNEL_BOLDGAME = 0x3,
		CON_CHANNEL_SUBTITLE = 0x4,
		CON_CHANNEL_OBITUARY = 0x5,
		CON_CHANNEL_LOGFILEONLY = 0x6,
		CON_CHANNEL_CONSOLEONLY = 0x7,
		CON_CHANNEL_GFX = 0x8,
		CON_CHANNEL_SOUND = 0x9,
		CON_CHANNEL_FILES = 0xA,
		CON_CHANNEL_DEVGUI = 0xB,
		CON_CHANNEL_PROFILE = 0xC,
		CON_CHANNEL_UI = 0xD,
		CON_CHANNEL_CLIENT = 0xE,
		CON_CHANNEL_SERVER = 0xF,
		CON_CHANNEL_SYSTEM = 0x10,
		CON_CHANNEL_PLAYERWEAP = 0x11,
		CON_CHANNEL_AI = 0x12,
		CON_CHANNEL_ANIM = 0x13,
		CON_CHANNEL_PHYS = 0x14,
		CON_CHANNEL_FX = 0x15,
		CON_CHANNEL_LEADERBOARDS = 0x16,
		CON_CHANNEL_PARSERSCRIPT = 0x17,
		CON_CHANNEL_SCRIPT = 0x18,
		CON_BUILTIN_CHANNEL_COUNT = 0x19,
	};

	//gamepad
	struct keyname_t
	{
		const char* name;
		int keynum;
	};

	enum KeyCatch_t
	{
		KEYCATCH_MASK_ANY = -1,
		KEYCATCH_CONSOLE = 0x1,
		KEYCATCH_LOCATION_SELECTION = 0x8,
		KEYCATCH_UI = 0x10,
		KEYCATCH_NUMERIC = 0x11,
	};

	enum keyNum_t
	{
		K_NONE = 0x0,
		K_FIRSTGAMEPADBUTTON_RANGE_1 = 0x1,
		K_BUTTON_A = 0x1,
		K_BUTTON_B = 0x2,
		K_BUTTON_X = 0x3,
		K_BUTTON_Y = 0x4,
		K_BUTTON_LSHLDR = 0x5,
		K_BUTTON_RSHLDR = 0x6,
		K_LASTGAMEPADBUTTON_RANGE_1 = 0x6,
		K_TAB = 0x9,
		K_ENTER = 0xD,
		K_FIRSTGAMEPADBUTTON_RANGE_2 = 0xE,
		K_BUTTON_START = 0xE,
		K_BUTTON_BACK = 0xF,
		K_BUTTON_LSTICK = 0x10,
		K_BUTTON_RSTICK = 0x11,
		K_BUTTON_LTRIG = 0x12,
		K_BUTTON_RTRIG = 0x13,
		K_DPAD_UP = 0x14,
		K_FIRSTDPAD = 0x14,
		K_DPAD_DOWN = 0x15,
		K_DPAD_LEFT = 0x16,
		K_DPAD_RIGHT = 0x17,
		K_BUTTON_LSTICK_ALTIMAGE = 0xBC,
		K_BUTTON_RSTICK_ALTIMAGE = 0xBD,
		K_LASTDPAD = 0x17,
		K_LASTGAMEPADBUTTON_RANGE_2 = 0x17,
		K_ESCAPE = 0x1B,
		K_FIRSTGAMEPADBUTTON_RANGE_3 = 0x1C,
		K_APAD_UP = 0x1C,
		K_FIRSTAPAD = 0x1C,
		K_APAD_DOWN = 0x1D,
		K_APAD_LEFT = 0x1E,
		K_APAD_RIGHT = 0x1F,
		K_LASTAPAD = 0x1F,
		K_LASTGAMEPADBUTTON_RANGE_3 = 0x1F,
		K_SPACE = 0x20,
		K_BACKSPACE = 0x7F,
		K_ASCII_FIRST = 0x80,
		K_ASCII_181 = 0x80,
		K_ASCII_191 = 0x81,
		K_ASCII_223 = 0x82,
		K_ASCII_224 = 0x83,
		K_ASCII_225 = 0x84,
		K_ASCII_228 = 0x85,
		K_ASCII_229 = 0x86,
		K_ASCII_230 = 0x87,
		K_ASCII_231 = 0x88,
		K_ASCII_232 = 0x89,
		K_ASCII_233 = 0x8A,
		K_ASCII_236 = 0x8B,
		K_ASCII_241 = 0x8C,
		K_ASCII_242 = 0x8D,
		K_ASCII_243 = 0x8E,
		K_ASCII_246 = 0x8F,
		K_ASCII_248 = 0x90,
		K_ASCII_249 = 0x91,
		K_ASCII_250 = 0x92,
		K_ASCII_252 = 0x93,
		K_END_ASCII_CHARS = 0x94,
		K_COMMAND = 0x96,
		K_CAPSLOCK = 0x97,
		K_POWER = 0x98,
		K_PAUSE = 0x99,
		K_UPARROW = 0x9A,
		K_DOWNARROW = 0x9B,
		K_LEFTARROW = 0x9C,
		K_RIGHTARROW = 0x9D,
		K_ALT = 0x9E,
		K_CTRL = 0x9F,
		K_SHIFT = 0xA0,
		K_INS = 0xA1,
		K_DEL = 0xA2,
		K_PGDN = 0xA3,
		K_PGUP = 0xA4,
		K_HOME = 0xA5,
		K_END = 0xA6,
		K_F1 = 0xA7,
		K_F2 = 0xA8,
		K_F3 = 0xA9,
		K_F4 = 0xAA,
		K_F5 = 0xAB,
		K_F6 = 0xAC,
		K_F7 = 0xAD,
		K_F8 = 0xAE,
		K_F9 = 0xAF,
		K_F10 = 0xB0,
		K_F11 = 0xB1,
		K_F12 = 0xB2,
		K_F13 = 0xB3,
		K_F14 = 0xB4,
		K_F15 = 0xB5,
		K_KP_HOME = 0xB6,
		K_KP_UPARROW = 0xB7,
		K_KP_PGUP = 0xB8,
		K_KP_LEFTARROW = 0xB9,
		K_KP_5 = 0xBA,
		K_KP_RIGHTARROW = 0xBB,
		K_KP_END = 0xBC,
		K_KP_DOWNARROW = 0xBD,
		K_KP_PGDN = 0xBE,
		K_KP_ENTER = 0xBF,
		K_KP_INS = 0xC0,
		K_KP_DEL = 0xC1,
		K_KP_SLASH = 0xC2,
		K_KP_MINUS = 0xC3,
		K_KP_PLUS = 0xC4,
		K_KP_NUMLOCK = 0xC5,
		K_KP_STAR = 0xC6,
		K_KP_EQUALS = 0xC7,
		K_MOUSE1 = 0xC8,
		K_MOUSE2 = 0xC9,
		K_MOUSE3 = 0xCA,
		K_MOUSE4 = 0xCB,
		K_MOUSE5 = 0xCC,
		K_MWHEELDOWN = 0xCD,
		K_MWHEELUP = 0xCE,
		K_AUX1 = 0xCF,
		K_AUX2 = 0xD0,
		K_AUX3 = 0xD1,
		K_AUX4 = 0xD2,
		K_AUX5 = 0xD3,
		K_AUX6 = 0xD4,
		K_AUX7 = 0xD5,
		K_AUX8 = 0xD6,
		K_AUX9 = 0xD7,
		K_AUX10 = 0xD8,
		K_AUX11 = 0xD9,
		K_AUX12 = 0xDA,
		K_AUX13 = 0xDB,
		K_AUX14 = 0xDC,
		K_AUX15 = 0xDD,
		K_AUX16 = 0xDE,
		K_LAST_KEY = 0xDF,
	};

	static constexpr auto MAX_GPAD_COUNT = 1;

	static constexpr auto GPAD_VALUE_MASK = 0xFFFFFFFu;
	static constexpr auto GPAD_DPAD_MASK = XINPUT_GAMEPAD_DPAD_UP | XINPUT_GAMEPAD_DPAD_DOWN | XINPUT_GAMEPAD_DPAD_LEFT | XINPUT_GAMEPAD_DPAD_RIGHT;
	static constexpr auto GPAD_DIGITAL_MASK = 1u << 28;
	static constexpr auto GPAD_ANALOG_MASK = 1u << 29;
	static constexpr auto GPAD_STICK_MASK = 1u << 30;

	enum GamePadButton
	{
		GPAD_NONE = 0,
		GPAD_UP = GPAD_DIGITAL_MASK | (XINPUT_GAMEPAD_DPAD_UP & GPAD_VALUE_MASK),
		GPAD_DOWN = GPAD_DIGITAL_MASK | (XINPUT_GAMEPAD_DPAD_DOWN & GPAD_VALUE_MASK),
		GPAD_LEFT = GPAD_DIGITAL_MASK | (XINPUT_GAMEPAD_DPAD_LEFT & GPAD_VALUE_MASK),
		GPAD_RIGHT = GPAD_DIGITAL_MASK | (XINPUT_GAMEPAD_DPAD_RIGHT & GPAD_VALUE_MASK),
		GPAD_START = GPAD_DIGITAL_MASK | (XINPUT_GAMEPAD_START & GPAD_VALUE_MASK),
		GPAD_BACK = GPAD_DIGITAL_MASK | (XINPUT_GAMEPAD_BACK & GPAD_VALUE_MASK),
		GPAD_L3 = GPAD_DIGITAL_MASK | (XINPUT_GAMEPAD_LEFT_THUMB & GPAD_VALUE_MASK),
		GPAD_R3 = GPAD_DIGITAL_MASK | (XINPUT_GAMEPAD_RIGHT_THUMB & GPAD_VALUE_MASK),
		GPAD_L_SHLDR = GPAD_DIGITAL_MASK | (XINPUT_GAMEPAD_LEFT_SHOULDER & GPAD_VALUE_MASK),
		GPAD_R_SHLDR = GPAD_DIGITAL_MASK | (XINPUT_GAMEPAD_RIGHT_SHOULDER & GPAD_VALUE_MASK),
		GPAD_A = GPAD_DIGITAL_MASK | (XINPUT_GAMEPAD_A & GPAD_VALUE_MASK),
		GPAD_B = GPAD_DIGITAL_MASK | (XINPUT_GAMEPAD_B & GPAD_VALUE_MASK),
		GPAD_X = GPAD_DIGITAL_MASK | (XINPUT_GAMEPAD_X & GPAD_VALUE_MASK),
		GPAD_Y = GPAD_DIGITAL_MASK | (XINPUT_GAMEPAD_Y & GPAD_VALUE_MASK),
		GPAD_L_TRIG = GPAD_ANALOG_MASK | (0 & GPAD_VALUE_MASK),
		GPAD_R_TRIG = GPAD_ANALOG_MASK | (1 & GPAD_VALUE_MASK),
	};
	/*
	enum GamePadButton
	{
		GPAD_NONE = 0x0,
		GPAD_UP = 0x10000001,
		GPAD_DOWN = 0x10000002,
		GPAD_LEFT = 0x10000004,
		GPAD_RIGHT = 0x10000008,
		GPAD_START = 0x10000010,
		GPAD_BACK = 0x10000020,
		GPAD_L3 = 0x10000040,
		GPAD_R3 = 0x10000080,
		GPAD_A = 0x10001000,
		GPAD_B = 0x10002000,
		GPAD_X = 0x10004000,
		GPAD_Y = 0x10008000,
		GPAD_L_SHLDR = 0x10000100,
		GPAD_R_SHLDR = 0x10000200,
		GPAD_L_TRIG = 0x20000000,
		GPAD_R_TRIG = 0x20000001,
	};
	*/

	enum GamePadStick
	{
		GPAD_INVALID = 0x0,
		GPAD_LX = GPAD_STICK_MASK | (0 & GPAD_VALUE_MASK),
		GPAD_LY = GPAD_STICK_MASK | (1 & GPAD_VALUE_MASK),
		GPAD_RX = GPAD_STICK_MASK | (2 & GPAD_VALUE_MASK),
		GPAD_RY = GPAD_STICK_MASK | (3 & GPAD_VALUE_MASK),
	};

	/*
	enum GamePadStick
	{
		GPAD_INVALID = 0x0,
		GPAD_LX = 0x40000000,
		GPAD_LY = 0x40000001,
		GPAD_RX = 0x40000002,
		GPAD_RY = 0x40000003,
	};
	*/

	enum GamePadButtonEvent
	{
		GPAD_BUTTON_RELEASED = 0x0,
		GPAD_BUTTON_PRESSED = 0x1,
		GPAD_BUTTON_UPDATE = 0x2,
	};

	struct StickToCodeMap_t
	{
		GamePadStick padStick;
		int posCode;
		int negCode;
	};

	enum GamepadPhysicalAxis
	{
		GPAD_PHYSAXIS_RSTICK_X = 0x0,
		GPAD_PHYSAXIS_RSTICK_Y = 0x1,
		GPAD_PHYSAXIS_LSTICK_X = 0x2,
		GPAD_PHYSAXIS_LSTICK_Y = 0x3,
		GPAD_PHYSAXIS_RTRIGGER = 0x4,
		GPAD_PHYSAXIS_LTRIGGER = 0x5,
		GPAD_PHYSAXIS_COUNT = 0x6,
		GPAD_PHYSAXIS_NONE = -1, //0xFFFFFFFF
	};

	enum GamepadVirtualAxis
	{
		GPAD_VIRTAXIS_NONE = -1,
		GPAD_VIRTAXIS_SIDE = 0x0,
		GPAD_VIRTAXIS_FORWARD = 0x1,
		GPAD_VIRTAXIS_UP = 0x2,
		GPAD_VIRTAXIS_YAW = 0x3,
		GPAD_VIRTAXIS_PITCH = 0x4,
		GPAD_VIRTAXIS_ATTACK = 0x5,

		GPAD_VIRTAXIS_COUNT
	};

	enum GamePadStickDir
	{
		GPAD_STICK_POS = 0x0,
		GPAD_STICK_NEG = 0x1,
		GPAD_STICK_DIR_COUNT
	};

	enum GamepadMapping
	{
		GPAD_MAP_NONE = -1,
		GPAD_MAP_LINEAR = 0x0,
		GPAD_MAP_SQUARED = 0x1,

		GPAD_MAP_COUNT
	};

	struct ButtonToCodeMap_t
	{
		GamePadButton padButton;
		int code;
	};

	struct GamepadVirtualAxisMapping
	{
		GamepadPhysicalAxis physicalAxis;
		GamepadMapping mapType;
	};

	struct GpadAxesGlob
	{
		float axesValues[GPAD_PHYSAXIS_COUNT];
		GamepadVirtualAxisMapping virtualAxes[GPAD_VIRTAXIS_COUNT];
	};

	enum LocSelInputState
	{
		LOC_SEL_INPUT_NONE = 0x0,
		LOC_SEL_INPUT_CONFIRM = 0x1,
		LOC_SEL_INPUT_CANCEL = 0x2,
	};

	struct field_t
	{
		int cursor;
		int scroll;
		int drawWidth;
		int widthInPixels;
		float charHeight;
		int fixedSize;
		char buffer[256];
	};

	struct KeyState
	{
		int down;
		int repeats;
		const char* binding;
	};

	struct PlayerKeyState
	{
		//why is it here?
		/*field_t chatField;
		int chat_team;*/
		int overstrikeMode;
		int anyKeyDown;
		KeyState keys[256];
		LocSelInputState locSelInputState;
	};

	struct ConversionArguments
	{
		int argCount;
		const char* args[9];
	};

	enum uiMenuCommand_t
	{
		UIMENU_BACKTOGAME = 0x0,
		UIMENU_ERROR = 0x1,
		UIMENU_PAUSED = 0x2,
		UIMENU_NEEDCD = 0x3,
		UIMENU_PREGAME = 0x5,
		UIMENU_ENDGAME = 0x6,
		UIMENU_BRIEFING = 0x8,
		UIMENU_VICTORYSCREEN = 0x9,
		UIMENU_SAVEGAMELOADING = 0xC,
	};

	struct ConDrawInputGlob
	{
		char autoCompleteChoice[64];
		int matchIndex;
		int matchCount;
		const char* inputText;
		int inputTextLen;
		bool hasExactMatch;
		bool mayAutoComplete;
		float x;
		float y;
		float leftX;
		float fontHeight;
	};

	struct msg_t
	{
		int overflowed;
		int readOnly;
		unsigned __int8* data;
		unsigned __int8* splitData;
		int maxsize;
		int cursize;
		int splitSize;
		int readcount;
		int bit;
		int lastEntityRef;
	};

	enum connstate_t
	{
		CA_DISCONNECTED = 0x0,
		CA_CINEMATIC = 0x1,
		CA_LOGO = 0x2,
		CA_LOADING = 0x3,
		CA_ACTIVE = 0x4,
		CA_MAP_RESTART = 0x5,
	};

	struct clientUIActive_t
	{
		bool isRunning;
		bool cgameInitialized;
		bool cgameInitCalled;
		bool isLoadComplete;
		int keyCatchers;
		bool displayHUDWithKeycatchUI;
		connstate_t connectionState;
		int nextScrollTime;
	};

	struct clSnapshot_t
	{
		int valid;
		int snapFlags;
		int serverTime;
		int messageNum;
		int cmdNum;
		playerState_s ps;
		int numEntities;
		int parseEntitiesNum;
		int serverCommandNum;
	};

	enum StanceState
	{
		CL_STANCE_STAND = 0x0,
		CL_STANCE_CROUCH = 0x1,
		CL_STANCE_PRONE = 0x2,
	};

	struct clientActive_t
	{
		clSnapshot_t snap;
		int serverTime;
		unsigned __int16 configstrings[2815];
		char mapname[64];
		bool usingAds;
		int parseEntitiesNum;
		int mouseDx[2];
		int mouseDy[2];
		int mouseIndex;
		bool stanceHeld;
		StanceState stance;
		StanceState stancePosition;
		int stanceTime;
		int cgameUserCmdWeapon;
		int cgameUserCmdOffHandIndex;
		float cgameUserCmdGunPitch;
		float cgameUserCmdGunYaw;
		float cgameUserCmdGunXOfs;
		float cgameUserCmdGunYOfs;
		float cgameUserCmdGunZOfs;
		float cgameFOVSensitivityScale;
		float cgameMaxPitchSpeed;
		float cgameMaxYawSpeed;
		int cgameExtraButtons;
		float viewangles[3];
		usercmd_s cmds[64];
		int cmdNumber;
		int cmdNumberAcknowledge;
		int bCmdForceValues;
		int iForceButtons;
		int iForceWeapon;
		int forceOffhand;
		int skelTimeStamp;
		volatile int skelMemPos;
		char skelMemory[524288];
		char* skelMemoryStart;
		bool allowedAllocSkel;
		clSnapshot_t snapshots[1];
		int parseEntityNums[2048];
	};

	struct AimInput
	{
		float deltaTime;
		float pitch;
		float pitchAxis;
		float pitchMax;
		float yaw;
		float yawAxis;
		float yawMax;
		float forwardAxis;
		float rightAxis;
		int buttons;
		int localClientNum;
		const playerState_s* ps;
	};

	struct AimOutput
	{
		float pitch;
		float yaw;
		float meleeChargeYaw;
		char meleeChargeDist;
	};

	struct AimTweakables
	{
		float slowdownRegionWidth;
		float slowdownRegionHeight;
		float autoAimRegionWidth;
		float autoAimRegionHeight;
		float autoMeleeRegionWidth;
		float autoMeleeRegionHeight;
		float lockOnRegionWidth;
		float lockOnRegionHeight;
	};
	
	constexpr auto AIM_TARGET_INVALID = 0x87F;
	struct AimScreenTarget
	{
		int entIndex;
		float clipMins[2];
		float clipMaxs[2];
		float aimPos[3];
		float velocity[3];
		float distSqr;
		float crosshairDistSqr;
	};

	struct AimAssistGlobals
	{
		bool initialized;
		AimTweakables tweakables;
		float viewOrigin[3];
		float viewAngles[3];
		float viewAxis[3][3];
		float fovTurnRateScale;
		float fovScaleInv;
		float adsLerp;
		float pitchDelta;
		float yawDelta;
		float screenWidth;
		float screenHeight;
		float screenMtx[4][4];
		float invScreenMtx[4][4];
		AimScreenTarget screenTargets[64];
		int screenTargetCount;
		int autoAimTargetEnt;
		bool autoAimPressed;
		bool autoAimActive;
		float autoAimPitch;
		float autoAimPitchTarget;
		float autoAimYaw;
		float autoAimYawTarget;
		int autoMeleeTargetEnt;
		bool autoMeleeActive;
		bool autoMeleePressed;
		float autoMeleePitch;
		float autoMeleePitchTarget;
		float autoMeleeYaw;
		float autoMeleeYawTarget;
		int lockOnTargetEnt;
	};

	struct clientLogo_t
	{
		int startTime;
		int duration;
		int fadein;
		int fadeout;
		Material* material[2];
	};

	struct __declspec(align(4)) vidConfig_t
	{
		unsigned int sceneWidth;
		unsigned int sceneHeight;
		unsigned int displayWidth;
		unsigned int displayHeight;
		float displayFrequency;
		int isWideScreen;
		int isHiDef;
		int isFullscreen;
		float aspectRatioWindow;
		float aspectRatioScenePixel;
		float aspectRatioDisplayPixel;
		unsigned int maxTextureSize;
		unsigned int maxTextureMaps;
		bool deviceSupportsGamma;
	};

	struct clientDebugStringInfo_t
	{
		int max;
		int num;
		trDebugString_t* strings;
		int* durations;
	};

	struct clientDebugLineInfo_t
	{
		int max;
		int num;
		trDebugLine_t* lines;
		int* durations;
	};

	struct clientDebug_t
	{
		int prevFromServer;
		int fromServer;
		clientDebugStringInfo_t clStrings;
		clientDebugStringInfo_t svStringsBuffer;
		clientDebugStringInfo_t svStrings;
		clientDebugLineInfo_t clLines;
		clientDebugLineInfo_t svLinesBuffer;
		clientDebugLineInfo_t svLines;
	};

	struct clientStatic_t
	{
		int quit;
		char servername[256];
		int rendererStarted;
		int soundStarted;
		int uiStarted;
		int frametime;
		int animFrametime;
		int realtime;
		int realFrametime;
		clientLogo_t logo;
		Font_s* consoleFont;
		bool demoplaying;
		bool demoPending;
		bool demorecording;
		bool isTimeDemo;
		char demoName[64];
		void* demofile;
		void* demobuf;
		void* timeDemoLog;
		int timeDemoFrames;
		int timeDemoStart;
		int timeDemoPrev;
		int timeDemoBaseTime;
		vidConfig_t vidConfig;
		Material* whiteMaterial;
		Material* consoleMaterial;
		clientDebug_t debug;
		float renderForward[3];
		float renderPos[3];
	};

	struct WinMouseVars_t
	{
		int oldButtonState;
		tagPOINT oldPos;
		bool mouseActive;
		bool mouseInitialized;
	};

	struct GraphFloat
	{
		char name[64];
		float knots[32][2];
		int knotCount;
		float scale;
		/*DevGraph devguiGraph;*/
	};

	enum FontPassType
	{
		FONTPASS_NORMAL = 0x0,
		FONTPASS_GLOW = 0x1,
		FONTPASS_OUTLINE = 0x2,
		FONTPASS_COUNT = 0x3,
	};

	enum TextRenderFlags
	{
		TEXT_RENDERFLAG_FORCEMONOSPACE = 0x1,
		TEXT_RENDERFLAG_CURSOR = 0x2,
		TEXT_RENDERFLAG_DROPSHADOW = 0x4,
		TEXT_RENDERFLAG_DROPSHADOW_EXTRA = 0x8,
		TEXT_RENDERFLAG_GLOW = 0x10,
		TEXT_RENDERFLAG_GLOW_FORCE_COLOR = 0x20,
		TEXT_RENDERFLAG_FX_DECODE = 0x40,
		TEXT_RENDERFLAG_PADDING = 0x80,
		TEXT_RENDERFLAG_SUBTITLETEXT = 0x100,
		TEXT_RENDERFLAG_CINEMATIC = 0x200,
		TEXT_RENDERFLAG_OUTLINE = 0x400,
		TEXT_RENDERFLAG_OUTLINE_EXTRA = 0x800,
	};

	struct languageInfo_t
	{
		const char* pszName;
		int bPresent;
	};

	enum GUI_MENUS
	{
		CHANGELOG = 0
	};

	struct gui_menus_t
	{
		bool menustate;
		bool mouse_ignores_menustate;
		bool was_open;
		bool hk_is_clicked;
		bool hk_is_down;
		bool one_time_init;
		bool got_layout_from_menu;
		float position[2];
		float size[2];
		int horzAlign;
		int vertAlign;
	};

	struct gui_t
	{
		bool initialized;
		bool any_menus_open;
		gui_menus_t menus[1];
	};

	enum pmType_t
	{
		PM_TYPE_NONE = 0x0,
		PM_TYPE_DEFAULT = 0x1,
		PM_TYPE_NOCLIP = 0x2,
		PM_TYPE_UFO = 0x3,
	};

	struct cgs_t
	{
		int viewX;
		int viewY;
		int viewWidth;
		int viewHeight;
		float viewAspect;
		char mapname[64];
		bool started;
		FxEffectDef* fxs[100];
		shellshock_parms_t holdBreathParams;
		float compassWidth;
		float compassHeight;
	};


	// rtx

	struct GfxCodeMatrices
	{
		GfxMatrix matrix[32];
	};

#pragma warning( push )
#pragma warning( disable : 4324 )
	struct __declspec(align(8)) GfxCmdBufInput
	{
		float consts[58][4];
		GfxImage* codeImages[27];
		char codeImageSamplerStates[27];
		GfxBackEndData* data;
	};
#pragma warning( pop )

	enum GfxViewMode
	{
		VIEW_MODE_NONE = 0x0,
		VIEW_MODE_3D = 0x1,
		VIEW_MODE_2D = 0x2,
		VIEW_MODE_IDENTITY = 0x3,
	};

	struct GfxSceneDef
	{
		int time;
		float floatTime;
		float viewOffset[3];
	};

	enum GfxViewportBehavior
	{
		GFX_USE_VIEWPORT_FOR_VIEW = 0x0,
		GFX_USE_VIEWPORT_FULL = 0x1,
	};

#pragma warning( push )
#pragma warning( disable : 4324 )
	struct __declspec(align(16)) GfxCmdBufSourceState
	{
		GfxCodeMatrices matrices;
		GfxCmdBufInput input;
		GfxViewParms viewParms;
		GfxMatrix shadowLookupMatrix;
		unsigned __int16 constVersions[90];
		unsigned __int16 matrixVersions[8];
		float eyeOffset[4];
		unsigned int shadowableLightForShadowLookupMatrix;
		GfxScaledPlacement* objectPlacement;
		GfxViewParms* viewParms3D;
		unsigned int depthHackFlags;
		GfxScaledPlacement skinnedPlacement;
		int cameraView;
		GfxViewMode viewMode;
		GfxSceneDef sceneDef;
		GfxViewport sceneViewport;
		float materialTime;
		GfxViewportBehavior viewportBehavior;
		int renderTargetWidth;
		int renderTargetHeight;
		bool viewportIsDirty;
		unsigned int shadowableLightIndex;
	};

	enum ShadowType
	{
		SHADOW_NONE = 0x0,
		SHADOW_COOKIE = 0x1,
		SHADOW_MAP = 0x2,
	};

	struct GfxViewInfo
	{
		GfxViewParms viewParms;
		GfxSceneDef sceneDef;
		GfxViewport sceneViewport;
		GfxViewport displayViewport;
		GfxViewport scissorViewport;
		ShadowType dynamicShadowType;
		/*__declspec(align(16)) ShadowCookieList shadowCookieList;
		int localClientNum;
		int isRenderingFullScreen;
		bool needsFloatZ;
		GfxLight shadowableLights[255];
		unsigned int shadowableLightCount;
		PointLightPartition pointLightPartitions[4];
		GfxMeshData pointLightMeshData[4];
		int pointLightCount;
		unsigned int emissiveSpotLightIndex;
		GfxLight emissiveSpotLight;
		int emissiveSpotDrawSurfCount;
		GfxDrawSurf* emissiveSpotDrawSurfs;
		unsigned int emissiveSpotLightCount;
		float blurRadius;
		float frustumPlanes[4][4];
		GfxDepthOfField dof;
		GfxFilm film;
		GfxGlow glow;
		const void* cmds;
		GfxSunShadow sunShadow;
		unsigned int spotShadowCount;
		__declspec() GfxSpotShadow spotShadows[4];
		GfxQuadMeshData* fullSceneViewMesh;
		__declspec(align(1)) GfxDrawSurfListInfo litInfo;
		__declspec(align(1)) GfxDrawSurfListInfo decalInfo;
		__declspec(align(1)) GfxDrawSurfListInfo emissiveInfo;
		GfxCmdBufInput input;*/
	};
#pragma warning( pop )

	enum MaterialVertexDeclType
	{
		VERTDECL_GENERIC = 0x0,
		VERTDECL_PACKED = 0x1,
		VERTDECL_WORLD = 0x2,
		VERTDECL_WORLD_T1N0 = 0x3,
		VERTDECL_WORLD_T1N1 = 0x4,
		VERTDECL_WORLD_T2N0 = 0x5,
		VERTDECL_WORLD_T2N1 = 0x6,
		VERTDECL_WORLD_T2N2 = 0x7,
		VERTDECL_WORLD_T3N0 = 0x8,
		VERTDECL_WORLD_T3N1 = 0x9,
		VERTDECL_WORLD_T3N2 = 0xA,
		VERTDECL_WORLD_T4N0 = 0xB,
		VERTDECL_WORLD_T4N1 = 0xC,
		VERTDECL_WORLD_T4N2 = 0xD,
		VERTDECL_POS_TEX = 0xE,
		VERTDECL_STATICMODELCACHE = 0xF,
		VERTDECL_COUNT = 0x10,
	};

	struct gfxVertexSteamsUnk
	{
		unsigned int stride;
		IDirect3DVertexBuffer9* vb; // IDirect3DVertexBuffer9
		unsigned int offset;
	};

	struct GfxCmdBufPrimState
	{
		IDirect3DDevice9* device; // IDirect3DDevice9
		IDirect3DIndexBuffer9* indexBuffer; // IDirect3DIndexBuffer9
		MaterialVertexDeclType vertDeclType;
		gfxVertexSteamsUnk streams[2];
		int* vertexDecl; // IDirect3DVertexDeclaration9
	};

	enum GfxDepthRangeType
	{
		GFX_DEPTH_RANGE_SCENE = 0x0,
		GFX_DEPTH_RANGE_VIEWMODEL = 0x2,
		GFX_DEPTH_RANGE_FULL = 0xFFFFFFFF,
	};

	struct GfxPixelShaderLoadDef
	{
		unsigned int* program;
		unsigned __int16 programSize;
		unsigned __int16 loadForRenderer;
	};

	struct MaterialPixelShaderProgram
	{
		void* ps;
		GfxPixelShaderLoadDef loadDef;
	};

	struct MaterialPixelShader
	{
		const char* name;
		MaterialPixelShaderProgram prog;
	};

	struct GfxVertexShaderLoadDef
	{
		unsigned int* program;
		unsigned __int16 programSize;
		unsigned __int16 loadForRenderer;
	};

	struct MaterialVertexShaderProgram
	{
		void* vs;
		GfxVertexShaderLoadDef loadDef;
	};

	struct MaterialVertexShader
	{
		const char* name;
		MaterialVertexShaderProgram prog;
	};

	enum GfxRenderTargetId
	{
		R_RENDERTARGET_SAVED_SCREEN = 0x0,
		R_RENDERTARGET_FRAME_BUFFER = 0x1,
		R_RENDERTARGET_SCENE = 0x2,
		R_RENDERTARGET_RESOLVED_POST_SUN = 0x3,
		R_RENDERTARGET_RESOLVED_SCENE = 0x4,
		R_RENDERTARGET_FLOAT_Z = 0x5,
		R_RENDERTARGET_DYNAMICSHADOWS = 0x6,
		R_RENDERTARGET_PINGPONG_0 = 0x7,
		R_RENDERTARGET_PINGPONG_1 = 0x8,
		R_RENDERTARGET_SHADOWCOOKIE = 0x9,
		R_RENDERTARGET_SHADOWCOOKIE_BLUR = 0xA,
		R_RENDERTARGET_POST_EFFECT_0 = 0xB,
		R_RENDERTARGET_POST_EFFECT_1 = 0xC,
		R_RENDERTARGET_SHADOWMAP_SUN = 0xD,
		R_RENDERTARGET_SHADOWMAP_SPOT = 0xE,
		R_RENDERTARGET_COUNT = 0xF,
		R_RENDERTARGET_NONE = 0x10,
	};

	struct GfxCmdBufState
	{
		char refSamplerState[16];
		unsigned int samplerState[16];
		GfxTexture* samplerTexture[16];
		GfxCmdBufPrimState prim;
		Material* material;
		MaterialTechniqueType techType;
		MaterialTechnique* technique;
		MaterialPass* pass;
		unsigned int passIndex;
		GfxDepthRangeType depthRangeType;
		float depthRangeNear;
		float depthRangeFar;
		unsigned __int64 vertexShaderConstState[32];
		unsigned __int64 pixelShaderConstState[256];
		char alphaRef;
		unsigned int refStateBits[2];
		unsigned int activeStateBits[2];
		MaterialPixelShader* pixelShader;
		MaterialVertexShader* vertexShader;
		GfxViewport viewport;
		GfxRenderTargetId renderTargetId;
		Material* origMaterial;
		MaterialTechniqueType origTechType;
	};

	struct Image_MemUsage
	{
		int total;
		int lightmap;
		int minspec;
	};

	struct trStatistics_t
	{
		int c_indexes;
		int c_fxIndexes;
		int c_viewIndexes;
		int c_shadowIndexes;
		int c_vertexes;
		int c_batches;
		float dc;
		Image_MemUsage c_imageUsage;
	};

	struct GfxPackedPlacement
	{
		float origin[3];
		vec3_t axis[3];
		float scale;
	};

	struct __declspec(align(4)) GfxStaticModelDrawInst
	{
		float cullDist;
		GfxPackedPlacement placement;
		XModel* model;
		unsigned __int16 smodelCacheIndex[4];
		char reflectionProbeIndex;
		char primaryLightIndex;
		unsigned __int16 lightingHandle;
		char flags;
	};

	struct GfxStaticModelDrawStream
	{
		const unsigned int* primDrawSurfPos;
		GfxTexture* reflectionProbeTexture;
		unsigned int customSamplerFlags;
		XSurface* localSurf;
		unsigned int smodelCount;
		const unsigned __int16* smodelList;
		unsigned int reflectionProbeIndex;
	};

#pragma warning( push )
#pragma warning( disable : 4324 )
	struct __declspec(align(8)) r_globals_t
	{
		GfxViewParms identityViewParms;
		bool inFrame;
		bool registered;
		bool forbidDelayLoadImages;
		bool ignorePrecacheErrors;
		float viewOrg[3];
		float viewDir[3];
		unsigned int frontEndFrameCount;
		int totalImageMemory;
		Material* materialHashTable[2048];
		GfxFog fogSettings[5];
		int fogIndex;
		GfxColor color_axis;
		GfxColor color_allies;
		int team;
		trStatistics_t* stats;
		GfxLodParms lodParms;
		GfxLodParms correctedLodParms;
		bool hasAnyImageOverrides;
		bool useSunLightOverride;
		bool useSunDirOverride;
		bool useSunDirLerp;
		float sunLightOverride[3];
		float sunDirOverride[3];
		float sunDirOverrideTarget[3];
		int sunDirLerpBeginTime;
		int sunDirLerpEndTime;
		GfxScaledPlacement identityPlacement;
		GfxViewParms* debugViewParms;
		int endTime;
		bool distortion;
		bool drawSModels;
		bool drawXModels;
		bool drawBModels;
		const char* codeImageNames[27];
		unsigned int viewInfoCount;
		int sunShadowFull;
		float sunShadowmapScale;
		float sunShadowmapScaleNum;
		unsigned int sunShadowSize;
		float sunShadowPartitionRatio;
		int drawSunShadow;
		int skinnedCacheReachedThreshold;
		float waterFloatTime;
	};
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4324 )

	struct GfxWorldStreamInfo
	{
		int aabbTreeCount;
		// 			GfxStreamingAabbTree *aabbTrees;
		// 			int leafRefCount;
		// 			int *leafRefs;
	};

	struct GfxWorldVertex
	{
		float xyz[3];
		float binormalSign;
		GfxColor color;
		float texCoord[2];
		float lmapCoord[2];
		PackedUnitVec normal;
		PackedUnitVec tangent;
	};

	struct GfxWorldVertexData
	{
		GfxWorldVertex* vertices;
		void/*IDirect3DVertexBuffer9*/* worldVb;
	};

	struct GfxWorldVertexLayerData
	{
		char* data;
		void/*IDirect3DVertexBuffer9*/* layerVb;
	};

	struct SunLightParseParams
	{
		char name[64];
		float ambientScale;
		float ambientColor[3];
		float diffuseFraction;
		float sunLight;
		float sunColor[3];
		float diffuseColor[3];
		char diffuseColorHasBeenSet;
		float angles[3];
	};

	struct DpvsPlane
	{
		float coeffs[4];
		char side[3];
		char pad;
	};

	struct DpvsView
	{
		unsigned int renderFxFlagsCull;
		DpvsPlane frustumPlanes[14];
		int frustumPlaneCount;
	};

	struct PortalHeapNode
	{
		GfxPortal* portal;
		float dist;
	};

	struct DpvsGlob
	{
		DpvsPlane viewPlane;
		DpvsPlane fogPlane;
		DpvsPlane* nearPlane;
		DpvsPlane* farPlane;
		GfxMatrix* viewProjMtx;
		GfxMatrix* invViewProjMtx;
		float viewOrg[4];
		int viewOrgIsDir;
		int queuedCount;
		PortalHeapNode* portalQueue;
		GfxHullPointsPool* nextFreeHullPoints;
		float cullDist;
		DpvsPlane childPlanes[2048];
		DpvsView views[4][3];
		unsigned int cameraCellIndex;
		DpvsPlane* sideFrustumPlanes;
		unsigned int* entVisBits[4];
		unsigned int* cellBits;
		unsigned int cellVisibleBits[32];
	};

	union GfxHullPointsPool
	{
		GfxHullPointsPool* nextFree;
		float points[64][2];
	};

	struct GfxPortalWritable
	{
		bool isQueued;
		bool isAncestor;
		char recursionDepth;
		char hullPointCount;
		float(*hullPoints)[2];
		GfxPortal* queuedParent;
	};

	struct GfxCell
	{
		float mins[3];
		float maxs[3];
		int aabbTreeCount;
		GfxAabbTree* aabbTree;
		int portalCount;
		GfxPortal* portals;
		int cullGroupCount;
		int* cullGroups;
		char reflectionProbeCount;
		char* reflectionProbes;
	};

	struct GfxPortal
	{
		GfxPortalWritable writable;
		DpvsPlane plane;
		GfxCell* cell;
		float(*vertices)[3];
		char vertexCount;
		float hullAxis[2][3];
	};

	struct GfxAabbTree
	{
		float mins[3];
		float maxs[3];
		unsigned __int16 childCount;
		unsigned __int16 surfaceCount;
		unsigned __int16 startSurfIndex;
		unsigned __int16 surfaceCountNoDecal;
		unsigned __int16 startSurfIndexNoDecal;
		unsigned __int16 smodelIndexCount;
		unsigned __int16* smodelIndexes;
		int childrenOffset;
	};

	struct GfxWorldDpvsPlanes
	{
		int cellCount;
		cplane_s* planes;
		unsigned __int16* nodes;
		unsigned int* sceneEntCellBits;
	};

	struct GfxLightGridEntry
	{
		unsigned __int16 colorsIndex;
		char primaryLightIndex;
		char needsTrace;
	};

	struct GfxLightGridColors
	{
		char rgb[56][3];
	};

	struct GfxLightGrid
	{
		char hasLightRegions;
		unsigned int sunPrimaryLightIndex;
		unsigned __int16 mins[3];
		unsigned __int16 maxs[3];
		unsigned int rowAxis;
		unsigned int colAxis;
		unsigned __int16* rowDataStart;
		unsigned int rawRowDataSize;
		char* rawRowData;
		unsigned int entryCount;
		GfxLightGridEntry* entries;
		unsigned int colorCount;
		GfxLightGridColors* colors;
	};

	struct sunflare_t
	{
		char hasValidData;
		Material* spriteMaterial;
		Material* flareMaterial;
		float spriteSize;
		float flareMinSize;
		float flareMinDot;
		float flareMaxSize;
		float flareMaxDot;
		float flareMaxAlpha;
		int flareFadeInTime;
		int flareFadeOutTime;
		float blindMinDot;
		float blindMaxDot;
		float blindMaxDarken;
		int blindFadeInTime;
		int blindFadeOutTime;
		float glareMinDot;
		float glareMaxDot;
		float glareMaxLighten;
		int glareFadeInTime;
		int glareFadeOutTime;
		float sunFxPosition[3];
	};

	struct GfxStaticModelInst
	{
		float mins[3];
		float maxs[3];
		GfxColor groundLighting;
	};

	struct srfTriangles_t
	{
		int vertexLayerData;
		int firstVertex;
		unsigned __int16 vertexCount;
		unsigned __int16 triCount;
		int baseIndex;
	};

	struct GfxSurface
	{
		srfTriangles_t tris;
		Material* material;
		char lightmapIndex;
		char reflectionProbeIndex;
		char primaryLightIndex;
		char flags;
		float bounds[2][3];
	};

	struct GfxCullGroup
	{
		float mins[3];
		float maxs[3];
		int surfaceCount;
		int startSurfIndex;
	};

	struct GfxWorldDpvsStatic
	{
		unsigned int smodelCount;
		unsigned int staticSurfaceCount;
		unsigned int staticSurfaceCountNoDecal;
		unsigned int litSurfsBegin;
		unsigned int litSurfsEnd;
		unsigned int decalSurfsBegin;
		unsigned int decalSurfsEnd;
		unsigned int emissiveSurfsBegin;
		unsigned int emissiveSurfsEnd;
		unsigned int smodelVisDataCount;
		unsigned int surfaceVisDataCount;
		char* smodelVisData[3];
		char* surfaceVisData[3];
		unsigned int* lodData;
		unsigned __int16* sortedSurfIndex;
		GfxStaticModelInst* smodelInsts;
		GfxSurface* surfaces;
		GfxCullGroup* cullGroups;
		GfxStaticModelDrawInst* smodelDrawInsts;
		GfxDrawSurf* surfaceMaterials;
		unsigned int* surfaceCastsSunShadow;
		volatile int usageCount;
	};

	struct GfxWorldDpvsDynamic
	{
		unsigned int dynEntClientWordCount[2];
		unsigned int dynEntClientCount[2];
		unsigned int* dynEntCellBits[2];
		char* dynEntVisData[2][3];
	};

	struct GfxWorld
	{
		const char* name;
		const char* baseName;
		int planeCount;
		int nodeCount;
		int indexCount;
		unsigned __int16* indices;
		int surfaceCount;
		GfxWorldStreamInfo streamInfo;
		int skySurfCount;
		int* skyStartSurfs;
		GfxImage* skyImage;
		char skySamplerState;
		unsigned int vertexCount;
		GfxWorldVertexData vd;
		unsigned int vertexLayerDataSize;
		GfxWorldVertexLayerData vld;
		SunLightParseParams sunParse;
		GfxLight* sunLight;
		float sunColorFromBsp[3];
		unsigned int sunPrimaryLightIndex;
		unsigned int primaryLightCount;
		int cullGroupCount;
		unsigned int reflectionProbeCount;
		void* reflectionProbes; // GfxReflectionProbe
		GfxTexture* reflectionProbeTextures;
		GfxWorldDpvsPlanes dpvsPlanes;
		int cellBitsCount;
		GfxCell* cells; // GfxCell
		int lightmapCount;
		void* lightmaps; // GfxLightmapArray
		GfxLightGrid lightGrid;
		GfxTexture* lightmapPrimaryTextures;
		GfxTexture* lightmapSecondaryTextures;
		int modelCount;
		void* models; // GfxBrushModel
		float mins[3];
		float maxs[3];
		unsigned int checksum;
		int materialMemoryCount;
		void* materialMemory; // MaterialMemory
		sunflare_t sun;
		float outdoorLookupMatrix[4][4];
		GfxImage* outdoorImage;
		unsigned int* cellCasterBits;
		void* sceneDynModel; // GfxSceneDynModel
		void* sceneDynBrush; // GfxSceneDynBrush
		unsigned int* primaryLightEntityShadowVis;
		unsigned int* primaryLightDynEntShadowVis[2];
		char* nonSunPrimaryLightForModelDynEnt;
		void* shadowGeom; // GfxShadowGeometry
		void* lightRegion; // GfxLightRegion
		GfxWorldDpvsStatic dpvs;
		GfxWorldDpvsDynamic dpvsDyn;
	};

	struct __declspec(align(128)) r_global_permanent_t
	{
		Material* sortedMaterials[2048];
		int needSortMaterials;
		int materialCount;
		GfxImage* whiteImage;
		GfxImage* blackImage;
		GfxImage* blackImage3D;
		GfxImage* blackImageCube;
		GfxImage* grayImage;
		GfxImage* identityNormalMapImage;
		GfxImage* specularityImage;
		GfxImage* outdoorImage;
		GfxImage* pixelCostColorCodeImage;
		GfxLightDef* dlightDef;
		Material* defaultMaterial;
		Material* whiteMaterial;
		Material* additiveMaterial;
		Material* pointMaterial;
		Material* lineMaterial;
		Material* lineMaterialNoDepth;
		Material* clearAlphaStencilMaterial;
		Material* shadowClearMaterial;
		Material* shadowCookieOverlayMaterial;
		Material* shadowCookieBlurMaterial;
		Material* shadowCasterMaterial;
		Material* shadowOverlayMaterial;
		Material* depthPrepassMaterial;
		Material* glareBlindMaterial;
		Material* pixelCostAddDepthAlwaysMaterial;
		Material* pixelCostAddDepthDisableMaterial;
		Material* pixelCostAddDepthEqualMaterial;
		Material* pixelCostAddDepthLessMaterial;
		Material* pixelCostAddDepthWriteMaterial;
		Material* pixelCostAddNoDepthWriteMaterial;
		Material* pixelCostColorCodeMaterial;
		Material* stencilShadowMaterial;
		Material* stencilDisplayMaterial;
		Material* floatZDisplayMaterial;
		Material* colorChannelMixerMaterial;
		Material* frameColorDebugMaterial;
		Material* frameAlphaDebugMaterial;
		GfxImage* rawImage;
		GfxWorld* world;
		Material* feedbackReplaceMaterial;
		Material* feedbackBlendMaterial;
		Material* feedbackFilmBlendMaterial;
		Material* cinematicMaterial;
		Material* dofDownsampleMaterial;
		Material* dofNearCocMaterial;
		Material* smallBlurMaterial;
		Material* postFxDofMaterial;
		Material* postFxDofColorMaterial;
		Material* postFxColorMaterial;
		Material* postFxMaterial;
		Material* symmetricFilterMaterial[8];
		Material* shellShockBlurredMaterial;
		Material* shellShockFlashedMaterial;
		Material* glowConsistentSetupMaterial;
		Material* glowApplyBloomMaterial;
		int savedScreenTimes[4];
	};
#pragma warning( pop )

	struct GfxDrawSurfListInfo
	{
		GfxDrawSurf* drawSurfs;
		unsigned int drawSurfCount;
		MaterialTechniqueType baseTechType;
		GfxViewInfo* viewInfo;
		float viewOrigin[4];
		GfxLight* light;
		int cameraView;
	};

	struct GfxCmdBufContext
	{
		GfxCmdBufSourceState* source;
		GfxCmdBufState* state;
	};

	struct GfxDrawSurfListArgs
	{
		GfxCmdBufContext context;
		unsigned int firstDrawSurfIndex;
		GfxDrawSurfListInfo* info;
	};

	struct BModelSurface
	{
		GfxScaledPlacement* placement;
		GfxSurface* surf;
	};

	struct GfxWindowTarget
	{
		HWND__* hwnd;
		IDirect3DSwapChain9* swapChain;
		int width;
		int height;
	};

	struct __declspec(align(8)) DxGlobals
	{
		IDirect3D9* d3d9;
		IDirect3DDevice9* device;
		unsigned int adapterIndex;
		bool adapterNativeIsValid;
		int adapterNativeWidth;
		int adapterNativeHeight;
		int adapterFullscreenWidth;
		int adapterFullscreenHeight;
		int depthStencilFormat;
		unsigned int displayModeCount;
		_D3DDISPLAYMODE displayModes[256];
		const char* resolutionNameTable[257];
		const char* refreshRateNameTable[257];
		char modeText[5120];
		IDirect3DQuery9* fencePool[8];
		unsigned int nextFence;
		int gpuSync;
		int multiSampleType;
		unsigned int multiSampleQuality;
		int sunSpriteSamples;
		IDirect3DSurface9* singleSampleDepthStencilSurface;
		bool deviceLost;
		bool inScene;
		int targetWindowIndex;
		int windowCount;
		GfxWindowTarget windows[1];
		int flushGpuQueryCount;
		IDirect3DQuery9* flushGpuQuery;
		unsigned __int64 gpuSyncDelay;
		unsigned __int64 gpuSyncStart;
		unsigned __int64 gpuSyncEnd;
		bool flushGpuQueryIssued;
		int linearNonMippedMinFilter;
		int linearNonMippedMagFilter;
		int linearMippedMinFilter;
		int linearMippedMagFilter;
		int anisotropicMinFilter;
		int anisotropicMagFilter;
		int linearMippedAnisotropy;
		int anisotropyFor2x;
		int anisotropyFor4x;
		int mipFilterMode;
		unsigned int mipBias;
		IDirect3DQuery9* swapFence;
	};

	struct switch_material_t
	{
		bool switch_material;
		bool switch_technique;
		bool switch_technique_type;

		Game::Material* current_material;
		Game::MaterialTechnique* current_technique;

		Game::Material* material;
		Game::MaterialTechnique* technique;

		Game::MaterialTechniqueType technique_type;
	};
}
