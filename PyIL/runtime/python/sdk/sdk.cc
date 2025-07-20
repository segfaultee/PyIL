#include "sdk.hh"

namespace python::sdk
{
    PyObject* module_init()
    {
        PyObject* m = PyModule_Create(&_sdk_mod);
        if (m == NULL)
        {
            PyErr_Print();
            return nullptr;
        }
        return m;
    }
}