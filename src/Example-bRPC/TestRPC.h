//
// Created by 赵程 on 2021/9/14.
//

#pragma once

#include <brpc/server.h>
#include <brpc/channel.h>
#include <utils/Log.h>
#include "echo.pb.h"

namespace examplebrpc {
    using namespace src;

    inline String iobuf2str(const butil::IOBuf &buf) {
        std::stringstream ss;
        const size_t n = buf.backing_block_num();
        for (size_t i = 0; i < n; ++i) {
            butil::StringPiece blk = buf.backing_block(i);
            ss.write(blk.data(), blk.size());
        }
        return ss.str();
    };

    class EchoServiceImpl : public EchoService {
    public:
        explicit EchoServiceImpl(bool attachment_) : attachment(attachment_) {}

        virtual ~EchoServiceImpl() {};

        virtual void Echo(google::protobuf::RpcController *cntl_base,
                          const EchoRequest *request,
                          EchoResponse *response,
                          google::protobuf::Closure *done) {
            // This object helps you to call done->Run() in RAII style. If you need
            // to process the request asynchronously, pass done_guard.release().
            brpc::ClosureGuard done_guard(done);

            brpc::Controller *cntl =
                    static_cast<brpc::Controller *>(cntl_base);

            // The purpose of following logs is to help you to understand
            // how clients interact with servers more intuitively. You should
            // remove these logs in performance-sensitive servers.

            LOG::info("Received request[log_id={}] from {} to {}: {} (attached={})",
                      cntl->log_id(),
                      butil::endpoint2str(cntl->remote_side()).c_str(),
                      butil::endpoint2str(cntl->local_side()).c_str(),
                      request->message(), iobuf2str(cntl->request_attachment()));

            // Fill response.
            response->set_message(request->message());

            // You can compress the response by setting Controller, but be aware
            // that compression may be costly, evaluate before turning on.
            // cntl->set_response_compress_type(brpc::COMPRESS_TYPE_GZIP);

            if (attachment) {
                // Set attachment which is wired to network directly instead of
                // being serialized into protobuf messages.
                cntl->response_attachment().append(cntl->request_attachment());
            }
        }

    private:
        bool attachment;

    };

    void startBRPCServer(int port, bool attachment) {
        brpc::Server server;
        examplebrpc::EchoServiceImpl echo_service_impl(attachment);
        if (server.AddService(&echo_service_impl, brpc::SERVER_DOESNT_OWN_SERVICE) != 0) {
            LOG::error("Fail to add service");
            exit(1);
        }
        brpc::ServerOptions options;
        options.idle_timeout_sec = -1;
        if (server.Start(port, &options) != 0) {
            LOG::error("Fail to add service");
            exit(1);
        }
        server.RunUntilAskedToQuit();
    }

    void bRPCClient(const String &server = "0.0.0.0:8000", const String &attachment = "",
                    const String &protocol = "baidu_std", const String &connection_type = "",
                    const String &load_balancer = "", int timeout_ms = 100,
                    int max_retry = 3, int interval_ms = 1000) {
        brpc::Channel channel;
        brpc::ChannelOptions options;
        options.protocol = protocol;
        options.connection_type = connection_type;
        options.timeout_ms = timeout_ms;
        options.max_retry = max_retry;
        LOG::debug("server: {}", server);
        if (channel.Init(server.c_str(), load_balancer.c_str(), &options) != 0) {
            LOG::error("Fail to initialize channel");
            exit(1);
        }

        examplebrpc::EchoService_Stub stub(&channel);

        int log_id = 0;
        while (!brpc::IsAskedToQuit()) {
            examplebrpc::EchoRequest request;
            examplebrpc::EchoResponse response;
            brpc::Controller cntl;

            request.set_message("Hello World!");

            cntl.set_log_id(log_id++);

            cntl.request_attachment().append(attachment);
            stub.Echo(&cntl, &request, &response, NULL);
            if (!cntl.Failed()) {
                LOG::info("Received response from {} to {}: {} (attached={}) latency={}us",
                          butil::endpoint2str(cntl.remote_side()).c_str(),
                          butil::endpoint2str(cntl.local_side()).c_str(), response.message(),
                          iobuf2str(cntl.response_attachment()), cntl.latency_us());
            } else {
                LOG::warn("[bRPC ErrorCode: {}]{}", cntl.ErrorCode(), cntl.ErrorText());
            }
            usleep(interval_ms * 1000L);

        }

        LOG::info("EchoClient is goint to quit");
    }
}