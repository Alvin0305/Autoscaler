#include "core/AutoScaler.hpp"
#include "Constants.hpp"
#include "VM.hpp"
#include "core/VMManager.hpp"
#include "core/VMMonitor.hpp"
#include "logger/Logger.hpp"
#include "utils/IDGenerator.hpp"
#include "utils/utils.hpp"
#include <algorithm>
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

void AutoScaler::populateVMs() {
    const auto vmNames = manager.getAllVMNames();
    for (const auto &vmName : vmNames) {
        if (vmName == baseVMName) continue;
        vms.push_back(manager.getVMByName(vmName));
    }
}

void AutoScaler::scaleDown(const VM &vm) {
    LOG_INFO("VM {} Reached MIN Threshold...", vm.getName());
    if (vms.size() == 1) {
        LOG_INFO("There is only one VM. Scale down NOT initiated");
        return;
    }

    LOG_INFO("There are {} VMs. Scale down initiated...", vms.size());
    destroyer.destroy(vm);
    auto it = std::find(vms.begin(), vms.end(), vm);
    if (it != vms.end()) {
        vms.erase(it);
    }

    LOG_INFO("VM destroyed");
}

void AutoScaler::scaleUp() {
    LOG_INFO("Reached MAX Threshold. Scale up initiated...");

    std::string cloneVMName = idGenerator.generateVMName();
    vms.push_back(cloner.clone(baseVMName, cloneVMName));
    manager.startVM(cloneVMName);

    LOG_INFO("VM {} Cloned to {} and started.", vmName, cloneVMName);
    LOG_INFO("State of {} is: {}", cloneVMName, VMStateToString(manager.getStateOfVM(cloneVMName)));
}

void AutoScaler::monitorAndScale() {
    while (true) {
        VMMonitor monitor(vms, Constants::MONITORING_INTERVAL);
        MonitorResult result =
            monitor.monitorUntilMaxOrMin(Constants::MAX_CPU_USAGE_THRESHOLD, Constants::MIN_CPU_USAGE_THRESHOLD);

        switch (result.action) {
            case MonitorResult::SCALE_UP:
                scaleUp();
                break;
            case MonitorResult::SCALE_DOWN:
                scaleDown(result.vm);
                break;
            default:
                LOG_ERROR("Unhandled action by VMMonitor: {}", static_cast<int>(result.action));
        }
    }
}

void AutoScaler::start() {
    VMManager manager;
    listAllVMs();
    printInfo();

    populateVMs();
    LOG_INFO("Watching {} VMs", vms.size());
    for (const auto &vm : vms) {
        LOG_INFO("- {}", vm.getName());
    }

    LOG_DEBUG("Starting all VMs");
    for (const auto &vm : vms) {
        if (manager.getStateOfVM(vmName) == VIR_DOMAIN_RUNNING) continue;
        manager.startVM(vmName);
    }

    try {
        monitorAndScale();
    } catch (std::runtime_error &e) {
        LOG_ERROR("Runtime Exception occured: {}", e.what());
    }
}
