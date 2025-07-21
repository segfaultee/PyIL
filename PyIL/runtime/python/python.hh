#pragma once

#include <map>

#include "modules/py_il2cpp.hh"
#include "modules/py_pyil.hh"

namespace python
{
    inline std::map<std::string, PyObject*(*)()> modules = {
        {"pyil", py_pyil_init},
        {"il2cpp", py_il2cpp_init}
    };

    bool init();
    void deinit();
}