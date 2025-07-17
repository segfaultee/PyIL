#include <iostream>
#include <format>

#include "il2cpp.hh"

namespace il2cpp
{
    // TODO: IL2CPP Versioning

    struct _AssemblyName
    {
        const char* name;
        const char* culture;
        const uint8_t* public_key;
        uint32_t hash_alg;
        int32_t hash_len;
        uint32_t flags;
        int32_t major;
        int32_t minor;
        int32_t build;
        int32_t revision;
        uint8_t public_key_token[8];
    };

    struct _Assembly
    {
        uintptr_t image;
        uint32_t token;
        int32_t referencedAssemblyStart;
        int32_t referencedAssemblyCount;
        _AssemblyName aname;
    };

    bool _populate_assemblies()
    {
        if (!il2cpp::thread_current())
        {
            std::cout << "[IL2CPP] ERROR: Attempted to populate assemblies while thread_current == 0\n";
            return false;
        }

        size_t assembly_count = 0;
        _Assembly** assemblies = (_Assembly**)il2cpp::domain_get_assemblies(il2cpp::domain_get(), &assembly_count);

        for (size_t i = 0; i < assembly_count; ++i) 
        {
            const char* name = assemblies[i]->aname.name;
            uintptr_t image = il2cpp::assembly_get_image((uintptr_t)assemblies[i]);

            _assemblies[name] = image;
            
            std::cout << std::format("[IL2CPP] INFO: Found assembly {}\n", name);
        }

        return true;
    }
}