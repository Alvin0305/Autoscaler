#ifndef UTILS_HPP
#define UTILS_HPP

#include <libvirt/libvirt.h>
#include <ostream>
#include <string>
#include <vector>

std::string VMStateToString(int state);

std::string memStatTagToString(int tag);
std::string memStatsToString(const std::vector<virDomainMemoryStatStruct> &stats);
std::string memSizeToString(unsigned long long size);

std::ostream &operator<<(std::ostream &out, const virDomainInfo &info);

#endif
