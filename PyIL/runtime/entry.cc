#include <iostream>
#include <filesystem>
#include <format>

#include <Windows.h>

#include <pylight/python.hh>
#include <il2cpp/il2cpp.hh>

#include "sdk/il2cpp.hh"
#include "utils/path.hh"

#include "console.hh"

static std::filesystem::path MODS_DIRECTORY;
static HMODULE mod;

void pyil_exit(int code = 0)
{
    python::deinit();
    
    console::read("Exit initiated, press enter to finalise.");
    console::free();

    FreeLibraryAndExitThread(mod, code);
}

bool init_il2cpp()
{
    if (!il2cpp::init())
        return false;

    if (il2cpp::_assemblies.find("Assembly-CSharp") != il2cpp::_assemblies.end())
        il2cpp::select_assembly("Assembly-CSharp");
    
    return true;
}

bool init_python()
{
    if (!python::init())
        return false;

    if (auto res = python::insert_path(MODS_DIRECTORY.string()); res.is_error())
    {
        console::log_error("[PYTHON]: {}", res.error_message());
        return false;
    }

    return true;
}

auto init_python_modules()
{
    auto pyil_module_res = python::Module::from_dotted("pyil");
    auto bootstrap_module_res = python::Module::from_dotted("pyil.bootstrap");
    auto il2cpp_module_res = python::Module::from_object(sdk::pyil2cpp::py_init());
    
    if (pyil_module_res.is_error()) { console::log_error("[PYTHON]: {}", pyil_module_res.error_message()); pyil_exit(1); }
    if (bootstrap_module_res.is_error()) { console::log_error("[PYTHON]: {}", bootstrap_module_res.error_message()); pyil_exit(1); }
    if (il2cpp_module_res.is_error()) { console::log_error("[PYTHON]: {}", il2cpp_module_res.error_message()); pyil_exit(1); }

    auto pyil_module = pyil_module_res.get();
    auto bootstrap_module = bootstrap_module_res.get();
    auto il2cpp_module = il2cpp_module_res.get();

    il2cpp_module.add_object("__name__", PyUnicode_FromString("pyil.il2cpp"));
    il2cpp_module.add_to_sys_modules("pyil.il2cpp");

    if (auto res = pyil_module.add_object("il2cpp", il2cpp_module.get()); res.is_error())
    {
        std::cout << "[PYTHON] ERROR: " << res.error_message(); 
        pyil_exit(1);
    }

    return std::make_tuple(
        pyil_module, 
        bootstrap_module, 
        il2cpp_module
    );
}

bool init_environment(python::Module& bootstrap_module)
{
    if (auto res = bootstrap_module.call("_set_cwd", python::pack_tuple(MODS_DIRECTORY.string())); res.is_error()) { console::log_error("[PYTHON]: {}", res.error_message()); return false; }
    if (auto res = bootstrap_module.call("setup_logger", nullptr); res.is_error()) { console::log_error("[PYTHON]: {}", res.error_message()); return false; }
    return true;
}

void entry()
{
    MODS_DIRECTORY = utils::get_cwd() / "pyil" / "mods";
    console::alloc();
    
    if (!init_il2cpp() || !init_python())
        return;

    auto [pyil_module, bootstrap_module, il2cpp_module] = init_python_modules();

    if (!init_environment(bootstrap_module))
        return;

    auto entry_module = python::Module::from_dotted("mod_entry");
    if (entry_module.is_error()) 
    { 
        console::log_error("[PYTHON]: {}", entry_module.error_message()); 
        pyil_exit(0); 
    }

    while (true)
    {
        auto input = console::read();
        if (input.starts_with("detach"))
            break;
    }
    
    pyil_exit(0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID)
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
        {
            mod = hModule;
            DisableThreadLibraryCalls(hModule);
            CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)entry, nullptr, 0, nullptr);
        } break;
    }
    return TRUE;
}