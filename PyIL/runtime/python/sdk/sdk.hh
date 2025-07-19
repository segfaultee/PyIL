#pragma once

#include <Python.h>

namespace python::sdk
{
    inline PyMethodDef _sdk_meths[] = {
        {NULL, NULL, 0, NULL}
    };

    inline struct PyModuleDef _sdk_mod = {
        PyModuleDef_HEAD_INIT,
        "pyil",
        NULL,
        -1,
        _sdk_meths
    };

    PyObject* module_init(void) 
    {
        PyObject* m = PyModule_Create(&_sdk_mod);
        return m;
    }
}