set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

add_library(boost-program_options
        boost/libs/program_options/src/cmdline.cpp
        boost/libs/program_options/src/config_file.cpp
        boost/libs/program_options/src/convert.cpp
        boost/libs/program_options/src/options_description.cpp
        boost/libs/program_options/src/parsers.cpp
        boost/libs/program_options/src/positional_options.cpp
        boost/libs/program_options/src/split.cpp
        boost/libs/program_options/src/utf8_codecvt_facet.cpp
        boost/libs/program_options/src/value_semantic.cpp
        boost/libs/program_options/src/variables_map.cpp
        boost/libs/program_options/src/winmain.cpp
        )

add_library(boost-system
        boost/libs/system/src/error_code.cpp
        )

add_library(boost-filesystem
        boost/libs/filesystem/src/codecvt_error_category.cpp
        boost/libs/filesystem/src/operations.cpp
        boost/libs/filesystem/src/path.cpp
        boost/libs/filesystem/src/path_traits.cpp
        boost/libs/filesystem/src/portability.cpp
        boost/libs/filesystem/src/unique_path.cpp
        boost/libs/filesystem/src/utf8_codecvt_facet.cpp
        boost/libs/filesystem/src/windows_file_codecvt.cpp
        )