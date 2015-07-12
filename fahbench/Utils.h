#ifndef UTILS_H
#define UTILS_H

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

fs::path getExecutablePath();
fs::path getExecutableDir();

#endif // UTILS_H
