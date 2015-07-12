include(ExternalProject)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/workunits")

ExternalProject_Add(wu-dhfr
    URL "${CMAKE_CURRENT_SOURCE_DIR}/dhfr.tar.bz2"
    URL_MD5 68f66327642a837819d1dc2c526fc4d6
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND "${CMAKE_COMMAND}" -E copy_directory <SOURCE_DIR> "${CMAKE_CURRENT_BINARY_DIR}/workunits/dhfr"
)

ExternalProject_Add(wu-dhfr-implicit
    URL "${CMAKE_CURRENT_SOURCE_DIR}/dhfr-implicit.tar.bz2"
    URL_MD5 db348d99f528ae35f732b409ae9402e5
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND "${CMAKE_COMMAND}" -E copy_directory <SOURCE_DIR> "${CMAKE_CURRENT_BINARY_DIR}/workunits/dhfr-implicit"
)

ExternalProject_Add(wu-nav
    URL "${CMAKE_CURRENT_SOURCE_DIR}/nav.tar.bz2"
    URL_MD5 37fa8d9ae719d0561cc1dc0237ff0b4c
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