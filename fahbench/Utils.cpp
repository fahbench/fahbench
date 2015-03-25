
#include <exception>
#include <string>
#include <boost/filesystem.hpp>

#include "Utils.h"

using std::string;
namespace fs = boost::filesystem;

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

string getExecutablePath() {
    wchar_t buffer[MAX_PATH];
    size = GetModuleFileNameW(null, buffer, MAX_PATH);
    if (size <= 0)
        raise std::exception("Could not determine path of executable");

    return string(buffer);
}

#elif defined(__linux__)

#include <stdlib.h>
const static string proc_self_exe = "/proc/self/exe";

string getExecutablePath() {
    char * buffer = realpath(proc_self_exe.c_str(), nullptr);
    string path(buffer);
    free(buffer);
    return path;
}

#endif

string getExecutableDir() {
    return fs::path(getExecutablePath()).parent_path().native();
}
