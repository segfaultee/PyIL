#include <string>
#include <filesystem>

#include <Windows.h>

namespace utils
{
    std::filesystem::path get_cwd(bool set_cwd)
    {
        std::vector<char> raw_path(MAX_PATH + 1);
        DWORD len = GetModuleFileNameA(NULL, raw_path.data(), static_cast<DWORD>(raw_path.size()));
        if (len == 0 || len >= raw_path.size())
            return {};

        auto parent_path = std::filesystem::path(raw_path.data()).parent_path();

        if (set_cwd)
            SetCurrentDirectoryA(parent_path.string().c_str());

        return parent_path;
    }
}
