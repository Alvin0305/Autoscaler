#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <nlohmann/json.hpp>
#include <string>

struct Message {
    enum class Action { VM_ADDED, VM_DESTROYED, NONE };

    Action action;
    std::string name;
    std::string ip;

    static Message add(const std::string &vmName, const std::string &ip) {
        return Message{Message::Action::VM_ADDED, vmName, ip};
    }

    static Message destroy(const std::string &vmName, const std::string &ip) {
        return Message{Message::Action::VM_DESTROYED, vmName, ip};
    }

    static Message none() {
        return Message{Message::Action::NONE, "", ""};
    }

    static Message fromString(const std::string &string);
};

#endif
