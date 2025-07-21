#pragma once

#include <Python.h>

namespace python
{
    PyObject* py_select_assembly(PyObject* self, PyObject* name);
    PyObject* py_il2cpp_init();
}