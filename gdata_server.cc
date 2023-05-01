#include <iostream>
#include <memory>
#include <string>
#include <map>

#include <grpcpp/grpcpp.h>
#include "receiver.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using gData::GetData;
using gData::PropRequest;
using gData::PropReply;

// Logic and data behind the server's behavior.
class GetDataServiceImpl final : public GetData::Service {

    Status GetProperty(ServerContext *context, const PropRequest *request,
                       PropReply *reply) override {
        std::map<std::string,int> m;
        m["one"] = 1;
        m["two"] = 2;
        m["three"] = 3;
        auto res = m.find(request->propname());
        if(res != m.end()) {
            reply->set_val(res->second);
        }
        else reply->set_val(-1);
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("127.0.0.1:50051");
    GetDataServiceImpl service;

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc, char **argv) {
    RunServer();

    return 0;
}
