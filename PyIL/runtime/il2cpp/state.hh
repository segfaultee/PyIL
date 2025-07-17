#pragma once

#include <atomic>

namespace il2cpp
{
    inline std::atomic<bool> _is_ready = false;
    
    bool _block_until_ready(int ms_timeout);
}