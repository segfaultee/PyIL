#include <format>

#include <pylight/python.hh>
#include <il2cpp/il2cpp.hh>

#include "il2cpp.hh"

namespace sdk::pyil2cpp
{
    PyObject* py_select_assembly(PyObject* self, PyObject* name)
    {
        if (!PyUnicode_Check(name))
        {
            PyErr_SetString(PyExc_TypeError, "name must be a string");
            return nullptr;
        }

        const char* assembly_name = PyUnicode_AsUTF8(name);
        if (assembly_name == nullptr)
            return nullptr;

        if (!il2cpp::_assemblies.contains(assembly_name))
        {
            PyErr_SetString(PyExc_FileNotFoundError, std::format("assembly {} is not loaded", assembly_name).c_str());
            return nullptr;
        }
        
        il2cpp::select_assembly(assembly_name);
        
        Py_INCREF(Py_None);
        return Py_None;
    }

    PyObject* py_get_assembly(PyObject* self, PyObject* _)
    {
        for (auto&[key, val] : il2cpp::_assemblies)
            if (val == il2cpp::_target_asm)
                return PyUnicode_FromString(key.c_str());
        
        Py_INCREF(Py_None);
        return Py_None;
    }

    PyObject* py_get_assemblies(PyObject* self, PyObject* _)
    {
        auto assemblies_res = python::List::create(il2cpp::_assemblies.size());
        if (assemblies_res.is_error())
            return nullptr;

        python::List assemblies = assemblies_res.get();
        int i = 0;
        for (auto&[key, _] : il2cpp::_assemblies)
        {
            auto py_str = PyUnicode_FromString(key.c_str());
            if (py_str == nullptr)
            {
                assemblies.decref();
                return nullptr;
            }

            if (assemblies.append(py_str).is_error())
            {
                assemblies.decref();
                return nullptr;
            }   
        }

        return assemblies.get();
    }

    PyObject* py_init()
    {
        static PyMethodDef methods[] = {
            {"select_assembly", py_select_assembly, METH_O, NULL},
            {"get_assembly", py_get_assembly, METH_NOARGS, NULL},
            {"get_assemblies", py_get_assemblies, METH_NOARGS, NULL},
            {NULL, NULL, 0, NULL}
        };

        static PyModuleDef mod_def = {
            PyModuleDef_HEAD_INIT,
            "il2cpp",
            NULL,
            -1,
            methods
        };

        PyObject* m = PyModule_Create(&mod_def);
        if (m == nullptr)
        {
            PyErr_Print();
            return nullptr;
        }

        return m;
    }
}