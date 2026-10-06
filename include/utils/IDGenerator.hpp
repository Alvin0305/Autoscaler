#ifndef ID_GENERATOR_HPP
#define ID_GENERATOR_HPP

#include <string>

class IDGenerator {
  private:
    static inline std::string MAC_STARTING = "52:54:00:";

  public:
    std::string generateMAC() const;
    std::string generateUUID() const;
};

#endif
