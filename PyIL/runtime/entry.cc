#include <iostream>
#include <filesystem>
#include <format>

#include <Windows.h>

#include <pylight/python.hh>
#include <il2cpp/il2cpp.hh>

#include "sdk/il2cpp.hh"
#include "utils/path.hh"

#include "console.hh"

void entry()
{
    std::filesystem::path mod_dir = utils::get_cwd() / "pyil" / "mods";

    console::init();

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

    auto pyil_module_res = python::Module::from_dotted("pyil");
    auto bootstrap_module_res = python::Module::from_dotted("pyil.bootstrap");
    auto il2cpp_module_res = python::Module::from_object(sdk::pyil2cpp::py_init());
    
    if (pyil_module_res.is_error()) { std::cout << "[PYTHON] ERROR: " << pyil_module_res.error_message(); return; }
    if (bootstrap_module_res.is_error()) { std::cout << "[PYTHON] ERROR: " << bootstrap_module_res.error_message(); return; }
    if (il2cpp_module_res.is_error()) { std::cout << "[PYTHON] ERROR: " << il2cpp_module_res.error_message(); return; }

    auto pyil_module = pyil_module_res.get();
    auto bootstrap_module = bootstrap_module_res.get();
    auto il2cpp_module = il2cpp_module_res.get();

    il2cpp_module.add_object("__name__", PyUnicode_FromString("pyil.il2cpp"));
    il2cpp_module.add_to_sys_modules("pyil.il2cpp");

    if (auto res = pyil_module.add_object("il2cpp", il2cpp_module.get()); res.is_error())
    {
        std::cout << "[PYTHON] ERROR: " << res.error_message(); 
        return;
    }
    
    if (auto res = bootstrap_module.call("_set_cwd", python::pack_tuple(mod_dir.string())); res.is_error())
    {
        std::cout << "[PYTHON] ERROR: " << res.error_message();
        return;
    }

    auto entry_module = python::Module::from_dotted("mod_entry");
    if (entry_module.is_error()) { std::cout << "[PYTHON] ERROR: " << entry_module.error_message(); return; }
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