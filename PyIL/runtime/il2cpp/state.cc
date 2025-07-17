#include <iostream>
#include <format>

#include <windows.h>

#include <minhook.h>

#include "il2cpp.hh"

#define CALL_MINHOOK(f, return_false, ...) \
{ \
    if (auto status = f(__VA_ARGS__); status != MH_OK) \
    { \
        std::cout << std::format("[IL2CPP:MINHOOK] ERROR: {} Failed ({})", #f, MH_StatusToString(status)); \
        if (return_false) \
            return false; \
    } \
}

namespace il2cpp
{
    uintptr_t (*O_rt_invoke)(uintptr_t, uintptr_t, uintptr_t, uintptr_t) = nullptr;
    uintptr_t HK_rt_invoke(uintptr_t method, uintptr_t obj, uintptr_t params, uintptr_t exc)   
    {
        if (!_is_ready.load())
            _is_ready.store(true);
        return O_rt_invoke(method, obj, params, exc);
    }
    
    bool _block_until_ready(int ms_timeout)
    {
        CALL_MINHOOK(MH_Initialize, true)
        CALL_MINHOOK(MH_CreateHook, true, il2cpp::runtime_invoke, HK_rt_invoke, (LPVOID*)&O_rt_invoke)
        CALL_MINHOOK(MH_EnableHook, true, il2cpp::runtime_invoke)
        
        for (int i = 0; i < ms_timeout / 100 && !_is_ready.load(); i++)
            Sleep(100);

        CALL_MINHOOK(MH_DisableHook, false, il2cpp::runtime_invoke)
        CALL_MINHOOK(MH_RemoveHook, false, il2cpp::runtime_invoke)
        CALL_MINHOOK(MH_Uninitialize, false)

        return _is_ready.load();
    }
}