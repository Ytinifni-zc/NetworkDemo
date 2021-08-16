add_subdirectory(ThriftRPC)

add_library(rpc Server.h Server.cpp)
target_link_libraries(rpc PRIVATE my_thrift)
