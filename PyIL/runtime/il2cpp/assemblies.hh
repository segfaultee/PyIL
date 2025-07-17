#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

namespace il2cpp
{
    inline uintptr_t _target_asm = 0;
    inline std::unordered_map<std::string, uintptr_t> _assemblies{}; 

    inline void select_assembly(std::string name) { _target_asm = _assemblies[name]; }

    bool _populate_assemblies();
}