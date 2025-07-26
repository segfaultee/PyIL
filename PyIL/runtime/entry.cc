#include <iostream>
#include <filesystem>
#include <format>

#include <Windows.h>

#include <pylight/python.hh>
#include <il2cpp/il2cpp.hh>

#include "utils/path.hh"

#include "console.hh"

void entry()
{
    std::filesystem::path mod_dir = utils::get_cwd() / "pyil" / "mods";

    pyil::console::init();

    if (!il2cpp::init())
        return;

    if (il2cpp::_assemblies.find("Assembly-CSharp") != il2cpp::_assemblies.end())
        il2cpp::select_assembly("Assembly-CSharp");

    if (!python::init())
        return;

    if (auto res = python::insert_path(mod_dir.string()); res.is_error())
    {
        std::cout << "[PYTHON] ERROR: " << res.error_message();
        return;
    }

    auto pyil_module = python::Module::from_dotted("pyil");
    auto bootstrap_module = python::Module::from_path("pyil.bootstrap");
    auto entry_module = python::Module::from_dotted("mod_entry");

    if (pyil_module.is_error()) { std::cout << "[PYTHON] ERROR: " << pyil_module.error_message(); return; }
    if (bootstrap_module.is_error()) { std::cout << "[PYTHON] ERROR: " << bootstrap_module.error_message(); return; }
    if (entry_module.is_error()) { std::cout << "[PYTHON] ERROR: " << entry_module.error_message(); return; }
    
    auto args = python::pack_tuple(mod_dir.string());
    if (args == nullptr)
    {
        std::cout << "[PYTHON] ERROR: Failed to pack tuple for set_cwd call\n";
        return;
    }

    if (bootstrap_module.get().call("_set_cwd", args).is_error())
    {
        std::cout << "[PYTHON] ERROR: _set_cwd failed\n";
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