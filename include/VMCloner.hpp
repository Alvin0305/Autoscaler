#ifndef VM_CLONER_HPP
#define VM_CLONER_HPP

#include "VM.hpp"
#include <libvirt/libvirt.h>
#include <string>

class VMCloner {
  private:
    const virConnectPtr connection;

    std::string getSourceVMDiskPath() const;

    std::string generateUUID() const;
    std::string generateMAC() const;
    std::string generateNewDiskPath(const std::string &sourceVMDiskPath, const std::string &newVMName) const;

    std::string generateNewVMXml() const;
    void cloneDisk(const std::string &sourceVMDiskPath, const std::string &cloneVMDiskPath) const;

  public:
    explicit VMCloner(const virConnectPtr connection) : connection(connection) {};

    VM clone(const std::string &sourceVMName, const std::string &newName) const;
};

#endif
