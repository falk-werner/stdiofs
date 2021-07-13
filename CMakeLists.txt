cmake_minimum_required(VERSION 3.10)
project(stdiofs)

option(WITHOUT_TEST, "Build without tests" OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FUSE REQUIRED IMPORTED_TARGET fuse3)

set(CMAKE_C_STANDARD 99)

add_library(stdiofs STATIC
    src/proxyfs.c
    src/fs/proxy.c
    src/fs/stub.c
    src/passthroughfs.c
    src/rpc/rpc.c
    src/rpc/buffer.c)

target_include_directories(stdiofs PRIVATE src)
target_link_libraries(stdiofs PUBLIC PkgConfig::FUSE)

add_executable(stdiofs_main
    src/main.c)

target_include_directories(stdiofs_main PRIVATE src)
target_link_libraries(stdiofs_main PRIVATE stdiofs)

if(NOT(WITHOUT_TEST))

pkg_check_modules(GTEST REQUIRED IMPORTED_TARGET gtest_main)


add_executable(alltests
    test/rpc/buffer.cpp)

target_include_directories(alltests PRIVATE src test)
target_link_libraries(alltests PRIVATE stdiofs PkgConfig::GTEST)

endif()