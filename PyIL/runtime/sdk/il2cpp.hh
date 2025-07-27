#pragma once

#include <Python.h>

namespace sdk::pyil2cpp
{
    PyObject* py_select_assembly(PyObject* self, PyObject* name);
    PyObject* py_get_assembly(PyObject* self, PyObject* _);
    PyObject* py_init();
}