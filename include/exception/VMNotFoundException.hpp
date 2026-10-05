#ifndef VM_NOT_FOUND_EXCEPTION_HPP
#define VM_NOT_FOUND_EXCEPTION_HPP

#include <stdexcept>
#include <string>

class VMNotFoundException : std::runtime_error {
  public:
    explicit VMNotFoundException(const std::string &name) : std::runtime_error("VM not found: " + name) {};
};

#endif
