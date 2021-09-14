//
// Created by 赵程 on 2021/9/14.
//

#pragma once

#include <Example-ThriftRPC/TestRPC.h>
#include <Example-bRPC/TestRPC.h>

enum Proto {
    Thrift,
    bRPC
};

inline Proto getProto(const String& input)  {
    if (input == "thrift") return Proto::Thrift;
    if (input == "brpc") return Proto::bRPC;
    return Proto::Thrift;
}
