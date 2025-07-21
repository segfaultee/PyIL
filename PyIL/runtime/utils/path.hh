#pragma once

#include <filesystem>

namespace utils
{
    std::filesystem::path get_cwd(bool set_cwd = true);
}