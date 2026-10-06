#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>

class Constants {
  public:
    static inline const char QEMU_URL[] = "qemu:///system";

    static inline const std::string DISK_IMAGE_FILE_EXTENSION = ".qcow2";
    static inline const char DEFAULT_STORAGE_POOL_NAME[] = "default";

    static inline const std::string DEBUG_ENABLE_ENV_VARNAME = "AUTO_SCALER_DEBUG";

    static inline const unsigned int MONITORING_INTERVAL = 5;
    static inline const unsigned int MIN_CPU_USAGE_THRESHOLD = 40;
    static inline const unsigned int MAX_CPU_USAGE_THRESHOLD = 80;
};

#endif
