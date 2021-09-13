add_subdirectory(ThriftRPC)
add_subdirectory(bRPC)

add_executable(Server Server.h Server.cpp)
target_link_libraries(Server PRIVATE my_thrift)
add_executable(Client Client.h Client.cpp)
target_link_libraries(Client PRIVATE my_thrift)
