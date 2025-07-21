#include <iostream>

#include "il2cpp/il2cpp.hh"

#include "py_pyil.hh"

namespace python
{
    PyObject* py_pyil_init()
    {
        static PyMethodDef methods[] = {
            {NULL, NULL, 0, NULL}
        };

        static PyModuleDef mod_def = {
            PyModuleDef_HEAD_INIT,
            "pyil",
            NULL,
            -1,
            methods
        };

        PyObject* m = PyModule_Create(&mod_def);
        if (m == NULL)
        {
            PyErr_Print();
            return nullptr;
        }

        return m;
    }
}