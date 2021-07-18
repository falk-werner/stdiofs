cmake_minimum_required(VERSION 3.10)
project(stdiofs)

option(WITHOUT_TEST, "Build without tests" OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FUSE REQUIRED IMPORTED_TARGET fuse3)

set(CMAKE_C_STANDARD 99)

add_library(stdiofs STATIC
    src/stdiofs/proxyfs.c
    src/stdiofs/fs/proxy.c
    src/stdiofs/fs/stub.c
    src/stdiofs/fs/stub_operations.c
    src/stdiofs/passthroughfs.c
    src/stdiofs/rpc/rpc.c
    src/stdiofs/rpc/stub.c
    src/stdiofs/rpc/buffer.c
    src/stdiofs/rpc/serializer.c
    src/stdiofs/rpc/deserializer.c
    src/stdiofs/rpc/connection.c
    src/stdiofs/rpc/types.c
    src/stdiofs/rpc/type/int.c
    src/stdiofs/rpc/type/string.c
    src/stdiofs/rpc/type/size.c
    src/stdiofs/rpc/type/u64.c
    src/stdiofs/rpc/type/stat.c
)

target_include_directories(stdiofs PRIVATE src)
target_link_libraries(stdiofs PUBLIC PkgConfig::FUSE)

add_executable(stdiofs_main
    src/main.c)

target_include_directories(stdiofs_main PRIVATE src)
target_link_libraries(stdiofs_main PRIVATE stdiofs)

if(NOT(WITHOUT_TEST))

pkg_check_modules(GTEST REQUIRED IMPORTED_TARGET gtest_main)


add_executable(alltests
    test/stdiofs/rpc/buffer.cpp
    test/stdiofs/rpc/connection.cpp
    test/stdiofs/rpc/serializer.cpp
    test/stdiofs/rpc/deserializer.cpp
    test/stdiofs/rpc/type/unknown.cpp
    test/stdiofs/rpc/type/int.cpp
)

target_include_directories(alltests PRIVATE src test)
target_link_libraries(alltests PRIVATE stdiofs PkgConfig::GTEST)

endif()