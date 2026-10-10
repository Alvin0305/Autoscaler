#include "connection/MessageSender.hpp"
#include "lib/httplib.hpp"
#include "logger/Logger.hpp"
#include "utils/utils.hpp"
#include <nlohmann/json.hpp>

std::ostream &operator<<(std::ostream &out, const Message::Action &action) {
    out << messageActionToString(action);
    return out;
}

const std::string MessageSender::buildMessageBody(const Message &message) const {
    const nlohmann::json j = message;
    return j.dump();
}

void MessageSender::send(const Message &message) {
    const std::string body = buildMessageBody(message);
    LOG_DEBUG("Sending Message: {}", body);
    auto response = client.Post("/scale", body, "application/json");

    if (not response) {
        auto error = response.error();
        LOG_ERROR("Request Failed with error: {}", httplib::to_string(error));
        return;
    }

    LOG_INFO("HTTP Response Status: {}", response->status);
    LOG_INFO("Body: {}", response->body);
    LOG_INFO("Content-Type: {}", response->get_header_value("Content-Type"));
}
