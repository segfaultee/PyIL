#include "il2cpp/il2cpp.hh"

#include "py_il2cpp.hh"

namespace python
{
    PyObject* py_select_assembly(PyObject* self, PyObject* name)
    {
        if (!PyUnicode_Check(name))
        {
            PyErr_SetString(PyExc_TypeError, "name must be a string");
            return nullptr;
        }

        const char* assembly_name = PyUnicode_AsUTF8(name);
        if (!assembly_name)
            return nullptr;

        il2cpp::select_assembly(assembly_name);
        
        Py_INCREF(Py_None);
        return Py_None;
    }

    PyObject* py_il2cpp_init()
    {
        static PyMethodDef methods[] = {
            {"select_assembly", py_select_assembly, METH_O, NULL},
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
        if (m == NULL)
        {
            PyErr_Print();
            return nullptr;
        }

        return m;
    }
}