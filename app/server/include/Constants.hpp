#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>

namespace ServerConstants {
    static inline constexpr char QEMU_URL[] = "qemu:///system";

    static inline constexpr std::string DISK_IMAGE_FILE_EXTENSION = ".qcow2";
    static inline constexpr char DEFAULT_STORAGE_POOL_NAME[] = "default";

    static inline const unsigned int MONITORING_INTERVAL = 5;
    static inline const unsigned int MIN_CPU_USAGE_THRESHOLD = 40;
    static inline const unsigned int MAX_CPU_USAGE_THRESHOLD = 70;

    static inline const std::string CLIENT_IP = "127.0.0.1";
    static inline const unsigned int CLIENT_PORT = 8000;
}; // namespace Constants

#endif
