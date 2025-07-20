#include <iostream>
#include <format>

#include <Windows.h>

#include <Python.h>

#include "sdk/sdk.hh"

#include "python.hh"

namespace python
{
    bool init()
    {
        if (PyImport_AppendInittab("pyil", sdk::module_init) == -1)
        {
            PyErr_Print();
            return false;
        }

        Py_Initialize();
        
        return true;
    }

    void deinit()
    {
        Py_Initialize();
    }
}