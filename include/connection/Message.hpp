#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>

struct Message {
    enum class Action { VM_ADDED, VM_DESTROYED };

    Action action;
    std::string vmName;
    std::string ip;

    static Message add(const std::string &vmName, const std::string &ip) {
        return Message{Message::Action::VM_ADDED, vmName, ip};
    }

    static Message destroy(const std::string &vmName, const std::string &ip) {
        return Message{Message::Action::VM_DESTROYED, vmName, ip};
    }
};

#endif
