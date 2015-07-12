
#include <exception>
#include <string>
#include <boost/filesystem.hpp>

#include "Utils.h"

using std::string;
using std::wstring;
namespace fs = boost::filesystem;

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <locale>
#include <codecvt>

fs::path getExecutablePath() {
    wchar_t buffer[MAX_PATH];
    int size = GetModuleFileNameW(NULL, buffer, MAX_PATH);
    if (size <= 0)
        throw std::exception("Could not determine path of executable");
    return fs::path(buffer);
}

#elif defined(__linux__)

#include <stdlib.h>
const static string proc_self_exe = "/proc/self/exe";

fs::path getExecutablePath() {
    char * buffer = realpath("/proc/self/exe", nullptr);
    string path(buffer);
    free(buffer);
    return fs::path(path);
}


#endif

fs::path getExecutableDir() {
    return getExecutablePath().parent_path();
}
