#include <iostream>
#include <format>

#include <windows.h>

#include <pylight/python.hh>
#include <il2cpp/il2cpp.hh>

#include "console.hh"

void entry()
{
    pyil::console::init();

    if (!il2cpp::init())
        return;

    if (il2cpp::_assemblies.find("Assembly-CSharp") != il2cpp::_assemblies.end())
        il2cpp::select_assembly("Assembly-CSharp");

    if (!python::init())
        return;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID)
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hModule);
            CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)entry, nullptr, 0, nullptr);
            break;
    }
    return TRUE;
}