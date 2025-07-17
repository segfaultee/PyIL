#pragma once

#include <cstdint>

namespace il2cpp::debug
{
    void dump_class_methods(uintptr_t cls);
    void dump_class_fields(uintptr_t cls);
    void dump_class_properties(uintptr_t cls);
    
    void dump_class_methods(const char* namespaze, const char* name);
    void dump_class_fields(const char* namespaze, const char* name);
    void dump_class_properties(const char* namespaze, const char* name);
}