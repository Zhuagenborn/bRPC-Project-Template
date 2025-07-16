/**
 * @file server.cpp
 * @brief A *bRPC* echo server.
 */

#include "echo.pb.h"

#include <brpc/server.h>

#include <algorithm>
#include <cstdint>

namespace {

inline constexpr std::uint16_t port {8000};

}  // namespace

//! Implementation of @p EchoService defined in @p proto/echo.proto.
class EchoServiceImpl : public echo::EchoService {
public:
    void Echo(google::protobuf::RpcController* const ctrl, const echo::EchoRequest* const request,
              echo::EchoResponse* const response,
              google::protobuf::Closure* const done) noexcept override {
        auto& pairs {*response->mutable_messages()->mutable_pairs()};
        std::ranges::for_each(request->messages().pairs(),
                              [&pairs](const auto& pair) { pairs[pair.first] = pair.second; });
        // Signal that RPC is done.
        done->Run();
    }
};

int main() {
    // Create a RPC server instance and service implementation.
    brpc::Server server;
    EchoServiceImpl service;

    // Register the service with the server.
    if (server.AddService(&service, brpc::SERVER_DOESNT_OWN_SERVICE) != 0) {
        LOG(FATAL) << "Failed to register RPC service";
        return EXIT_FAILURE;
    }

    // Start the server on the specified port.
    if (server.Start(port, nullptr) != 0) {
        LOG(FATAL) << "Failed to start RPC service";
        return EXIT_FAILURE;
    }

    // Run the server until it is asked to quit.
    LOG(INFO) << "RPC service is running";
    server.RunUntilAskedToQuit();
    return EXIT_SUCCESS;
}