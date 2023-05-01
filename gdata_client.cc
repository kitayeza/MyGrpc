
#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "receiver.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using gData::GetData;
using gData::PropRequest;
using gData::PropReply;

class GetDataClient {
 public:
    GetDataClient(std::shared_ptr<Channel> channel)
      : stub_(GetData::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  int GetProperty(std::string& propName) {
      PropRequest request;
      request.set_propname(propName);
      PropReply reply;
      ClientContext context;
      Status status = stub_->GetProperty(&context, request, &reply);
      if (status.ok()) {
          return reply.val();
      } else {
          std::cout << status.error_code() << ": " << status.error_message()
                    << std::endl;
          return -1;
      }
  }

 private:
  std::unique_ptr<GetData::Stub> stub_;
};

int main(int argc, char** argv) {
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint (in this case,
  // localhost at port 50051). We indicate that the channel isn't authenticated
  // (use of InsecureChannelCredentials()).
    GetDataClient gdata(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials()));
  std::string user("two");
  int reply = gdata.GetProperty(user);
  std::cout << "GetData received: " << reply << std::endl;

  return 0;
}
