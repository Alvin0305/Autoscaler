#include "scaler/VMDestroyer.hpp"
#include "logger/Logger.hpp"
#include "tools/XMLParser.hpp"
#include <chrono>
#include <libvirt/libvirt.h>
#include <string>
#include <thread>

void VMDestroyer::turnOffVM(const VM &vm) const {
    LOG_DEBUG("Turning Off VM");

    vm.shutdown();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    if (vm.getState() == VIR_DOMAIN_RUNNING) {
        LOG_INFO("VM is unresponsive for {} seconds. Force shutdown", 2);
        vm.destroy();
    }
}

void VMDestroyer::destroyStorage(const VM &vm) const {
    LOG_DEBUG("Destroying Storage");
    char *xml = virDomainGetXMLDesc(vm.domainPtr, 0);
    XMLParser parser(xml);

    const auto storageVolPaths = parser.getAllVolumePaths();
    for (const auto &storageVolPath : storageVolPaths) {
        LOG_DEBUG("\tDestroying Volume: {}", storageVolPath);
        const auto storageVolPtr = virStorageVolLookupByPath(connection, storageVolPath.c_str());
        virStorageVolDelete(storageVolPtr, 0);
    }
}

void VMDestroyer::destroy(const VM &vm) const {
    turnOffVM(vm);

    destroyStorage(vm);

    LOG_DEBUG("Destroying VM");
    if (virDomainUndefine(vm.domainPtr) != 0) {
        LOG_ERROR("Failed to Undefine VM");
        return;
    }
}
