//
// Created by 赵程 on 2021/8/10.
//

#pragma once

#include "gen-cpp/TestProto.h"
#include "IRPC.h"

namespace thriftRPC {
class TestProtoHandler : public TestProtoIf {
public:
    int64_t remoteTime() {

    }

    void send(std::string& _return) {
        const UInt64 size = 10 << 30;
        _return.resize(size);
    }

private:
    UInt64 time;

};
}
