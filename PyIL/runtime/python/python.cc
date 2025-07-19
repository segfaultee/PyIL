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
        PyImport_AppendInittab("pyil", sdk::module_init);
        Py_Initialize();
        PyRun_SimpleString("import pyil;print(pyil.__dir__())");
        return true;
    }
}