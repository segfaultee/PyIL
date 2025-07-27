#pragma once

#include <iostream>
#include <string>

#include <windows.h>

namespace console
{
    inline HANDLE out_handle = INVALID_HANDLE_VALUE;

    inline void init()
    {
        AllocConsole();

        out_handle = CreateFileA(
            "CONOUT$", GENERIC_WRITE, FILE_SHARE_WRITE, nullptr,
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr
        );

        FILE* f;
        freopen_s(&f, "CONOUT$", "w", stdout);

        std::ios::sync_with_stdio();
    }

    inline void deinit()
    {
        if (out_handle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(out_handle);
            out_handle = INVALID_HANDLE_VALUE;
        }

        FreeConsole();
    }

    inline void write(const std::string& msg)
    {
        DWORD w;
        WriteConsoleA(out_handle, msg.c_str(), (DWORD)msg.size(), &w, nullptr);
    }
}