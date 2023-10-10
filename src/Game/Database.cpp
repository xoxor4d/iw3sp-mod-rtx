#include "STDInc.hpp"

namespace Game
{
	unsigned int* g_poolSize = reinterpret_cast<unsigned int*>(0x6DEFA0);
	const char** g_assetNames = (const char**)0x6DF440;
	XAssetHeader* DB_XAssetPool = reinterpret_cast<XAssetHeader*>(0x6DF200);
	HANDLE DatabaseHandle = reinterpret_cast<HANDLE>(0xFC6308);
	HANDLE databaseCompletedEvent2 = reinterpret_cast<HANDLE>(0xFC6348);
	//-------------------------------------------------
	DB_LoadXAssets_t DB_LoadXAssets = (DB_LoadXAssets_t)0x45B990;
	DB_EnumXAssets_t DB_EnumXAssets = DB_EnumXAssets_t(0x45A8C0);
	DB_FindXAssetHeader_t DB_FindXAssetHeader = DB_FindXAssetHeader_t(0x45AD10);
	DB_GetXAssetSizeHandler_t* DB_GetXAssetSizeHandlers = reinterpret_cast<DB_GetXAssetSizeHandler_t*>(0x6DF5F0);
	DB_GetXAssetNameHandler_t* DB_GetXAssetNameHandlers = reinterpret_cast<DB_GetXAssetNameHandler_t*>(0x6DF4D8);
	DB_BeginRecoverLostDevice_t DB_BeginRecoverLostDevice = DB_BeginRecoverLostDevice_t(0x45B180);
	DB_EndRecoverLostDevice_t DB_EndRecoverLostDevice = DB_EndRecoverLostDevice_t(0x45B210);
	DB_IsXAssetDefault_t DB_IsXAssetDefault = DB_IsXAssetDefault_t(0x45B040);
	//-------------------------------------------------
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
}