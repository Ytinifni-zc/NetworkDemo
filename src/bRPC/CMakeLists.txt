include_directories(${BRPC_INCLUDE_PATH})
add_library(my_brpc gen-cpp/echo.pb.h gen-cpp/echo.pb.cc)
target_include_directories(my_brpc PRIVATE brpc)
target_link_libraries(my_thrift PRIVATE ${BRPC_LIB_PATH})