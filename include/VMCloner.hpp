#ifndef VM_CLONER_HPP
#define VM_CLONER_HPP

#include "VM.hpp"
#include "utils/IDGenerator.hpp"
#include <libvirt/libvirt.h>
#include <string>

class VMCloner {
  private:
    const virConnectPtr connection;
    const IDGenerator idGenerator;

    std::string getSourceVMDiskPath() const;

    std::string buildNewDiskPath(const std::string &sourceVMDiskPath, const std::string &newVMName) const;
    std::string buildStorageVolXML(const std::string &diskName, unsigned long long capacity) const;

    std::string generateNewVMXml() const;
    void cloneDisk(const std::string &sourceVMDiskPath, const std::string &cloneVMDiskPath) const;

  public:
    explicit VMCloner(const virConnectPtr connection) : connection(connection), idGenerator() {};

    VM clone(const std::string &sourceVMName, const std::string &newName) const;
};

#endif
