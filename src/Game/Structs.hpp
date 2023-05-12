#pragma once

// This allows us to compile our structures in IDA, for easier reversing :3
#ifndef __cplusplus
#define IDA
#endif

#ifndef IDA
namespace Game
{
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
		XFILE_BLOCK_PHYSICAL = 0x1,
		XFILE_BLOCK_RUNTIME = 0x2,
		XFILE_BLOCK_VIRTUAL = 0x3,
		XFILE_BLOCK_LARGE = 0x4,

		// Those are probably incorrect
		XFILE_BLOCK_CALLBACK,
		XFILE_BLOCK_VERTEX,
		XFILE_BLOCK_INDEX,

		MAX_XFILE_COUNT,

		XFILE_BLOCK_INVALID = -1
	};

	struct XBlock
	{
		char* data;
		unsigned int size;
	};

	struct XZoneMemory
	{
		XBlock blocks[MAX_XFILE_COUNT];
		char* lockedVertexData;
		char* lockedIndexData;
		void* vertexBuffer;
		void* indexBuffer;
	};

	struct XZone
	{
		int unk;
		char name[64];
		int flags;
		int allocType;
		XZoneMemory mem;
		int fileSize;
		char modZone;
		int ff_dir;
	};

	struct XFile
	{
		unsigned int size;
		unsigned int externalSize;
		unsigned int blockSize[9];
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
		unsigned int totalMsec;
	};

	union SoundFileRef
	{
		LoadedSound* loadSnd;
		StreamedSound streamSnd;
	};

	struct SoundFile_t
	{
		byte type;
		byte pad2[3];
		SoundFileRef sound;
		byte exists;
		byte pad[3];
	};

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

	typedef union
	{
		int handle;
	}SndEntHandle_t;

	typedef enum
	{
		SASYS_UI = 0x0,
		SASYS_CGAME = 0x1,
		SASYS_GAME = 0x2,
		SASYS_COUNT = 0x3,
	}snd_alias_system_t;

	typedef struct
	{
		struct snd_alias_t* alias0; //0
		struct snd_alias_t* alias1; //4
		float lerp; //8
		SndEntHandle_t sndEnt; //12
		float org[3]; //16
		float volume; //20
		float pitch;
		int timeshift;
		float fraction;
		int startDelay;
		byte master;
		byte timescale;
		byte pad[2];
		snd_alias_system_t system;
	}SndStartAliasInfo_t;

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

	struct snd_channelvolgroup
	{
		snd_volume_info_t* channelIndex[64];
		bool active;
	};

	struct snd_background_info_t
	{
		float goalvolume;
		float goalrate;
	};

	struct snd_local_t
	{
		bool Initialized2d;
		bool Initialized3d;
		bool paused;
		int playbackIdCounter;
		unsigned int playback_rate;
		int playback_channels;
		float timescale;
		int pausetime;
		int cpu;
		struct
		{
			char buffer[16384];
			volatile int size;
			bool compress;
		} restore;
		float volume;
		SndStartAliasInfo_t mastervol;
		snd_channelvolgroup channelVolGroups[4];
		snd_channelvolgroup* channelvol;
		snd_background_info_t background[4];
		int ambient_track;
		float slaveLerp;
		float masterPercentage;
		/*
		snd_enveffect envEffects[5];
		snd_enveffect* effect;
		snd_listener listeners[2];
		int time;
		int looptime;
		snd_amplifier amplifier;
		snd_entchannel_info_t entchaninfo[64];
		snd_entchan_overrides_t entchanOverrides;
		int entchannel_count;
		snd_channel_info_t chaninfo[52];
		int max_2D_channels;
		int max_3D_channels;
		int max_stream_channels;
		*/
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

	typedef struct
	{
		int version;
		unsigned int hash;
		int numEnums;
		int numStructs;
		int numIndexedArrays;
		int numEnumArrays;
	} structuredData_t;

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
		CMD_BUTTON_THROW = 1 << 19,
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
		float meleeChargeYaw;
		char meleeChargeDist;
		char selectedLocation[2];
	};

	typedef enum BGEvent
	{
		EV_NONE = 1 << 0,
		EV_FOLIAGE_SOUND = 1 << 1,
		EV_STOP_WEAPON_SOUND = 1 << 2,
		EV_SOUND_ALIAS = 1 << 3,
		EV_SOUND_ALIAS_AS_MASTER = 1 << 4,
		EV_STOPSOUNDS = 1 << 5,
		EV_STANCE_FORCE_STAND = 1 << 6,
		EV_STANCE_FORCE_CROUCH = 1 << 7,
		EV_STANCE_FORCE_PRONE = 1 << 8,
		EV_ITEM_PICKUP = 1 << 9,
		EV_AMMO_PICKUP = 1 << 10,
		EV_NOAMMO = 1 << 11,
		EV_EMPTYCLIP = 1 << 12,
		EV_EMPTY_OFFHAND = 1 << 13,
		EV_RESET_ADS = 1 << 14,
		EV_RELOAD = 1 << 15,
		EV_RELOAD_FROM_EMPTY = 1 << 16,
		EV_RELOAD_START = 1 << 17,
		EV_RELOAD_END = 1 << 18,
		EV_RELOAD_START_NOTIFY = 1 << 19,
		EV_RELOAD_ADDAMMO = 1 << 20,
		EV_RAISE_WEAPON = 1 << 21,
		EV_FIRST_RAISE_WEAPON = 1 << 22,
		EV_PUTAWAY_WEAPON = 1 << 23,
		EV_WEAPON_ALT = 1 << 24
		/*
		EV_PULLBACK_WEAPON,
		EV_FIRE_WEAPON,
		EV_FIRE_WEAPON_LASTSHOT,
		EV_RECHAMBER_WEAPON,
		EV_EJECT_BRASS,
		EV_MELEE_SWIPE,
		EV_FIRE_MELEE,
		EV_PREP_OFFHAND,
		EV_USE_OFFHAND,
		EV_SWITCH_OFFHAND,
		EV_MELEE_HIT,
		EV_MELEE_MISS,
		EV_MELEE_BLOOD,
		EV_FIRE_WEAPON_MG42,
		EV_FIRE_QUADBARREL_1,
		EV_FIRE_QUADBARREL_2,
		EV_BULLET_HIT,
		EV_BULLET_HIT_CLIENT_SMALL,
		EV_BULLET_HIT_CLIENT_LARGE,
		EV_GRENADE_BOUNCE,
		EV_GRENADE_EXPLODE,
		EV_ROCKET_EXPLODE,
		EV_ROCKET_EXPLODE_NOMARKS,
		EV_FLASHBANG_EXPLODE,
		EV_CUSTOM_EXPLODE,
		EV_CUSTOM_EXPLODE_NOMARKS,
		EV_CHANGE_TO_DUD,
		EV_DUD_EXPLODE,
		EV_DUD_IMPACT,
		EV_BULLET,
		EV_PLAY_FX,
		EV_PLAY_FX_ON_TAG,
		EV_PHYS_EXPLOSION_SPHERE,
		EV_PHYS_EXPLOSION_CYLINDER,
		EV_PHYS_EXPLOSION_JOLT,
		EV_PHYS_JITTER,
		EV_EARTHQUAKE,
		EV_GRENADE_SUICIDE,
		EV_DETONATE,
		EV_NIGHTVISION_WEAR,
		EV_NIGHTVISION_REMOVE,
		EV_OBITUARY,
		EV_NO_FRAG_GRENADE_HINT,
		EV_NO_SPECIAL_GRENADE_HINT,
		EV_TARGET_TOO_CLOSE_HINT,
		EV_TARGET_NOT_ENOUGH_CLEARANCE,
		EV_LOCKON_REQUIRED_HINT,
		EV_FOOTSTEP_SPRINT,
		EV_FOOTSTEP_RUN,
		EV_FOOTSTEP_WALK,
		EV_FOOTSTEP_PRONE,
		EV_JUMP,
		EV_LANDING_DEFAULT,
		EV_LANDING_BARK,
		EV_LANDING_BRICK,
		EV_LANDING_CARPET,
		EV_LANDING_CLOTH,
		EV_LANDING_CONCRETE,
		EV_LANDING_DIRT,
		EV_LANDING_FLESH,
		EV_LANDING_FOLIAGE,
		EV_LANDING_GLASS,
		EV_LANDING_GRASS,
		EV_LANDING_GRAVEL,
		EV_LANDING_ICE,
		EV_LANDING_METAL,
		EV_LANDING_MUD,
		EV_LANDING_PAPER,
		EV_LANDING_PLASTER,
		EV_LANDING_ROCK,
		EV_LANDING_SAND,
		EV_LANDING_SNOW,
		EV_LANDING_WATER,
		EV_LANDING_WOOD,
		EV_LANDING_ASPHALT,
		EV_LANDING_CERAMIC,
		EV_LANDING_PLASTIC,
		EV_LANDING_RUBBER,
		EV_LANDING_CUSHION,
		EV_LANDING_FRUIT,
		EV_LANDING_PAINTEDMETAL,
		EV_LANDING_PAIN_DEFAULT,
		EV_LANDING_PAIN_BARK,
		EV_LANDING_PAIN_BRICK,
		EV_LANDING_PAIN_CARPET,
		EV_LANDING_PAIN_CLOTH,
		EV_LANDING_PAIN_CONCRETE,
		EV_LANDING_PAIN_DIRT,
		EV_LANDING_PAIN_FLESH,
		EV_LANDING_PAIN_FOLIAGE,
		EV_LANDING_PAIN_GLASS,
		EV_LANDING_PAIN_GRASS,
		EV_LANDING_PAIN_GRAVEL,
		EV_LANDING_PAIN_ICE,
		EV_LANDING_PAIN_METAL,
		EV_LANDING_PAIN_MUD,
		EV_LANDING_PAIN_PAPER,
		EV_LANDING_PAIN_PLASTER,
		EV_LANDING_PAIN_ROCK,
		EV_LANDING_PAIN_SAND,
		EV_LANDING_PAIN_SNOW,
		EV_LANDING_PAIN_WATER,
		EV_LANDING_PAIN_WOOD,
		EV_LANDING_PAIN_ASPHALT,
		EV_LANDING_PAIN_CERAMIC,
		EV_LANDING_PAIN_PLASTIC,
		EV_LANDING_PAIN_RUBBER,
		EV_LANDING_PAIN_CUSHION,
		EV_LANDING_PAIN_FRUIT,
		EV_LANDING_PAIN_PAINTEDMETAL
		*/
	}BGEvent;

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
	struct GfxImage
	{
		char* texture;
		char unknown2;
		char a3;
		char a2;
		char unknown3;
		char unknown4;
		char unknown5;
		char unknown6;
		char a4;
		int dataLength1;
		int dataLength2;
		short height;
		short width;
		short depth;
		short unknown8;
		char* name;
	};

	struct MaterialTextureDef
	{
		unsigned int typeHash; // asset hash of type
		char firstCharacter; // first character of image name
		char secondLastCharacter; // second-last character of image name (maybe only in CoD4?!)
		unsigned char unknown; // maybe 0xE2
		char unknown2; // likely 0x00
		GfxImage* image; // GfxImage* actually
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

	struct MaterialPass
	{
		VertexDecl* vertexDecl;
		VertexShader* vertexShader;
		PixelShader* pixelShader;
		char pad[8];
	};

	struct MaterialTechnique
	{
		int pad;
		short pad2;
		short numPasses;
		MaterialPass passes[1];
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
		unsigned __int16 letter;
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

	struct ScreenPlacement
	{
		float scaleVirtualToReal[2];
		float scaleVirtualToFull[2];
		float scaleRealToVirtual[2];
		float realViewportPosition[2];
		float realViewportSize[2];
		float virtualViewableMin[2];
		float virtualViewableMax[2];
		float realViewableMin[2];
		float realViewableMax[2];
		float virtualAdjustableMin[2];
		float virtualAdjustableMax[2];
		float realAdjustableMin[2];
		float realAdjustableMax[2];
		float subScreenLeft;
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

	union XAssetHeader
	{
		void* data;
		LocalizeEntry* localize;
		Font_s* font;
		snd_alias_list_t* sound;
		WeaponDef_s* weapon;
		RawFile* rawfile;
		Material* material;
		StringTable* stringTable;
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

	struct VariableStackBuffer
	{
		const char* pos;
		uint16_t size;
		uint16_t bufLen;
		uint16_t localId;
		char time;
		char buf[1];
	};

	union VariableUnion
	{
		int intValue;
		float floatValue;
		unsigned int stringValue;
		const float* vectorValue;
		const char* codePosValue;
		unsigned int pointerValue;
		struct VariableStackBuffer* stackValue;
		unsigned int entityOffset;
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

	union entryInternalData
	{
		int operationEnum;
		Operand operand;
	};

	struct expressionEntry
	{
		int type;
		entryInternalData data;
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

	typedef struct
	{
		scr_string_t tag_clip;
		scr_string_t light_gear;
		scr_string_t heavy_gear;
	}scr_const_t;

	struct entityState_s {
		unsigned char eType; // 0
		unsigned char __pad0[0x7D];
		unsigned __int16 number; // 0x7E
		unsigned char __pad1[0x2C];
	};

	struct entityShared_t {
		unsigned char __pad0[0x5C];
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

	typedef enum
	{
		OBJST_EMPTY = 0x0,
		OBJST_ACTIVE = 0x1,
		OBJST_INVISIBLE = 0x2,
		OBJST_DONE = 0x3,
		OBJST_CURRENT = 0x4,
		OBJST_FAILED = 0x5,
		OBJST_NUMSTATES = 0x6,
	}objectiveState_t;

	typedef struct objective_s
	{
		objectiveState_t state;
		float origin[3];
		int entNum;
		int teamNum;
		int icon;
	}objective_t;

	typedef struct
	{
		unsigned int index;
	}ActionSlotParam_SpecifyWeapon_t;


	typedef struct
	{
		ActionSlotParam_SpecifyWeapon_t specifyWeapon;
	}ActionSlotParam_t;

#define MAX_HUDELEMENTS 31

	typedef enum
	{
		HE_TYPE_FREE = 0x0,
		HE_TYPE_TEXT = 0x1,
		HE_TYPE_VALUE = 0x2,
		HE_TYPE_PLAYERNAME = 0x3,
		HE_TYPE_MAPNAME = 0x4,
		HE_TYPE_GAMETYPE = 0x5,
		HE_TYPE_MATERIAL = 0x6,
		HE_TYPE_TIMER_DOWN = 0x7,
		HE_TYPE_TIMER_UP = 0x8,
		HE_TYPE_TENTHS_TIMER_DOWN = 0x9,
		HE_TYPE_TENTHS_TIMER_UP = 0xA,
		HE_TYPE_CLOCK_DOWN = 0xB,
		HE_TYPE_CLOCK_UP = 0xC,
		HE_TYPE_WAYPOINT = 0xD,
		HE_TYPE_COUNT = 0xE,
	}he_type_t;

	/* 6853 */
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
		int font;
		int alignOrg;
		int alignScreen;
		hudelem_color_t color;
		hudelem_color_t fromColor; //0x28
		int fadeStartTime; //0x2c
		int fadeTime;
		int label;
		int width;
		int height; //0x3C
		int materialIndex;
		int offscreenMaterialIdx; //0x44
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
		hudelem_color_t glowColor; //0x84
		int fxBirthTime;
		int fxLetterTime;
		int fxDecayStartTime;
		int fxDecayDuration;
		int soundID;
		int flags;
	}hudelem_t;

	typedef struct hudElemState_s
	{
		hudelem_t current[MAX_HUDELEMENTS];
		hudelem_t archival[MAX_HUDELEMENTS];
	}hudElemState_t;



	struct playerState_s {
		int		commandTime;  // 0
		int		pm_type;  // 4
		int		bobCycle;  // 8
		int		pm_flags;  // 12
		int		weapFlags;  // 16
		int		otherFlags;  // 20
		int		pm_time;  // 24
		vec3_t		origin;  // 28
		vec3_t		velocity;  // 32
		vec2_t		oldVelocity; //36
		int		weaponTime;  // 40
		int		weaponDelay;  // 44
		int		grenadeTimeLeft;  // 48
		int		throwBackGrenadeOwner;  // 52
		int		throwBackGrenadeTimeLeft;  // 56
		int		weaponRestrictKickTime;  // 60
		int		foliageSoundTime;  // 64
		int		gravity;  // 68
		float	leanf;  // 72
		int		speed;  // 76
		vec3_t		delta_angles;  // 80
		int		groundEntityNum;  // 84
		vec3_t		vLadderVec;  // 88
		int			jumpTime;  // 92
		float		jumpOriginZ;  // 96
		int		movementDir;  // 100
		int		eFlags;  // 104
		int		eventSequence;  // 108
		int		events[4]; // 124
		unsigned int eventParms[4]; //140
		int		oldEventSequence; //144
		int		clientNum;  // 148
		int		offHandIndex;  // 152
		OffhandSecondaryClass_t	offhandSecondary;  // 156
		unsigned int weapon;  // 160
		int		weaponstate;  // 164
		unsigned int weaponShotCount;  // 168
		float	fWeaponPosFrac;  // 172
		int		adsDelayTime;  // 176
		int		spreadOverride;  // 180
		int		spreadOverrideState;  // 256
		int		viewmodelIndex;  // 260
		vec3_t		viewangles;  // 264
		int		viewHeightTarget;  // 276
		float	viewHeightCurrent;  // 280
		int		viewHeightLerpTime;  // 284
		int		viewHeightLerpTarget;  // 288
		int		viewHeightLerpDown;  // 292
		vec2_t		viewAngleClampBase;  // 296
		vec2_t		viewAngleClampRange;  // 304
		int		damageEvent;  // 312
		int		damageYaw;  // 316
		int		damagePitch;  // 320
		int		damageCount;  // 324
		int		stats[5]; // 328
		int		ammo[128]; // 348
		int		ammoclip[128]; // 860, 0x35C
		unsigned int	weapons[4]; // 1372 0x55C
		unsigned int	weaponold[4]; // 1388 0x56C
		unsigned int	weaponrechamber[4]; // 1404 0x57C
		float		proneDirection;  // 1420
		float		proneDirectionPitch;  // 1424
		//float		proneTorsoPitch;  // 1428
		ViewLockTypes_t		viewlocked;  // 1432
		int		viewlocked_entNum;  // 1436
		int		cursorHint;  // 1440
		int		cursorHintString;  // 1444
		int		cursorHintEntIndex;  // 1448
		int		iCompassPlayerInfo;  // 1452
		int		radarEnabled;  // 1456
		int		locationSelectionInfo;  // 1460
		sprintState_t	sprintState;  // 1464
		float		fTorsoPitch;  // 1484
		float		fWaistPitch;  // 1488
		float		holdBreathScale;  // 1492
		int		holdBreathTimer;  // 1496
		float		moveSpeedScaleMultiplier;  // 1500
		mantleState_t	mantleState;  // 1504
		float		meleeChargeYaw;  // 1520
		int		meleeChargeDist;  // 1524
		int		meleeChargeTime;  // 1528
		int		perks;  // 1532
		ActionSlotType_t	actionSlotType[4];  // 1536
		ActionSlotParam_t		actionSlotParam[4];  // 1552
		int		entityEventSequence; // 1568
		int		weapAnim;  // 1572 0x624
		float	aimSpreadScale;  // 1576
		int		shellshockIndex;  // 1580
		int		shellshockTime;  // 1584
		int		shellshockDuration;  // 1588
		float		dofNearStart;  // 1592
		float		dofNearEnd;  // 1596
		float		dofFarStart;  // 1600
		float		dofFarEnd;  // 1604
		float		dofNearBlur;  // 1608
		float		dofFarBlur;  // 1612
		float		dofViewmodelStart;  // 1616
		float		dofViewmodelEnd;  // 1620
		int			hudElemLastAssignedSoundID;  // 1624
		objective_t objective[16];
		char weaponmodels[128];
		int		deltaTime;  // 2204
		int		killCamEntity;  // 2208
		hudElemState_t hud;// 2212
	};

	struct EntHandle
	{
		unsigned __int16 number;
		unsigned __int16 infoIndex;
	};

	struct gclient_s {
		playerState_s predictedPlayerState;
		unsigned char __pad0[0xB0];
		int flags; // 0xAD54
		unsigned char __pad1[0x27C];

	};

	struct scr_vehicle_s;

	struct gentity_s {
		entityState_s s;
		entityShared_t r;
		gclient_s* client;
		char _pad1[0x8];
		scr_vehicle_s* vehicle;
		char _pad2[0x38];
		int health;
		int maxHealth;
		char _pad3[0x1C0];
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
		float normal[3];
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
		float forward[3];
		float right[3];
		float up[3];
		float frametime;
		int msec;
		int walking;
		int groundPlane;
		int almostGroundPlane;
		trace_t groundTrace;
		float impactSpeed;
		float previous_origin[3];
		float previous_velocity[3];
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
		int proneChange;
		float maxSprintTimeMultiplier;
		bool mantleStarted;
		bool test1;
		bool test2;
		bool test3;
		float mantleEndPos[3];
		int mantleDuration;
		int viewChangeTime;
		float viewChange;
		char handler;
	};

	//26.10.22
	/*
	static constexpr auto MAX_GPAD_COUNT = 1;
	static constexpr auto GPAD_VALUE_MASK = 0xFFFFFFFu;
	static constexpr auto GPAD_DPAD_MASK = XINPUT_GAMEPAD_DPAD_UP | XINPUT_GAMEPAD_DPAD_DOWN | XINPUT_GAMEPAD_DPAD_LEFT | XINPUT_GAMEPAD_DPAD_RIGHT;
	static constexpr auto GPAD_DIGITAL_MASK = 1u << 28;
	static constexpr auto GPAD_ANALOG_MASK = 1u << 29;
	static constexpr auto GPAD_STICK_MASK = 1u << 30;

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
		field_t chatField;
		int chat_team;
		int overstrikeMode;
		int anyKeyDown;
		KeyState keys[256];
		LocSelInputState locSelInputState;
	};

	enum LocSelInputState
	{
		LOC_SEL_INPUT_NONE = 0x0,
		LOC_SEL_INPUT_CONFIRM = 0x1,
		LOC_SEL_INPUT_CANCEL = 0x2,
	};

	enum uiMenuCommand_t
	{
		UIMENU_NONE = 0x0,
		UIMENU_MAIN = 0x1,
		UIMENU_INGAME = 0x2,
		UIMENU_PREGAME = 0x3,
		UIMENU_POSTGAME = 0x4,
		UIMENU_SCRIPT_POPUP = 0x5,
		UIMENU_SCOREBOARD = 0x6,
		UIMENU_PARTY = 0x7,
		UIMENU_GAMELOBBY = 0x8,
		UIMENU_PRIVATELOBBY = 0x9,
		UIMENU_ENDOFGAME = 0xA,
		UIMENU_MIGRATION = 0xB,
	};

	enum KeyCatch_t
	{
		KEYCATCH_MASK_ANY = -1,
		KEYCATCH_CONSOLE = 0x1,
		KEYCATCH_UNKNOWN2 = 0x2,
		KEYCATCH_UNKNOWN4 = 0x4,
		KEYCATCH_LOCATION_SELECTION = 0x8,
		KEYCATCH_UI = 0x10,
		KEYCATCH_CHAT = 0x20,
		KEYCATCH_UNKNOWN40 = 0x40,
		KEYCATCH_UNKNOWN80 = 0x80,
		KEYCATCH_UNKNOWN100 = 0x100,
	};

	enum keyNum_t
	{
		K_NONE = 0x0,
		K_FIRSTGAMEPADBUTTON_RANGE_1 = 0x1, // First Gamepad 1
		K_BUTTON_A = 0x1,
		K_BUTTON_B = 0x2,
		K_BUTTON_X = 0x3,
		K_BUTTON_Y = 0x4,
		K_BUTTON_LSHLDR = 0x5,
		K_BUTTON_RSHLDR = 0x6,
		K_LASTGAMEPADBUTTON_RANGE_1 = 0x6, // Last Gamepad 1
		K_TAB = 0x9,
		K_ENTER = 0xD,
		K_FIRSTGAMEPADBUTTON_RANGE_2 = 0xE, // First Gamepad 2
		K_BUTTON_START = 0xE,
		K_BUTTON_BACK = 0xF,
		K_BUTTON_LSTICK = 0x10,
		K_BUTTON_RSTICK = 0x11,
		K_BUTTON_LTRIG = 0x12,
		K_BUTTON_RTRIG = 0x13,
		K_FIRSTDPAD = 0x14, // First Dpad
		K_DPAD_UP = 0x14,
		K_DPAD_DOWN = 0x15,
		K_DPAD_LEFT = 0x16,
		K_DPAD_RIGHT = 0x17,
		K_LASTDPAD = 0x17, // Last Dpad
		K_DPAD_LEFTRIGHT = 0x18,
		K_DPAD_UPDOWN = 0x19,
		K_LASTGAMEPADBUTTON_RANGE_2 = 0x19, // Last Gamepad 2
		K_ESCAPE = 0x1B,
		K_FIRSTGAMEPADBUTTON_RANGE_3 = 0x1C, // First Gamepad 3
		K_FIRSTAPAD = 0x1C, // First APad
		K_APAD_UP = 0x1C,
		K_APAD_DOWN = 0x1D,
		K_APAD_LEFT = 0x1E,
		K_APAD_RIGHT = 0x1F,
		K_LASTAPAD = 0x1F, // Last APad
		K_LASTGAMEPADBUTTON_RANGE_3 = 0x1F, // Last Gamepad 3
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

	enum GamePadStick
	{
		GPAD_INVALID = 0x0,
		GPAD_LX = GPAD_STICK_MASK | (0 & GPAD_VALUE_MASK),
		GPAD_LY = GPAD_STICK_MASK | (1 & GPAD_VALUE_MASK),
		GPAD_RX = GPAD_STICK_MASK | (2 & GPAD_VALUE_MASK),
		GPAD_RY = GPAD_STICK_MASK | (3 & GPAD_VALUE_MASK),
	};

	enum GamePadButtonEvent
	{
		GPAD_BUTTON_RELEASED = 0x0,
		GPAD_BUTTON_PRESSED = 0x1,
		GPAD_BUTTON_UPDATE = 0x2,
	};

	enum GamepadPhysicalAxis
	{
		GPAD_PHYSAXIS_NONE = -1,
		GPAD_PHYSAXIS_RSTICK_X = 0x0,
		GPAD_PHYSAXIS_RSTICK_Y = 0x1,
		GPAD_PHYSAXIS_LSTICK_X = 0x2,
		GPAD_PHYSAXIS_LSTICK_Y = 0x3,
		GPAD_PHYSAXIS_RTRIGGER = 0x4,
		GPAD_PHYSAXIS_LTRIGGER = 0x5,

		GPAD_PHYSAXIS_COUNT,
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

	struct StickToCodeMap_t
	{
		GamePadStick padStick;
		int posCode;
		int negCode;
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
	*/

	//30.10.22
	struct CachedAssets_t
	{
		Font_s* bigFont;
		Font_s* smallFont;
		Font_s* consoleFont;
		Font_s* boldFont;
		Font_s* textFont;
		Font_s* extraBigFont;
		Font_s* objectiveFont;
		Font_s* normalFont;
		snd_alias_list_t* itemFocusSound;
	};

	struct sharedUiInfo_t
	{
		CachedAssets_t assets;
	};
	#define uiMem (*((sharedUiInfo_t*)(0x128F9C4)))

#pragma pack(push, 1)
	typedef struct
	{
		union VariableUnion u;
		int type;
	}VariableValue;

#pragma pack(pop)

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

	union PackedTexCoords
	{
		unsigned int packed;
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

	struct GfxDebugPlume
	{
		float origin[3];
		float color[4];
		int score;
		int startTime;
		int duration;
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
		int anim[6];
		int control[6];
		int worldCtrl[6];
		int skel[6];
	};

	struct DObjAnimMat
	{
		float quat[4];
		float trans[3];
		float transWeight;
	};

	struct DSkel
	{
		DSkelPartBits partBits;
		int timeStamp;
		DObjAnimMat* mat;
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

	struct XSurfaceCollisionAabb
	{
		unsigned __int16 mins[3];
		unsigned __int16 maxs[3];
	};

	struct XSurfaceCollisionNode
	{
		XSurfaceCollisionAabb aabb;
		unsigned __int16 childBeginIndex;
		unsigned __int16 childCount;
	};

	struct XSurfaceCollisionLeaf
	{
		unsigned __int16 triangleBeginIndex;
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
		unsigned __int16 boneOffset;
		unsigned __int16 vertCount;
		unsigned __int16 triOffset;
		unsigned __int16 triCount;
		XSurfaceCollisionTree* collisionTree;
	};

	struct XSurfaceVertexInfo
	{
		__int16 vertCount[4];
		unsigned __int16* vertsBlend;
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
		int partBits[6];
	};

	struct XModelSurfs
	{
		const char* name;
		XSurface* surfs;
		unsigned __int16 numsurfs;
		int partBits[6];
	};

	struct XModelLodInfo
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
	};

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

#pragma pack(push, 4)
	typedef struct
	{
		XAnim_s* anims;
		uint16_t children;
		uint16_t pad;
	}XAnimTree_s;
#pragma pack(pop)



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

#pragma pack(push, 4)
	typedef struct weaponInfo_s
	{
		DObj_s* viewModelDObj; //4
		XModel* handModel; //8
		XModel* gogglesModel; //12
		XModel* rocketModel; //16
		XModel* knifeModel; //20
		char weapModelIdx; //24
		char pad[3];
		int partBits[4]; // 40
		int iPrevAnim; //44
		XAnimTree_s* tree; //48
		int registered; //52
		gitem_s* item; //56
		const char* translatedDisplayName; //60
		const char* translatedModename; //64
		const char* translatedAIOverlayDescription; //68
	}weaponInfo_t;
#pragma pack(pop)


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
		int ping;
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
		Material* friendlyfireMaterial;
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
		snd_alias_list_t* stepSprintSound[58];
		snd_alias_list_t* stepSprintSoundPlayer[58];
		snd_alias_list_t* stepRunSound[58];
		snd_alias_list_t* stepRunSoundPlayer[58];
		snd_alias_list_t* stepWalkSound[58];
		snd_alias_list_t* stepWalkSoundPlayer[58];
		snd_alias_list_t* stepProneSound[58];
		snd_alias_list_t* stepProneSoundPlayer[58];
		snd_alias_list_t* landSound[58];
		snd_alias_list_t* landSoundPlayer[58];
		snd_alias_list_t* qsprintingEquipmentSound;
		snd_alias_list_t* qsprintingEquipmentSoundPlayer;
		snd_alias_list_t* qrunningEquipmentSound;
		snd_alias_list_t* qrunningEquipmentSoundPlayer;
		snd_alias_list_t* qwalkingEquipmentSound;
		snd_alias_list_t* qwalkingEquipmentSoundPlayer;
		snd_alias_list_t* sprintingEquipmentSound;
		snd_alias_list_t* sprintingEquipmentSoundPlayer;
		snd_alias_list_t* runningEquipmentSound;
		snd_alias_list_t* runningEquipmentSoundPlayer;
		snd_alias_list_t* walkingEquipmentSound;
		snd_alias_list_t* walkingEquipmentSoundPlayer;
		snd_alias_list_t* foliageMovement;
		snd_alias_list_t* bulletWhizby;
		snd_alias_list_t* meleeSwingLarge;
		snd_alias_list_t* meleeSwingSmall;
		snd_alias_list_t* meleeHit;
		snd_alias_list_t* meleeHitOther;
		snd_alias_list_t* nightVisionOn;
		snd_alias_list_t* nightVisionOff;
		snd_alias_list_t* playerSprintGasp;
		snd_alias_list_t* playerHeartBeatSound;
		snd_alias_list_t* playerBreathInSound;
		snd_alias_list_t* playerBreathOutSound;
		snd_alias_list_t* playerBreathGaspSound;
		snd_alias_list_t* playerSwapOffhand;
		snd_alias_list_t* physCollisionSound[50][29];
		Material* compassping_friendlyfiring;
		Material* compassping_friendlyyelling;
		Material* compassping_enemy;
		Material* compassping_enemyfiring;
		Material* compassping_enemyyelling;
		Material* compassping_grenade;
		Material* compassping_explosion;
		Material* compass_radarline;
		Material* compass_helicopter_friendly;
		Material* compass_helicopter_enemy;
		Material* compass_plane_friendly;
		Material* compass_plane_enemy;
		Material* grenadeIconFrag;
		Material* grenadeIconFlash;
		Material* grenadeIconThrowBack;
		Material* grenadePointer;
		Material* offscreenObjectivePointer;
		int* fx;
		FxEffectDef* fxNoBloodFleshHit;
		FxEffectDef* fxKnifeBlood;
		FxEffectDef* fxKnifeNoBlood;
		FxEffectDef* heliDustEffect;
		FxEffectDef* heliWaterEffect;
		FxEffectDef* helicopterLightSmoke;
		FxEffectDef* helicopterHeavySmoke;
		FxEffectDef* helicopterOnFire;
		FxEffectDef* jetAfterburner;
		FxEffectDef* fxVehicleTireDust;
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

	enum svscmd_type
	{
		SV_CMD_CAN_IGNORE = 0x0,
		SV_CMD_RELIABLE = 0x1,
	};

	//static_assert(sizeof(cg_s) == 760);
	//static_assert(offsetof(cg_s, nextState) == 28);


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
}
