include(ExternalProject)

ExternalProject_Add(boost
    URL http://downloads.sourceforge.net/project/boost/boost/1.58.0/boost_1_58_0.tar.gz
    URL_MD5 5a5d5614d9a07672e1ab2a250b5defc5
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND
        ./bootstrap.sh
        --with-libraries=program_options,filesystem,system
        --prefix=<INSTALL_DIR>
    BUILD_COMMAND ""
    INSTALL_COMMAND
        ./b2 install
        cxxflags=-fPIC
        link=static
)

ExternalProject_Get_Property(boost install_dir)
set(BOOST_LIBROOT "${install_dir}/lib")
set(BOOST_INCLUDE_DIR "${install_dir}/include")

set(BOOST_LIBRARIES
    "${BOOST_LIBROOT}/${CMAKE_STATIC_LIBRARY_PREFIX}boost_program_options${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${BOOST_LIBROOT}/${CMAKE_STATIC_LIBRARY_PREFIX}boost_filesystem${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${BOOST_LIBROOT}/${CMAKE_STATIC_LIBRARY_PREFIX}boost_system${CMAKE_STATIC_LIBRARY_SUFFIX}"
)

    