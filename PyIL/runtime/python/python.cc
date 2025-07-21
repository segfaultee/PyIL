#include <iostream>
#include <format>

#include <Python.h>

#include "il2cpp/il2cpp.hh"

#include "mod_inject.hh"
#include "python.hh"

namespace python
{
    bool init()
    {
        for (const auto& [name, init] : modules)
        {
            if (PyImport_AppendInittab(name.c_str(), init) == -1) 
            {
                std::cout << std::format("Failed to register {} module in inittab\n", name);
                return false;
            }
        }

        Py_Initialize();
        inject_module_into("il2cpp", "pyil");
        
        return true;
    }

    void deinit()
    {
        Py_Finalize();
    }
}