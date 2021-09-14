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
    Args::Arg attachment_('a', false, false);
    cmd.addArg(attachment_);


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
            examplebrpc::startBRPCServer(port, attachment);
        }
            break;
    }
    return 0;
}