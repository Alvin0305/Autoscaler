#include "connection/MessageSender.hpp"
#include "logger/Logger.hpp"
#include "utils/utils.hpp"
#include <sstream>

std::ostream &operator<<(std::ostream &out, const Message::Action &action) {
    out << messageActionToString(action);
    return out;
}

const std::string MessageSender::buildMessageBody(const Message &message) const {
    std::stringstream ss;
    ss << "{\n";
    ss << "\tevent: " << message.action << ",\n";
    ss << "\tname: " << message.vmName << ",\n";
    ss << "\tip: " << message.ip << '\n';
    ss << "}\n";

    return ss.str();
}

const std::string MessageSender::buildMessage(const std::string &body) const {
    std::stringstream ss;
    ss << "POST /autoscaler/scale HTTP/1.1\r\n";
    ss << "Content-Type: application/json\r\n";
    ss << "Content-Length: " << body.size() << "\r\n";
    ss << "\r\n";
    ss << body;
    return ss.str();
}

void MessageSender::send(const Message &message) const {
    const std::string body = buildMessageBody(message);
    const std::string bytes = buildMessage(body);

    LOG_DEBUG("Sending Message: {}", bytes);
    socket.send(bytes);
}
