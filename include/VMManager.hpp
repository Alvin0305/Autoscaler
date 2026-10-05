#ifndef VM_MANAGER_HPP
#define VM_MANAGER_HPP

#include "VM.hpp"
#include <libvirt/libvirt.h>
#include <string>
#include <vector>

class VMManager {
  private:
    virConnectPtr connection;

  public:
    explicit VMManager() = default;
    ~VMManager();

    VMManager(const VMManager &other) = delete;
    VMManager &operator=(const VMManager &other) = delete;
    VMManager(VMManager &&other) noexcept;
    VMManager &operator=(VMManager &&other) noexcept;

    bool connect();
    void disconnect();

    std::vector<std::string> getAllVMNames() const;
    VM getVMByName(const std::string &name) const;

    void startVM(const std::string &name) const;
    void startVM(const VM &vm) const;
    void shutdownVM(const std::string &name) const;
    void shutdownVM(const VM &vm) const;

    int getStateOfVM(const std::string &name) const;
    int getStateOfVM(const VM &vm) const;
};

#endif
