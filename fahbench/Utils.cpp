
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

string getExecutablePath() {
    wchar_t buffer[MAX_PATH];
    int size = GetModuleFileNameW(NULL, buffer, MAX_PATH);
    if (size <= 0)
        throw std::exception("Could not determine path of executable");

    // Convert a wchar_t string to a normal string
    // This may or may not be legit
    wstring wret = wstring(buffer);
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    return converter.to_bytes(wret);

}

string getExecutableDir() {
    wstring wret = fs::path(getExecutablePath()).parent_path().native();
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    return converter.to_bytes(wret);
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

string getExecutableDir() {
    return fs::path(getExecutablePath()).parent_path().native();
}

#endif
