#include <iostream>
#include <filesystem>
#include <format>

#include <windows.h>

#include <pylight/python.hh>
#include <il2cpp/il2cpp.hh>

#include "utils/path.hh"

#include "console.hh"

void entry()
{
    std::filesystem::path cwd = utils::get_cwd();
    std::filesystem::path mod_dir = cwd / "pyil" / "mods";

    pyil::console::init();

    if (!il2cpp::init())
        return;

    if (il2cpp::_assemblies.find("Assembly-CSharp") != il2cpp::_assemblies.end())
        il2cpp::select_assembly("Assembly-CSharp");

    if (!python::init())
        return;

    python::Result<void*> res = python::insert_path(mod_dir.string());
    if (res.is_error())
    {
        std::cout << "[PYTHON] ERROR: " << res.error_message();
        return;
    }
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