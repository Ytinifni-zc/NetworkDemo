//
// Created by 赵程 on 2021/8/30.
//

#include <args-parser/args-parser/all.hpp>

#include "Common.h"

int main(int argc, char *argv[]) {
    using namespace ThriftRPC;

//    std::locale::global(std::locale("en_US.UTF-8"));
    LOG::set_pattern("[%^%l%$] %v");

    Args::CmdLine cmd(argc, argv);

    Args::Arg log_level_('v', "log_level", true, false);
    cmd.addArg(log_level_);
    Args::Arg host_('h', "host", true, false);
    cmd.addArg(host_);
    Args::Arg port_('p', "port", true, false);
    cmd.addArg(port_);
    Args::Arg proto_('t', "proto", true, false);
    cmd.addArg(proto_);
    Args::Arg attachment_('a', "attachment", true, false);
    cmd.addArg(attachment_);
    Args::Arg request_size_('r', "request_size", true, false);
    cmd.addArg(request_size_);
    Args::Arg use_bthread_("use_bthread", false);
    cmd.addArg(use_bthread_);
    Args::Arg thread_num_("thread_num", true, false);
    cmd.addArg(thread_num_);

    cmd.parse();

    char log_level = 'i';
    if (log_level_.isDefined())
        log_level = log_level_.value().front();
    assert(log_level == 'i' || log_level == 't' || log_level == 'd');
    if (log_level == 'd')
        LOG::set_level(LOG::level::debug);
    if (log_level == 't')
        LOG::set_level(LOG::level::trace);

    auto proto = proto_.isDefined() ? getProto(proto_.value()) : Proto::Thrift;

    String host = host_.isDefined() ? host_.value() : "127.0.0.1";
    auto port = port_.isDefined() ? std::stoi(port_.value()) : 38888;
    auto attachment = attachment_.isDefined() ? attachment_.value() : "";
    auto request_size = request_size_.isDefined() ? std::stol(request_size_.value()) : 0l;
    auto use_bthread = use_bthread_.isDefined();
    auto thread_num = thread_num_.isDefined() ? std::stoi(thread_num_.value()) : 1;

    LOG::info("Proto: {} port: {}", proto, port);

    switch (proto) {
        case Proto::Thrift: {
            TestProtoRPCClient client(host, port);
            clockid_t cid{CLOCK_MONOTONIC};
            {
                Stopwatch w;
                auto request_time = StopWatchDetail::nanoseconds(cid);
                auto remote_time = client.remoteTime();
                auto response_time = StopWatchDetail::nanoseconds(cid);

                LOG::info("Request time: 0");
                LOG::info("Remote time: {}us", (remote_time - request_time) / 1000);
                LOG::info("Response time: {}us", (response_time - remote_time) / 1000);
                LOG::info("Total elapse: {}us", w.elapsedMicroseconds());

            }

            {
                Stopwatch w;
                auto request_time = StopWatchDetail::nanoseconds(cid);
                String data;
                client.send(data);
                LOG::trace("Receive remote data size: {}", data.size());
                LOG::info("Receive elapse: {}us", w.elapsedMicroseconds());
                auto remote_time = *(UInt64 *) data.data();
                auto response_time = StopWatchDetail::nanoseconds(cid);

                LOG::info("Request time: 0");
                LOG::info("Remote time: {}us", (remote_time - request_time) / 1000);
                LOG::info("Response time: {}us", (response_time - remote_time) / 1000);
                LOG::info("Total elapse: {}ms", w.elapsedMilliseconds());
            }
        }
            break;
        case Proto::bRPC: {
            std::stringstream ss("");
            ss << host << ":" << std::to_string(port);
            String server = ss.str();
            examplebrpc::bRPCClient(server, attachment, "baidu_std", "",
                                    "", 100, 3, 1000, false,
                                    0, request_size, -1, use_bthread, thread_num);
        }
            break;
    }


    return 0;
}