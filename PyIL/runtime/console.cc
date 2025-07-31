#include <cstdio>
#include <fstream>

#include <Windows.h>
#include <io.h>
#include <fcntl.h>

#include "console.hh"

namespace console
{
    bool alloc()
    {
        if (_alloced)
            free();

        _alloced = false;

        if (!AllocConsole())
            return false;

        SetConsoleTitleA("PyIL");
        
        console::inp_handle = CreateFileA("CONIN$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (console::inp_handle == INVALID_HANDLE_VALUE)
            return false;
        
        console::out_handle = CreateFileA("CONOUT$", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (console::out_handle == INVALID_HANDLE_VALUE)
        {
            CloseHandle(console::inp_handle);
            return false;
        }

        _alloced = true;

        DWORD mode;
        if (!GetConsoleMode(console::out_handle, &mode) || !SetConsoleMode(console::out_handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING))
        {
            console::_no_colours = true;
            console::write("Failed to enable virtual terminal processing on out handle. Colours will be disabled.\n");
        }

        FILE* f;
        freopen_s(&f, "CONOUT$", "w", stdout);
        freopen_s(&f, "CONOUT$", "w", stderr);
        freopen_s(&f, "CONIN$", "r", stdin);
    
        std::ios::sync_with_stdio(true); 
        return true;
    }

    void free()
    {    
        if (console::inp_handle && console::inp_handle != INVALID_HANDLE_VALUE)
            CloseHandle(console::inp_handle);

        if (console::out_handle && console::out_handle != INVALID_HANDLE_VALUE)
            CloseHandle(console::out_handle);

        fclose(stdin);
        fclose(stdout);
        fclose(stderr);

        FreeConsole();
        _alloced = false;
    }

    std::string read(std::string_view out)
    {
        if (!_alloced && !alloc())
            return "";
            
        if (!console::inp_handle || console::inp_handle == INVALID_HANDLE_VALUE)
            return "";

        if (!out.empty())
            console::write(out);

        char buff[256] = {0};
        DWORD read;

        if (!ReadConsole(console::inp_handle, buff, sizeof(buff), &read, NULL))
            return "";

        buff[read] = '\0';
        return std::string(buff, read);
    }

    void clear()
    {
        CONSOLE_SCREEN_BUFFER_INFO info;
        if (!GetConsoleScreenBufferInfo(console::out_handle, &info))
            return;

        auto sizep = info.dwSize.X * info.dwSize.Y;

        DWORD _;
        COORD coord = { 0, 0 }; 

        if (!FillConsoleOutputCharacter(console::out_handle, ' ', sizep, coord, &_))
            return;

        if (!FillConsoleOutputAttribute(console::out_handle, info.wAttributes, sizep, coord, &_))
            return;

        if (!SetConsoleCursorPosition(console::out_handle, coord))
            return;
    }
}