#include "core/VMManager.hpp"
#include "Constants.hpp"
#include "VM.hpp"
#include "exception/VMNotFoundException.hpp"
#include "logger/Logger.hpp"
#include <libvirt/libvirt.h>
#include <stdexcept>
#include <string>
#include <vector>

VMManager::VMManager() {
    if (not connect()) {
        throw std::runtime_error("Connection failed");
    }
}

VMManager::~VMManager() {
    disconnect();
}

VMManager::VMManager(VMManager &&other) noexcept : connection(other.connection) {
    other.connection = nullptr;
}

VMManager &VMManager::operator=(VMManager &&other) noexcept {
    if (this != &other) {
        if (connection != nullptr) {
            virConnectClose(connection);
        }

        connection = other.connection;
        other.connection = nullptr;
    }

    return *this;
}

bool VMManager::connect() {
    connection = virConnectOpen(Constants::QEMU_URL);
    LOG_INFO("Connected to: {}", virConnectGetURI(connection));
    return connection != nullptr;
}

void VMManager::disconnect() {
    if (connection != nullptr) {
        virConnectClose(connection);
    }
}

std::vector<std::string> VMManager::getAllVMNames() const {
    virDomainPtr *domains = nullptr;
    int numDomains = virConnectListAllDomains(connection, &domains, 0);

    if (numDomains == 0) {
        LOG_WARN("No VMs");
    }

    std::vector<std::string> domainNames(numDomains);
    for (int i = 0; i < numDomains; i++) {
        domainNames[i] = virDomainGetName(domains[i]);
    }

    free(domains);
    return domainNames;
}

VM VMManager::getVMByName(const std::string &name) const {
    virDomainPtr domainPtr = virDomainLookupByName(connection, name.c_str());

    if (domainPtr == nullptr) {
        throw VMNotFoundException(name);
    }
    return VM(domainPtr, name);
}

void VMManager::startVM(const std::string &name) const {
    LOG_INFO("Starting {}", name);
    VM vm = getVMByName(name);
    vm.start();
}

void VMManager::shutdownVM(const std::string &name) const {
    LOG_INFO("Shutting down {}", name);
    VM vm = getVMByName(name);
    vm.shutdown();
}

int VMManager::getStateOfVM(const std::string &name) const {
    VM vm = getVMByName(name);
    return vm.getState();
}

virDomainInfo VMManager::getInfoOfVM(const std::string &name) const {
    VM vm = getVMByName(name);
    return vm.getInfo();
}

std::vector<virDomainMemoryStatStruct> VMManager::getMemInfoOfVM(const std::string &name) const {
    VM vm = getVMByName(name);
    return vm.getMemoryStats();
}

const virConnectPtr VMManager::getConnectionPtr() const {
    return connection;
}
