#include "STDInc.hpp"

namespace Main
{
    //static Utils::Hook EntryPointHook_;

    void Initialize()
    {
        //Main::EntryPointHook_.uninstall();
        Components::Loader::Initialize();
    }

    void Uninitialize()
    {
        Components::Loader::Uninitialize();
    }

    __declspec(naked) void StartEntryPoint()
    {
        __asm
        {
            pushad;
            call	Main::Initialize;
            popad;

            push    64391Bh;
            push    64CB85h;
            retn;
            //mov		eax, 0x643AFB;
            //jmp		eax;
        }
    }
}

BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD  ul_reason_for_call, LPVOID /*lpReserved*/)
{
    //auto* _module = reinterpret_cast<char*>(0x400000);

    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        // < rtx begin
        //if (Components::Loader::BinaryCheck())
        // rtx end >
        {
            //DWORD oldProtect;
            //VirtualProtect(_module + 0x1000, 0x0FB7000, PAGE_EXECUTE_READWRITE, &oldProtect);

            //Main::EntryPointHook_.initialize(0x643AFB, start_entry_point)->install();

            SetProcessDEPPolicy(PROCESS_DEP_ENABLE);
            std::srand(std::uint32_t(std::time(nullptr)) ^ ~(GetTickCount() * GetCurrentProcessId()));
            Utils::SetEnvironment();
            Utils::Hook(0x643AFB, Main::StartEntryPoint, HOOK_JUMP).install()->quick();
        }
        // < rtx begin
        //else
        //    return FALSE;
        // rtx end >
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        Main::Uninitialize();
    }

    return TRUE;
}