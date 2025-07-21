#pragma once

#include <Python.h>

namespace python
{
    bool inject_module_into(const char* target_module_name, const char* container_module_name);
}