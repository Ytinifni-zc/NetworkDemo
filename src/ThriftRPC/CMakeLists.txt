if (NOT EXISTS ${THRIFT_COMPILER})
    #    set(THRIFT_COMPILER $<TARGET_FILE:thrift-compiler>)
    set(THRIFT_COMPILER )
endif ()

add_custom_command(
        OUTPUT gen-cpp/TestProto.cpp
        COMMAND ${THRIFT_COMPILER} --gen cpp -r frontier.thrift
)


include_directories(${CUSTOM_THRIFT_INCLUDE_DIR})

add_library(my_thrift gen-cpp/proto_types.h gen-cpp/TestProto.h gen-cpp/TestProto.cpp IRPC.h TestRPC.h)
target_include_directories(my_thrift PRIVATE ${CUSTOM_THRIFT_INCLUDE_DIR})
target_link_libraries(my_thrift PRIVATE ${CUSTOM_THRIFT_LIB})
