#include <iostream>
#include <format>

#include <Python.h>

#include "il2cpp/il2cpp.hh"

#include "python.hh"

namespace python
{
    bool inject_module_into(const char* target_module_name, const char* container_module_name)
    {
        PyObject* target_module = PyImport_ImportModule(target_module_name);
        if (!target_module) 
        {
            PyErr_Print();
            return false;
        }

        PyObject* container_module = PyImport_ImportModule(container_module_name);
        if (!container_module) 
        {
            Py_DECREF(target_module);
            PyErr_Print();
            return false;
        }

        if (PyModule_AddObject(container_module, target_module_name, target_module) < 0) 
        {
            Py_DECREF(container_module);
            Py_DECREF(target_module);
            PyErr_Print();
            return false;
        }

        PyObject* sys_modules = PyImport_GetModuleDict();
        std::string key = std::string(container_module_name) + "." + target_module_name;
        if (PyDict_SetItemString(sys_modules, key.c_str(), target_module) < 0)
        {
            Py_DECREF(container_module);
            PyErr_Print();
            return false;
        }

        Py_DECREF(container_module);
        return true;
    }

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