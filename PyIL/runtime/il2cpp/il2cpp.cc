#include <iostream>
#include <format>

#include <windows.h>

#include <il2cpp/il2cpp.hh>

namespace il2cpp
{
    bool init(const char* mod, int ms_timeout)
    {
        HMODULE mhandle = nullptr;
        for (int i = 0; i < 1'000 / 100 && !(mhandle = GetModuleHandleA(mod)); i++)
            Sleep(100);

        if (!mhandle)
            return false;

        for (auto &[key, ptr]: il2cpp::_symbols)
        {
            void* f = GetProcAddress(mhandle, key.c_str());
            
            if (!f)
            {
                std::cout << std::format("[IL2CPP] ERROR: Failed to resolve {}\n", key);
                continue;
            }

            std::cout << std::format("[IL2CPP] INFO: Resolved {}\n", key);

            *ptr = f;
        }

        if (!_block_until_ready(ms_timeout))
            return false;

        il2cpp::thread_attach(il2cpp::domain_get());
        return _populate_assemblies();
    }
}