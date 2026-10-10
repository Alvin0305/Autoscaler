#ifndef VM_DESTROYER_HPP
#define VM_DESTROYER_HPP

#include "VM.hpp"
#include <libvirt/libvirt.h>

class VMDestroyer {
  private:
    virConnectPtr connection;

    void turnOffVM(const VM &vm) const;
    void destroyStorage(const VM &vm) const;

  public:
    VMDestroyer(virConnectPtr connection) : connection(connection) {};

    void destroy(const VM &vm) const;
};

#endif
