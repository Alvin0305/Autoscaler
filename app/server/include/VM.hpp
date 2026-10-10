#ifndef VM_HPP
#define VM_HPP

#include <libvirt/libvirt.h>
#include <string>
#include <vector>

class VMManager;
class VMCloner;
class VMDestroyer;

class VM {
  private:
    std::string name;
    virDomainPtr domainPtr;

  public:
    explicit VM(virDomainPtr domain, const std::string &name) : domainPtr(domain), name(name) {};
    ~VM();

    VM(const VM &other) = delete;
    VM &operator=(const VM &other) = delete;
    VM(VM &&other) noexcept;
    VM &operator=(VM &&other) noexcept;

    bool operator==(const VM &other) const;

    void start() const;
    void shutdown() const;
    void destroy() const;

    int getState() const;
    virDomainInfo getInfo() const;
    std::vector<virDomainMemoryStatStruct> getMemoryStats() const;

    std::string getIP() const;

    friend class VMManager;
    friend class VMCloner;
    friend class VMDestroyer;

    std::string getName() const {
        return name;
    }
};

#endif
