//
// Created by 赵程 on 2021/9/14.
//

#pragma once

#include <bthread/bthread.h>
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
        EchoServiceImpl(bool attachment_, bool is_r) : attachment(attachment_), is_response(is_r) {}

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

//            LOG::info("Received request[log_id={}] from {} to {}: {} (attached={})",
//                      cntl->log_id(),
//                      butil::endpoint2str(cntl->remote_side()).c_str(),
//                      butil::endpoint2str(cntl->local_side()).c_str(),
//                      request->message(), iobuf2str(cntl->request_attachment()));

            // Fill response.
            if (is_response)
                response->set_message(request->message());
            else
                std::string tmp = request->message();

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
        bool is_response;

    };

    void startBRPCServer(int port, bool attachment, bool is_response, int max_concurrency, int internal_port) {
        brpc::Server server;
        examplebrpc::EchoServiceImpl echo_service_impl(attachment, is_response);
        if (server.AddService(&echo_service_impl, brpc::SERVER_DOESNT_OWN_SERVICE) != 0) {
            LOG::error("Fail to add service");
            exit(1);
        }
        brpc::ServerOptions options;
        options.mutable_ssl_options()->default_cert.certificate = "cert.pem";
        options.mutable_ssl_options()->default_cert.private_key = "key.pem";
        options.idle_timeout_sec = -1;
        options.max_concurrency = max_concurrency;
        options.internal_port = internal_port;
        if (server.Start(port, &options) != 0) {
            LOG::error("Fail to add service");
            exit(1);
        }
        server.RunUntilAskedToQuit();
    }

    bvar::LatencyRecorder g_latency_recorder("client");
    bvar::Adder<int> g_error_count("client_error_count");

    std::string g_request;
    std::string g_attachment;

    bool dont_fail{false};

    void *sender(void *arg) {
        // Normally, you should not call a Channel directly, but instead construct
        // a stub Service wrapping it. stub can be shared by all threads as well.
        EchoService_Stub stub(static_cast<google::protobuf::RpcChannel *>(arg));

        int log_id = 0;
        while (!brpc::IsAskedToQuit()) {
            // We will receive response synchronously, safe to put variables
            // on stack.
            EchoRequest request;
            EchoResponse response;
            brpc::Controller cntl;

            request.set_message(g_request);
            cntl.set_log_id(log_id++);  // set by user
            // Set attachment which is wired to network directly instead of
            // being serialized into protobuf messages.
            cntl.request_attachment().append(g_attachment);

            // Because `done'(last parameter) is NULL, this function waits until
            // the response comes back or error occurs(including timedout).
            stub.Echo(&cntl, &request, &response, NULL);
            if (!cntl.Failed()) {
                g_latency_recorder << cntl.latency_us();
            } else {
                g_error_count << 1;
                    CHECK(brpc::IsAskedToQuit() || !dont_fail)
//                CHECK(brpc::IsAskedToQuit())
                << "error=" << cntl.ErrorText() << " latency=" << cntl.latency_us();
                // We can't connect to the server, sleep a while. Notice that this
                // is a specific sleeping to prevent this thread from spinning too
                // fast. You should continue the business logic in a production
                // server rather than sleeping.
                bthread_usleep(50000);
            }
        }
        return NULL;
    }

    void bRPCClient(const String &server = "0.0.0.0:8000", const String &attachment = "",
                    const String &protocol = "baidu_std", const String &connection_type = "",
                    const String &load_balancer = "", int timeout_ms = 100,
                    int max_retry = 3, int interval_ms = 1000, bool enable_ssl = false, long attachment_size = 0,
                    long request_size = 16, int dummy_port = -1, bool use_bthread = true, int thread_num = 50) {

        brpc::Channel channel;
        brpc::ChannelOptions options;
        if (enable_ssl) {
            options.mutable_ssl_options();
        }
        options.protocol = protocol;
        options.connection_type = connection_type;
        options.connect_timeout_ms = std::min(timeout_ms / 2, 100);
        options.timeout_ms = timeout_ms;
        options.max_retry = max_retry;
        LOG::debug("server: {}", server);
        if (channel.Init(server.c_str(), load_balancer.c_str(), &options) != 0) {
            LOG::error("Fail to initialize channel");
            exit(1);
        }
        if (attachment_size > 0) {
            g_attachment.resize(attachment_size, 'a');
        }
        if (request_size <= 0) {
            LOG::error("Bad request_size={}", request_size);
            exit(1);
        }
        g_request.resize(request_size, 'r');

        if (dummy_port >= 0) {
            brpc::StartDummyServerAt(dummy_port);
        }

        std::vector<bthread_t> bids;
        std::vector<pthread_t> pids;
        if (!use_bthread) {
            pids.resize(thread_num);
            for (int i = 0; i < thread_num; ++i) {
                if (pthread_create(&pids[i], NULL, sender, &channel) != 0) {
                    LOG::error("Fail to create pthread");
                    exit(1);
                }
            }
        } else {
            bids.resize(thread_num);
            for (int i = 0; i < thread_num; ++i) {
                if (bthread_start_background(
                        &bids[i], NULL, sender, &channel) != 0) {
                    LOG::error("Fail to create pthread");
                    exit(1);
                }
            }
        }

        while (!brpc::IsAskedToQuit()) {
            sleep(1);
            LOG::info("Sending EchoRequest at qps={} latency={}", g_latency_recorder.qps(1),
                      g_latency_recorder.latency(1));
        }

        LOG::info("EchoClient is going to quit");
        for (int i = 0; i < thread_num; ++i) {
            if (!use_bthread) {
                pthread_join(pids[i], NULL);
            } else {
                bthread_join(bids[i], NULL);
            }
        }

    }
}