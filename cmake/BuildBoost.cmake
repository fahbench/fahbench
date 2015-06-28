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
    INSTALL_COMMAND ./b2 install
)