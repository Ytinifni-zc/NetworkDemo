//
// Created by 赵程 on 2021/8/10.
//

#pragma once

#include <thrift/TToString.h>
#include <thrift/concurrency/ThreadFactory.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include <iostream>
#include <memory>

#include <RPCConfig.h>
#include <utils/Log.h>
#include <utils/Type.h>
#include <utils/StopWatch.h>

namespace ThriftRPC {
    using namespace apache::thrift;
    using namespace apache::thrift::concurrency;
    using namespace apache::thrift::protocol;
    using namespace apache::thrift::transport;
    using namespace apache::thrift::server;

    class IRPCServer {
    public:
        IRPCServer() = default;

        void serve() {
            try {
                std::cerr << "===== Starting the RPC server =====\n";
                server->serve();
                std::cerr << "===== Leaving the RPC server    =====\n";
            }
            catch (TException &tx) {
                throw "Could not establish RPC server: " + String(tx.what());
            }
        }

        void stop() {
            server->stop();
            if (thread) {
                thread->join();
                leave();
            }
            LOG::info("RPCServer leaves.");
        }

        void leave() { server.reset(); }

        void asyncRun() {
            apache::thrift::concurrency::ThreadFactory factory;
            factory.setDetached(false);
            std::shared_ptr<apache::thrift::concurrency::Runnable> serverThreadRunner(server);
            thread = factory.newThread(serverThreadRunner);
            thread->start();
        }

        virtual ~IRPCServer() { stop(); }

    protected:
        std::shared_ptr<TThreadedServer> server;

    private:
        std::shared_ptr<apache::thrift::concurrency::Thread> thread;
    };

    class IRPCClient {
    public:
        IRPCClient() = default;

        IRPCClient(const String &host, UInt32 port) {
            socket = std::shared_ptr<TTransport>(new TSocket(host, port));
            transport = std::shared_ptr<TTransport>(new TBufferedTransport(socket));
            auto retry_limit = 1000;
            auto retry_times = 0;
            bool is_open{false};
            while (!is_open) {
                try {
                    transport->open();
                    is_open = true;
                }
                catch (TException &tx) {
                    is_open = false;
                    retry_times++;
                    if (retry_limit == retry_times)
                        throw "Connection to (" + host + ":" + std::to_string(port) + ") refused. Retry " +
                              std::to_string(retry_limit)
                              + " times. " + String(tx.what());
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(RETRY_CONNECT_TIME_OUT));
            }
            protocol = std::shared_ptr<TProtocol>(new TBinaryProtocol(transport));
        }

        IRPCClient &operator=(IRPCClient const &) = default;

        IRPCClient &operator=(IRPCClient &&) = default;

        void transportOpen() { return transport->open(); }

        void transportClose() { return transport->close(); }

        virtual void leave() = 0; // { client.reset(); }

        virtual ~IRPCClient() = default;

    protected:
        std::shared_ptr<TProtocol> protocol;

    private:
        std::shared_ptr<TTransport> socket;
        std::shared_ptr<TTransport> transport;
    };
}
