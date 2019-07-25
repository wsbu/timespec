# (C)2019 Red Lion Controls, Inc. All rights reserved. Red Lion, the Red Lion logo and Sixnet are registered trademarks
# of Red Lion Controls, Inc. All other company and product names are trademarks of their respective owners.

cmake_minimum_required(VERSION 3.3)

file(STRINGS "version.txt" VERSION)
project(timespec LANGUAGES C VERSION ${VERSION})

option(CONAN "Enable Conan dependency manager" ON)
if (CONAN AND EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    conan_basic_setup(TARGETS)
    string(REPLACE ";" ":" LINK_FLAGS "${CONAN_LIB_DIRS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-rpath-link,${LINK_FLAGS}")
endif ()

enable_testing()

find_package(Doxygen QUIET)
find_package(WsbuDocGen QUIET)

option(BUILD_SHARED_LIBS "Build dynamic libraries when on, else static" ON)

add_compile_options(-Wall -Wpedantic -Wconversion)
set(CMAKE_C_STANDARD 99)
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")

add_subdirectory(src)
add_subdirectory(test)

install(DIRECTORY "include/" DESTINATION include)
install(EXPORT ${PROJECT_NAME}Config
    DESTINATION lib/cmake/${PROJECT_NAME}
)

include(timespecCPack.cmake)
