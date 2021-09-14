add_subdirectory(Example-ThriftRPC)
add_subdirectory(Example-bRPC)

add_executable(Server Server.cpp Common.h)
target_link_libraries(Server PUBLIC my_thrift my_brpc protobuf)
add_executable(Client Client.cpp Common.h)
target_link_libraries(Client PUBLIC my_thrift my_brpc protobuf)
