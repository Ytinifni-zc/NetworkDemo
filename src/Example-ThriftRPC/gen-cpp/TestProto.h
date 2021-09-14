/**
 * Autogenerated by Thrift Compiler (0.14.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef TestProto_H
#define TestProto_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include <memory>
#include "proto_types.h"



#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class TestProtoIf {
 public:
  virtual ~TestProtoIf() {}
  virtual int64_t remoteTime() = 0;
  virtual void send(std::string& _return) = 0;
};

class TestProtoIfFactory {
 public:
  typedef TestProtoIf Handler;

  virtual ~TestProtoIfFactory() {}

  virtual TestProtoIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(TestProtoIf* /* handler */) = 0;
};

class TestProtoIfSingletonFactory : virtual public TestProtoIfFactory {
 public:
  TestProtoIfSingletonFactory(const ::std::shared_ptr<TestProtoIf>& iface) : iface_(iface) {}
  virtual ~TestProtoIfSingletonFactory() {}

  virtual TestProtoIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(TestProtoIf* /* handler */) {}

 protected:
  ::std::shared_ptr<TestProtoIf> iface_;
};

class TestProtoNull : virtual public TestProtoIf {
 public:
  virtual ~TestProtoNull() {}
  int64_t remoteTime() {
    int64_t _return = 0;
    return _return;
  }
  void send(std::string& /* _return */) {
    return;
  }
};


class TestProto_remoteTime_args {
 public:

  TestProto_remoteTime_args(const TestProto_remoteTime_args&);
  TestProto_remoteTime_args& operator=(const TestProto_remoteTime_args&);
  TestProto_remoteTime_args() {
  }

  virtual ~TestProto_remoteTime_args() noexcept;

  bool operator == (const TestProto_remoteTime_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const TestProto_remoteTime_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TestProto_remoteTime_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class TestProto_remoteTime_pargs {
 public:


  virtual ~TestProto_remoteTime_pargs() noexcept;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _TestProto_remoteTime_result__isset {
  _TestProto_remoteTime_result__isset() : success(false) {}
  bool success :1;
} _TestProto_remoteTime_result__isset;

class TestProto_remoteTime_result {
 public:

  TestProto_remoteTime_result(const TestProto_remoteTime_result&);
  TestProto_remoteTime_result& operator=(const TestProto_remoteTime_result&);
  TestProto_remoteTime_result() : success(0) {
  }

  virtual ~TestProto_remoteTime_result() noexcept;
  int64_t success;

  _TestProto_remoteTime_result__isset __isset;

  void __set_success(const int64_t val);

  bool operator == (const TestProto_remoteTime_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const TestProto_remoteTime_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TestProto_remoteTime_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _TestProto_remoteTime_presult__isset {
  _TestProto_remoteTime_presult__isset() : success(false) {}
  bool success :1;
} _TestProto_remoteTime_presult__isset;

class TestProto_remoteTime_presult {
 public:


  virtual ~TestProto_remoteTime_presult() noexcept;
  int64_t* success;

  _TestProto_remoteTime_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};


class TestProto_send_args {
 public:

  TestProto_send_args(const TestProto_send_args&);
  TestProto_send_args& operator=(const TestProto_send_args&);
  TestProto_send_args() {
  }

  virtual ~TestProto_send_args() noexcept;

  bool operator == (const TestProto_send_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const TestProto_send_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TestProto_send_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class TestProto_send_pargs {
 public:


  virtual ~TestProto_send_pargs() noexcept;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _TestProto_send_result__isset {
  _TestProto_send_result__isset() : success(false) {}
  bool success :1;
} _TestProto_send_result__isset;

class TestProto_send_result {
 public:

  TestProto_send_result(const TestProto_send_result&);
  TestProto_send_result& operator=(const TestProto_send_result&);
  TestProto_send_result() : success() {
  }

  virtual ~TestProto_send_result() noexcept;
  std::string success;

  _TestProto_send_result__isset __isset;

  void __set_success(const std::string& val);

  bool operator == (const TestProto_send_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const TestProto_send_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TestProto_send_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _TestProto_send_presult__isset {
  _TestProto_send_presult__isset() : success(false) {}
  bool success :1;
} _TestProto_send_presult__isset;

class TestProto_send_presult {
 public:


  virtual ~TestProto_send_presult() noexcept;
  std::string* success;

  _TestProto_send_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class TestProtoClient : virtual public TestProtoIf {
 public:
  TestProtoClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  TestProtoClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  int64_t remoteTime();
  void send_remoteTime();
  int64_t recv_remoteTime();
  void send(std::string& _return);
  void send_send();
  void recv_send(std::string& _return);
 protected:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class TestProtoProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::std::shared_ptr<TestProtoIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (TestProtoProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_remoteTime(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_send(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  TestProtoProcessor(::std::shared_ptr<TestProtoIf> iface) :
    iface_(iface) {
    processMap_["remoteTime"] = &TestProtoProcessor::process_remoteTime;
    processMap_["send"] = &TestProtoProcessor::process_send;
  }

  virtual ~TestProtoProcessor() {}
};

class TestProtoProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  TestProtoProcessorFactory(const ::std::shared_ptr< TestProtoIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::std::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::std::shared_ptr< TestProtoIfFactory > handlerFactory_;
};

class TestProtoMultiface : virtual public TestProtoIf {
 public:
  TestProtoMultiface(std::vector<std::shared_ptr<TestProtoIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~TestProtoMultiface() {}
 protected:
  std::vector<std::shared_ptr<TestProtoIf> > ifaces_;
  TestProtoMultiface() {}
  void add(::std::shared_ptr<TestProtoIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  int64_t remoteTime() {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->remoteTime();
    }
    return ifaces_[i]->remoteTime();
  }

  void send(std::string& _return) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->send(_return);
    }
    ifaces_[i]->send(_return);
    return;
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class TestProtoConcurrentClient : virtual public TestProtoIf {
 public:
  TestProtoConcurrentClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot, std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync) : sync_(sync)
{
    setProtocol(prot);
  }
  TestProtoConcurrentClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot, std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync) : sync_(sync)
{
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  int64_t remoteTime();
  int32_t send_remoteTime();
  int64_t recv_remoteTime(const int32_t seqid);
  void send(std::string& _return);
  int32_t send_send();
  void recv_send(std::string& _return, const int32_t seqid);
 protected:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync_;
};

#ifdef _MSC_VER
  #pragma warning( pop )
#endif



#endif