//
// Created by 赵程 on 2021/8/10.
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
    Args::Arg port_('p', "port", true, false);
    cmd.addArg(port_);
    Args::Arg proto_('t', "proto", true, false);
    cmd.addArg(proto_);
    Args::Arg attachment_('a', "attachment", false, false);
    cmd.addArg(attachment_);
    Args::Arg is_response_('b', "is_response", false, false);
    cmd.addArg(is_response_);
    Args::Arg idle_timeout_s_("timeout", true, false);
    cmd.addArg(idle_timeout_s_);
    Args::Arg max_concurrency_("max_c", true, false);
    cmd.addArg(max_concurrency_);
    Args::Arg internal_port_("internal_port", true, false);
    cmd.addArg(internal_port_);


    cmd.parse();

    char log_level = 'i';
    if (log_level_.isDefined())
        log_level = log_level_.value().front();
    assert(log_level == 'i' || log_level == 't' || log_level == 'd');
    if (log_level == 'd')
        LOG::set_level(LOG::level::debug);
    if (log_level == 't')
        LOG::set_level(LOG::level::trace);

    auto port = port_.isDefined()? std::stoi(port_.value()) : 38888;
    auto proto = proto_.isDefined()? getProto(proto_.value()) : Proto::Thrift;
    auto attachment = attachment_.isDefined();
    auto is_response = is_response_.isDefined();
    auto idle_timeout_s = idle_timeout_s_.isDefined()? std::stoi(idle_timeout_s_.value()) : -1;
    auto max_concurrency = max_concurrency_.isDefined()? std::stoi(max_concurrency_.value()) : -1;
    auto internal_port = internal_port_.isDefined()? std::stoi(internal_port_.value()) : -1;

    LOG::info("Proto: {} port: {}", proto, port);

    switch (proto) {
        case Proto::Thrift:
        {
            TestProtoRPCServer server(port);
            server.asyncRun();
        }
            break;
        case Proto::bRPC:
        {
            examplebrpc::startBRPCServer(port, attachment, is_response, max_concurrency, internal_port);
        }
            break;
    }
    return 0;
}