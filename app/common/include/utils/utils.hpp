#ifndef UTILS_HPP
#define UTILS_HPP

#include "model/Message.hpp"
#include <libvirt/libvirt.h>
#include <ostream>

std::string messageActionToString(const Message::Action &action);
std::string VMStateToString(int state);

std::string memStatTagToString(int tag);
std::string memStatsToString(const std::vector<virDomainMemoryStatStruct> &stats);
std::string memSizeToString(unsigned long long size);

void to_json(nlohmann::json &j, const Message &message);

std::ostream &operator<<(std::ostream &out, const virDomainInfo &info);
std::ostream &operator<<(std::ostream &out, const Message &message);

#endif
