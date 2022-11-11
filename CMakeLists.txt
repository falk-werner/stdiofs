cmake_minimum_required(VERSION 3.10)
project(stdiofs VERSION 1.0.0)

option(WITHOUT_TEST     "Build without tests"   OFF)
option(WITHOUT_PROVIDER "Omit stdiofs provider" OFF)
option(WITHOUT_STDIOFS  "Omit stdiofs binary"   OFF)

find_package(PkgConfig REQUIRED)

set(CMAKE_C_STANDARD 99)


add_library(stdiofs STATIC
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
    src/stdiofs/rpc/type/statvfs.c
    src/stdiofs/rpc/type/stringlist.c
    src/stdiofs/rpc/type/offset.c
    src/stdiofs/rpc/type/mode.c
    src/stdiofs/rpc/type/dev.c
    src/stdiofs/rpc/type/uid.c
    src/stdiofs/rpc/type/gid.c
    src/stdiofs/rpc/type/bytes.c
    src/stdiofs/util/connection_monitor.c
)

target_include_directories(stdiofs PRIVATE src)


if(NOT(WITHOUT_STDIOFS))
    pkg_check_modules(FUSE REQUIRED IMPORTED_TARGET fuse3)

    add_executable(stdiofs_main
        src/stdiofs/app/main.c
            src/stdiofs/proxyfs.c)

    set_target_properties(stdiofs_main PROPERTIES OUTPUT_NAME stdiofs)
    target_include_directories(stdiofs_main PRIVATE src)
    target_link_libraries(stdiofs_main PRIVATE stdiofs PkgConfig::FUSE)
    install(TARGETS stdiofs_main RUNTIME)
endif()


if(NOT(WITHOUT_PROVIDER))
    add_executable(stdiofs_provider
        src/stdiofs/app/provider.c)

    target_include_directories(stdiofs_provider PRIVATE src)
    target_link_libraries(stdiofs_provider PRIVATE stdiofs)
    install(TARGETS stdiofs_provider RUNTIME DESTINATION bin)
endif()


if(NOT(WITHOUT_TEST))
    pkg_check_modules(GTEST REQUIRED IMPORTED_TARGET gtest_main)

    add_executable(alltests
        test/stdiofs/rpc/buffer.cpp
        test/stdiofs/rpc/connection.cpp
        test/stdiofs/rpc/serializer.cpp
        test/stdiofs/rpc/deserializer.cpp
        test/stdiofs/rpc/type/unknown.cpp
        test/stdiofs/rpc/type/int.cpp
        test/stdiofs/rpc/type/stringlist.cpp
    )

    target_include_directories(alltests PRIVATE src test)
    target_link_libraries(alltests PRIVATE stdiofs PkgConfig::GTEST)
endif()
