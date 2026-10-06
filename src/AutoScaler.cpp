#include "AutoScaler.hpp"
#include "VM.hpp"
#include "VMManager.hpp"
#include "VMMonitor.hpp"
#include "utils/utils.hpp"
#include <iostream>
#include <libvirt/libvirt.h>
#include <stdexcept>

void AutoScaler::start() const {
    VMManager manager;

    const auto vmNames = manager.getAllVMNames();
    std::cout << "VMs: " << std::endl;
    for (const auto &vmName : vmNames) {
        std::cout << vmName << std::endl;
    }

    const auto vmName = vmNames[1];

    try {
        if (manager.getStateOfVM(vmName) != VIR_DOMAIN_RUNNING) {
            std::cout << "Starting " << vmName << std::endl;
            manager.startVM(vmName);
            std::cout << "State of " << vmName << " is now: " << VMStateToString(manager.getStateOfVM(vmName)) << std::endl;
        }

        virDomainInfo info = manager.getInfoOfVM(vmName);
        std::cout << info;
        const auto memStats = manager.getMemInfoOfVM(vmName);
        std::cout << memStatsToString(memStats);

        const VM &vm = manager.getVMByName(vmName);
        VMMonitor monitor(vm, 5);
        MonitorResult result = monitor.monitorUntilMaxOrMin(80, 10);

        if (result == MonitorResult::SCALE_UP) {
            std::cout << "Reached MAX Threshold. Triggering cloning" << std::endl;
            std::string cloneVMName = "autoscaler-server2";

            cloner.clone(baseVMName, cloneVMName);
            manager.startVM(cloneVMName);
            std::cout << "State of " << vmName << " is now: " << VMStateToString(manager.getStateOfVM(vmName)) << std::endl;
        } else {
            std::cout << "Reached MIN Threashold. Triggering decloning" << std::endl;
        }
    } catch (std::runtime_error &e) {
        std::cerr << "Runtime Exception occured: " << e.what() << std::endl;
    }
}
