set(SPDLOG_BUILD_SHARED 1)

add_subdirectory(fmt)
add_subdirectory(jemalloc-cmake)
add_subdirectory(spdlog)
add_subdirectory(numactl-cmake)

# add thrift
set(BUILD_TESTING OFF)
set(BUILD_COMPILER ON)
set(BUILD_CPP ON)
set(BUILD_TUTORIALS OFF)
set(BUILD_C_GLIB OFF)
set(BUILD_JAVA OFF)
set(BUILD_PYTHON OFF)
set(BUILD_HASKELL OFF)
set(WITH_PLUGIN ON)
set(WITH_QT5 OFF)
#set(WITH_ZLIB OFF)
set(WITH_OPENSSL OFF)
set(BUILD_EXAMPLES ON)
add_subdirectory(thrift)

