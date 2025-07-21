#include <iostream>
#include <filesystem>

#include <Windows.h>

HANDLE open(std::filesystem::path bin_path)
{
    if (!std::filesystem::exists(bin_path))
        return INVALID_HANDLE_VALUE;

    STARTUPINFOA si = { 0 };
    PROCESS_INFORMATION pi = { 0 };
    si.cb = sizeof(si);

    if (!CreateProcessA(bin_path.string().data(), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
        return INVALID_HANDLE_VALUE;

    return pi.hProcess;
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout << "Expected 2 arguments: game_path, game_binary\n";
        return 1;
    }

    std::filesystem::path game_path = std::filesystem::path(argv[1]);
    std::filesystem::path pyil_dll_path = game_path / "pyil" / "PyIL-RT.dll";

    char* game_binary = argv[2];

    if (!std::filesystem::exists(game_path) || !std::filesystem::is_directory(game_path))
    {
        std::cout << "game path is not a valid directory.\n";
        return 1;
    }

    if (!std::filesystem::exists(game_path / "pyil"))
    {
        std::cout << game_path / "pyil" << " doesn't exist\n";
        return 1;
    }
    
    if (!std::filesystem::exists(pyil_dll_path))
    {
        std::cout << pyil_dll_path << " doesn't exist\n";
        return 1;
    }

    HANDLE handle = open(game_path / game_binary);
    if (handle == INVALID_HANDLE_VALUE)
    {
        std::cout << "Failed to open " << game_path / game_binary << '\n';
        return 1;
    }

    const char* data = pyil_dll_path.string().c_str();
    void* remote_mem = VirtualAllocEx(handle, nullptr, strlen(data), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if (!remote_mem)
    {
        std::cout << "Failed to allocate remote memory of size " << strlen(data) << '\n'; 
        return 1;
    }

    if (!WriteProcessMemory(handle, remote_mem, data, strlen(data), nullptr))
    {
        std::cout << "Failed to write data to allocated remote memory\n";
        return 1;
    }

    FARPROC load_lib = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    if (load_lib == NULL)
    {
        std::cout << "Failed to get LoadLibraryA\n";
        return 1;
    }

    HANDLE remote_thread = CreateRemoteThread(handle, nullptr, 0, (LPTHREAD_START_ROUTINE)load_lib, remote_mem, 0, nullptr);
    if (remote_thread == INVALID_HANDLE_VALUE)
    {
        std::cout << "Failed to create remote thread\n";
        return false;
    }

    WaitForSingleObject(remote_thread, INFINITE);
    CloseHandle(remote_thread);
    
    VirtualFreeEx(handle, remote_mem, strlen(data), MEM_RELEASE);

    return 0;
}