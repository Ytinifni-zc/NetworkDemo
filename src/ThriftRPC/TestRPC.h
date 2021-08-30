//
// Created by 赵程 on 2021/8/10.
//

#pragma once

#include <chrono>
#include <ctime>

#include "gen-cpp/TestProto.h"
#include "IRPC.h"

namespace ThriftRPC {
    class TestProtoHandler : public TestProtoIf {
    public:
        int64_t remoteTime() {
//            std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
//            return now;
            clockid_t cid{CLOCK_MONOTONIC};
            return StopWatchDetail::nanoseconds(cid);
        }

        void send(std::string &_return) {
            const UInt64 size = (2ul << 30)-1;
            _return.resize(size);
            clockid_t cid{CLOCK_MONOTONIC};
            UInt64 now_t = StopWatchDetail::nanoseconds(cid);
            auto *addr = (UInt64 *) _return.data();
            addr[0] = now_t;
        }

    };

    class TestProtoRPCServer : public IRPCServer {
    public:
        TestProtoRPCServer(UInt32 port) {
            server = std::make_shared<TThreadedServer>(
                    std::make_shared<TestProtoProcessor>(std::make_shared<TestProtoHandler>()),
                    std::make_shared<TServerSocket>(port),
                    std::make_shared<TBufferedTransportFactory>(),
                    std::make_shared<TBinaryProtocolFactory>());
            LOG::info("Build RPCServer (localhost: {})", port);
        }
    };

    class TestProtoRPCClient : public IRPCClient {
    public:
        TestProtoRPCClient(const String &host, UInt32 port) : IRPCClient(host, port) {
            client = std::make_shared<TestProtoClient>(protocol);
        }

        int64_t remoteTime() {
            try {
                transportOpen();
                auto ret = client->remoteTime();
                transportClose();
                return ret;
            }
            catch (TException &tx) {
                throw "[ThriftRPC] remoteTime() Error: " + String(tx.what());
            }
        }

        void send(String &ret) {
            try {

                transportOpen();
                client->send(ret);
                transportClose();
            }
            catch (TException &tx) {
                throw "[ThriftRPC] send(String&) Error: " + String(tx.what());
            }
        }

        void leave() { client.reset(); }

    private:
        std::shared_ptr<TestProtoClient> client;
    };
}
