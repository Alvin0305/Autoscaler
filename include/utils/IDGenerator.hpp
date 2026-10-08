#ifndef ID_GENERATOR_HPP
#define ID_GENERATOR_HPP

#include <string>

class IDGenerator {
  private:
    static inline std::string MAC_STARTING = "52:54:00:";

  public:
    explicit IDGenerator() = default;
    IDGenerator(const IDGenerator &other) = delete;
    IDGenerator operator=(const IDGenerator &other) = delete;

    std::string generateMAC() const;
    std::string generateUUID() const;
    std::string generateVMName() const;
};

#endif
