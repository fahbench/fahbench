include(ExternalProject)

set(BOOST_COMPILE_OPTIONS "")

IF(${WIN32})
    set(SHELL_SCRIPT_EXT "bat")
    IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(BOOST_COMPILE_OPTIONS ${BOOST_COMPILE_OPTIONS} "address-model=64")
    ENDIF()
    set(BOOST_COMPILE_OPTIONS ${BOOST_COMPILE_OPTIONS} "variant=release")
ELSE()
    set(SHELL_SCRIPT_EXT "sh")
    set(BOOST_COMPILE_OPTIONS ${BOOST_COMPILE_OPTIONS} "cxxflags=-fPIC")
ENDIF()

ExternalProject_Add(boost
    URL http://downloads.sourceforge.net/project/boost/boost/1.58.0/boost_1_58_0.tar.gz
    URL_MD5 5a5d5614d9a07672e1ab2a250b5defc5
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ./bootstrap.${SHELL_SCRIPT_EXT}
    BUILD_COMMAND ""
    INSTALL_COMMAND
        ./b2 install
        --layout=tagged
        --with-program_options
        --with-filesystem
        --with-system
        --prefix=<INSTALL_DIR>
        ${BOOST_COMPILE_OPTIONS}
        link=static
        threading=multi
)

ExternalProject_Get_Property(boost install_dir)
set(BOOST_INCLUDE_DIR "${install_dir}/include/")
# Boost.build lovingly preprends these libraries with "lib" even on windows
set(BOOST_LIBPREFIX "${install_dir}/lib/libboost_")
set(BOOST_LIBSUFFIX "-mt${CMAKE_STATIC_LIBRARY_SUFFIX}")

set(BOOST_LIBRARIES
    "${BOOST_LIBPREFIX}program_options${BOOST_LIBSUFFIX}"
    "${BOOST_LIBPREFIX}filesystem${BOOST_LIBSUFFIX}"
    "${BOOST_LIBPREFIX}system${BOOST_LIBSUFFIX}"
)

# Disable "autolink" of boost
add_definitions("-DBOOST_ALL_NO_LIB")