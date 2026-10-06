#ifndef VM_HPP
#define VM_HPP

#include <libvirt/libvirt.h>
#include <vector>

class VMManager;

class VM {
  private:
    virDomainPtr domainPtr;

  public:
    explicit VM(virDomainPtr domain) : domainPtr(domain) {};
    ~VM();

    VM(const VM &other) = delete;
    VM &operator=(const VM &other) = delete;

    VM(VM &&other) noexcept;
    VM &operator=(VM &&other) noexcept;

    void start() const;
    void shutdown() const;

    int getState() const;
    virDomainInfo getInfo() const;
    std::vector<virDomainMemoryStatStruct> getMemoryStats() const;

    friend class VMManager;
};

#endif
