#include "model/Message.hpp"
#include "logger/Logger.hpp"

using json = nlohmann::json;

Message Message::fromString(const std::string &string) {
    json data = json::parse(string);

    try {
        std::string action = data.at("action").get<std::string>();
        std::string name = data.at("name").get<std::string>();
        std::string ip = data.at("ip").get<std::string>();

        if (action == "VM_ADDED") {
            return Message::add(name, ip);
        } else if (action == "VM_DESTROYED") {
            return Message::destroy(name, ip);
        } else {
            LOG_DEBUG("None");
            return Message::none();
        }

    } catch (nlohmann::json::exception &e) {
        LOG_ERROR("Invalid Message JSON: {}", string);
        return Message::none();
    }
}
