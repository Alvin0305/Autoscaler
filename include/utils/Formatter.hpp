#ifndef FORMATTER_HPP
#define FORMATTER_HPP

#include "connection/Message.hpp"
#include "utils/utils.hpp"
#include <format>
#include <sstream>
#include <string>

template <> struct std::formatter<virDomainInfo> : std::formatter<std::string> {
    auto format(const virDomainInfo &vm, auto &ctx) const {
        std::ostringstream oss;
        oss << vm;

        return std::formatter<std::string>::format(oss.str(), ctx);
    }
};

template <> struct std::formatter<Message> : std::formatter<std::string> {
    auto format(const Message &message, auto &ctx) const {
        std::ostringstream oss;
        oss << message;

        return std::formatter<std::string>::format(oss.str(), ctx);
    }
};

#endif
