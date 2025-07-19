#include <iostream>
#include <format>

#include <windows.h>

#include "il2cpp/il2cpp.hh"
#include "python/python.hh"

#include "console.hh"

void entry()
{
    pyil::console::init();

    if (!il2cpp::init())
        return;

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