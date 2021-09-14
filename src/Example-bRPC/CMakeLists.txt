FILE(GLOB protofiles "${CMAKE_CURRENT_SOURCE_DIR}/*.proto")
include(FindProtobuf)
protobuf_generate_cpp(PROTOSRCS PROTOHDRS ${protofiles})
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS ${PROTOSRCS})
message(STATUS ${PROTOHDRS})
message(STATUS ${CMAKE_CURRENT_BINARY_DIR})

include_directories("${BRPC_PATH}/include")
include_directories("${CMAKE_SOURCE_DIR}/contrib/incubator-brpc/src")

add_library(my_brpc TestRPC.h ${PROTOSRCS} ${PROTOHDRS})
target_include_directories(my_brpc PUBLIC ${BRPC_INCLUDE_PATH})
target_include_directories(my_brpc PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_directories(my_brpc PUBLIC ${BRPC_LIB_PATH})
target_link_libraries(my_brpc PUBLIC brpc spdlog)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/key.pem
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/cert.pem
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/key.pem
        DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/cert.pem
        DESTINATION ${CMAKE_BINARY_DIR})
