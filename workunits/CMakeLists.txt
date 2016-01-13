include(ExternalProject)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/workunits")

ExternalProject_Add(wu-dhfr
    URL "${CMAKE_CURRENT_SOURCE_DIR}/dhfr.tar.bz2"
    URL_MD5 d79283fd8f0ab780e1990c624ba8bce3
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND "${CMAKE_COMMAND}" -E copy_directory <SOURCE_DIR> "${CMAKE_CURRENT_BINARY_DIR}/workunits/dhfr"
)

ExternalProject_Add(wu-dhfr-implicit
    URL "${CMAKE_CURRENT_SOURCE_DIR}/dhfr-implicit.tar.bz2"
    URL_MD5 e7e8d395523685afdc6e0d3070ae050a
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND "${CMAKE_COMMAND}" -E copy_directory <SOURCE_DIR> "${CMAKE_CURRENT_BINARY_DIR}/workunits/dhfr-implicit"
)

ExternalProject_Add(wu-nav
    URL "${CMAKE_CURRENT_SOURCE_DIR}/nav.tar.bz2"
    URL_MD5 15551db7d643904a0168ec49be5f7bea
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND "${CMAKE_COMMAND}" -E copy_directory <SOURCE_DIR> "${CMAKE_CURRENT_BINARY_DIR}/workunits/nav"
)

add_custom_target(workunits DEPENDS
    wu-dhfr
    wu-dhfr-implicit
    wu-nav
)

install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/workunits"
    DESTINATION share/fahbench)
