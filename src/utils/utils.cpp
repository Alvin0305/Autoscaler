#include "utils/utils.hpp"
#include <libvirt/libvirt.h>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

std::string VMStateToString(int state) {
    switch (state) {
        case VIR_DOMAIN_RUNNING:
            return "RUNNING";
        case VIR_DOMAIN_SHUTOFF:
            return "SHUTOFF";
        case VIR_DOMAIN_PAUSED:
            return "PAUSED";
        default:
            return "Unhandled State";
    }
}

std::string memStatTagToString(int tag) {
    switch (tag) {
        case VIR_DOMAIN_MEMORY_STAT_ACTUAL_BALLOON:
            return "ACTUAL_BALLOON";
        case VIR_DOMAIN_MEMORY_STAT_SWAP_IN:
            return "SWAP_IN";
        case VIR_DOMAIN_MEMORY_STAT_SWAP_OUT:
            return "SWAP_OUT";
        case VIR_DOMAIN_MEMORY_STAT_MAJOR_FAULT:
            return "MAJOR_FAULT";
        case VIR_DOMAIN_MEMORY_STAT_MINOR_FAULT:
            return "MINOR_FAULT";
        case VIR_DOMAIN_MEMORY_STAT_UNUSED:
            return "UNUSED";
        case VIR_DOMAIN_MEMORY_STAT_AVAILABLE:
            return "AVAILABLE";
        case VIR_DOMAIN_MEMORY_STAT_USABLE:
            return "USABLE";
        case VIR_DOMAIN_MEMORY_STAT_LAST_UPDATE:
            return "LAST_UPDATE";
        case VIR_DOMAIN_MEMORY_STAT_RSS:
            return "RSS (Memory Used)";
        default:
            return "UNKNOWN";
    }
}

std::string memStatsToString(const std::vector<virDomainMemoryStatStruct> &stats) {
    std::stringstream ss;
    ss << std::string(50, '-') << std::endl;
    ss << "VM Mem Info" << std::endl;
    ss << std::string(50, '-') << std::endl;
    for (const auto &stat : stats) {
        ss << memStatTagToString(stat.tag) << ": " << stat.val << std::endl;
    }
    ss << std::string(50, '-') << std::endl;
    return ss.str();
}

std::string memSizeToString(unsigned long long size) {
    std::vector<std::string> units{"KB", "MB", "GB", "TB"};

    int i = 0;
    while (size >= 1024) {
        size /= 1024;
        i++;
    }

    return std::to_string(size) + " " + units[i];
}

std::ostream &operator<<(std::ostream &out, const virDomainInfo &info) {
    out << std::string(50, '-') << std::endl;
    out << "VM Info" << std::endl;
    out << std::string(50, '-') << std::endl;
    out << "CPU Time: " << info.cpuTime << std::endl;
    out << "Max Memory: " << memSizeToString(info.maxMem) << std::endl;
    out << "Memory Alloted to VM: " << memSizeToString(info.memory) << std::endl;
    out << "# Virtual CPU: " << info.nrVirtCpu << std::endl;
    out << "State: " << VMStateToString(info.state) << std::endl;
    out << std::string(50, '-') << std::endl;
    return out;
}
