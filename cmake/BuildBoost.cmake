include(ExternalProject)

set(BOOST_TAG boost-1.58.0
    CACHE STRING "Git tag to use when building boost")


ExternalProject_Add(boost
    GIT_REPOSITORY https://github.com/boostorg/boost.git
    GIT_TAG ${BOOST_TAG}
    GIT_SUBMODULES
        tools/build
        tools/inspect
        libs/program_options
        libs/filesystem
        libs/system
        libs/config # ?
        libs/wave   # ?
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND 
        ./bootstrap.sh
        --with-libraries=program_options,filesystem,system
        --prefix=<INSTALL_DIR>
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND
        ./b2 install
        cxxflags=-fPIC
        link=static
)

ExternalProject_Get_Property(boost install_dir)
set(BOOST_LIBROOT "${install_dir}/lib")
set(BOOST_INCLUDE_DIR "{install_dir}/include")
set(BOOST_LIBRARIES
    "${BOOST_LIBROOT}/${CMAKE_STATIC_LIBRARY_PREFIX}boost_program_options${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${BOOST_LIBROOT}/${CMAKE_STATIC_LIBRARY_PREFIX}boost_filesystem${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${BOOST_LIBROOT}/${CMAKE_STATIC_LIBRARY_PREFIX}boost_system${CMAKE_STATIC_LIBRARY_SUFFIX}"
)

    