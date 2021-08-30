cmake_minimum_required(VERSION 3.2)
project(NetworkDemo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g -Wall -ldl")

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/contrib)
include_directories(${CMAKE_SOURCE_DIR}/contrib/thrift/lib/cpp/src)
include_directories(${CMAKE_SOURCE_DIR}/build/contrib/thrift)
include_directories(${CMAKE_SOURCE_DIR}/contrib/spdlog/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

include(${CMAKE_SOURCE_DIR}/cmake/find_jemalloc.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/arch.cmake)

if (CMAKE_GENERATOR STREQUAL "Ninja")
    # Turn on colored output. https://github.com/ninja-build/ninja/wiki/FAQ
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdiagnostics-color=always")
endif ()

add_subdirectory(contrib)
if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()

set(CUSTOM_THRIFT_LIB thrift)
list(APPEND CUSTOM_THRIFT_INCLUDE_DIR
        "${CMAKE_SOURCE_DIR}/src/ThriftRPC/gen-cpp "
        "${CMAKE_SOURCE_DIR}/contrib/thrift/lib/cpp/src "
        "${CMAKE_SOURCE_DIR}/build/contrib/thrift/lib/cpp "
        "${CMAKE_SOURCE_DIR}/cmake-build-relwithdebinfo/contrib/thrift/lib/cpp ")

add_subdirectory(utils)
add_subdirectory(src)
#add_subdirectory(tests)

