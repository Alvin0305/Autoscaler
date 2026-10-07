#include "core/AutoScaler.hpp"
#include "Constants.hpp"
#include "VM.hpp"
#include "core/VMManager.hpp"
#include "core/VMMonitor.hpp"
#include "logger/Logger.hpp"
#include "utils/utils.hpp"
#include <libvirt/libvirt.h>
#include <stdexcept>
#include <string>

void AutoScaler::listAllVMs() const {
    const auto vmNames = manager.getAllVMNames();
    LOG_INFO("{}\nVMs\n{}", std::string(50, '-'), std::string(50, '-'));
    for (const auto &vmName : vmNames) {
        LOG_INFO("{}", vmName);
    }
    LOG_INFO("{}", std::string(50, '-'));
}

void AutoScaler::printInfo() const {
    virDomainInfo info = manager.getInfoOfVM(vmName);
    const auto memStats = manager.getMemInfoOfVM(vmName);
    LOG_INFO("{}", info);
    LOG_INFO("{}", memStatsToString(memStats));
}

void AutoScaler::scaleDown() {
    LOG_INFO("Reached MIN Threshold...");
    if (not secondVM) {
        LOG_INFO("There is no second VM. No scale down initiated");
        return;
    }

    LOG_INFO("There is a second VM. Scale down initiated...");
    destroyer.destroy(secondVM.value());
    secondVM = std::nullopt;
    LOG_INFO("Second VM destroyed");
}

void AutoScaler::scaleUp() {
    LOG_INFO("Reached MAX Threshold...");
    if (secondVM) {
        LOG_INFO("There is already a second VM. No scale up initiated");
        return;
    }

    LOG_INFO("There is no second VM. Scale up initiated...");
    secondVM = cloner.clone(baseVMName, cloneVMName);
    manager.startVM(cloneVMName);
    LOG_INFO("VM Cloned and started. State of {} is: {}", vmName, VMStateToString(manager.getStateOfVM(vmName)));
}

void AutoScaler::monitorAndScale() {
    while (true) {
        const VM &vm = manager.getVMByName(vmName);

        VMMonitor monitor(vm, Constants::MONITORING_INTERVAL);
        MonitorResult result =
            monitor.monitorUntilMaxOrMin(Constants::MAX_CPU_USAGE_THRESHOLD, Constants::MIN_CPU_USAGE_THRESHOLD);

        if (result == MonitorResult::SCALE_UP) {
            scaleUp();
        } else {
            scaleDown();
        }
    }
}

void AutoScaler::start() {
    VMManager manager;
    listAllVMs();
    printInfo();

    if (manager.getStateOfVM(vmName) != VIR_DOMAIN_RUNNING) {
        manager.startVM(vmName);
    }

    try {
        monitorAndScale();
    } catch (std::runtime_error &e) {
        LOG_ERROR("Runtime Exception occured: {}", e.what());
    }
}
