#pragma once

#include <format>

#include <Windows.h>

namespace console
{
    inline bool _alloced = false;
    inline bool _no_colours = false;

    inline HANDLE out_handle = INVALID_HANDLE_VALUE;
    inline HANDLE inp_handle = INVALID_HANDLE_VALUE;
    inline HANDLE err_handle = INVALID_HANDLE_VALUE;

    bool alloc();
    void free();

    std::string read(std::string_view out = "");

    template<typename... TArgs>
    void write(std::string_view msg, TArgs&&... args)
    {
        if (!_alloced && !alloc())
            return;
            
        if (!out_handle || out_handle == INVALID_HANDLE_VALUE)
            return;

        std::string fmted = std::vformat(msg, std::make_format_args(std::forward<TArgs>(args)...));
        
        DWORD wrote;
        WriteConsole(out_handle, fmted.c_str(), (DWORD)fmted.size(), &wrote, NULL);
    }

    template<typename... Args>
    inline void log(std::string colour, std::string prefix, std::string msg, Args... args) 
    { 
        std::string reset = "\033[0m";
        if (_no_colours)
        {
            colour = "";
            reset = "";
        }

        write(std::format("{}{}{}{}\n", colour, prefix, std::vformat(msg, std::make_format_args(args...)), reset));
    }

    template<typename... Args>
    inline void log_info(std::string msg, Args... args) { log("\033[34m", "[INFO] ", msg, args...); }
    template<typename... Args>
    inline void log_warning(std::string msg, Args... args) { log("\033[33m", "[WARN] ", msg, args...); }
    template<typename... Args>
    inline void log_error(std::string msg, Args... args) { log("\033[31m", "[ERROR] ", msg, args...); }
    template<typename... Args>
    inline void log_important(std::string msg, Args... args) { log("\033[35m", "[IMPORTANT] ", msg, args...); }

    void clear();
}