/**
 * @file client.cpp
 * @brief A *bRPC* echo client.
 */

#include "echo.pb.h"

#include <brpc/channel.h>

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <format>
#include <string_view>

namespace {

inline constexpr std::string_view ip {"127.0.0.1"};
inline constexpr std::uint16_t port {8000};

}  // namespace

int main() {
    // Create a RPC communication channel.
    brpc::Channel channel;

    // Set channel options.
    brpc::ChannelOptions options;
    options.timeout_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::seconds {1}).count();
    options.max_retry = 3;

    // Initialize the channel with server address and options.
    if (channel.Init(std::format("{}:{}", ip, port).c_str(), &options) != 0) {
        LOG(FATAL) << "Fail to initialize RPC channel";
        return EXIT_FAILURE;
    }

    // Create and populate the request message.
    echo::EchoRequest request;
    const auto pairs {request.mutable_messages()->mutable_pairs()};
    (*pairs)["name"] = "Mike";
    (*pairs)["Gender"] = "Male";

    // Create a stub for the echo service, binding it to the channel.
    echo::EchoService_Stub service {&channel};

    // Prepare the response and controller objects.
    echo::EchoResponse response;
    brpc::Controller ctrl;

    // Make a RPC call.
    service.Echo(&ctrl, &request, &response, nullptr);

    // Check if the RPC call failed.
    if (ctrl.Failed()) {
        LOG(FATAL) << std::format("Fail to call RPC service: {}", ctrl.ErrorText());
        return EXIT_FAILURE;
    }

    // Print the response messages.
    std::ranges::for_each(response.messages().pairs(), [](const auto& pair) {
        const auto& [key, val] {pair};
        LOG(INFO) << std::format("Receive: {} -> {}", key, val);
    });

    return EXIT_SUCCESS;
}