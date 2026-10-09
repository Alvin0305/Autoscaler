#include "core/VMMonitor.hpp"
#include "logger/Logger.hpp"
#include <chrono>
#include <libvirt/libvirt.h>
#include <thread>
#include <vector>

unsigned int VMMonitor::calculateCPUUsage(unsigned long long cpuTime1, unsigned long long cpuTime2, unsigned int numCPUs,
                                          double elapsedTime) const {
    const double cpuTimeDelta = static_cast<double>(cpuTime2 - cpuTime1);
    const double cpuSeconds = static_cast<double>(cpuTimeDelta / 1'000'000'000.0);
    const double usage = (cpuSeconds / (numCPUs * elapsedTime)) * 100;

    LOG_DEBUG("\tCPU Time Delta: {}\n\tCPU Seconds: {}\n\tElapsed Seconds: {}\n\tvCPUs: {}", cpuTimeDelta, cpuSeconds,
              elapsedTime, numCPUs);
    return static_cast<int>(usage);
}

MonitorResult VMMonitor::monitorUntilMaxOrMin(unsigned int maxThreshold, unsigned int minThreshold) {
    LOG_INFO("Started monitoring {} VMs", vms.size());

    std::vector<unsigned long long> prevCpuTimes;
    for (const auto &vm : vms) {
        auto info = vm.getInfo();
        prevCpuTimes.push_back(info.cpuTime);
    }

    auto prevTime = std::chrono::steady_clock::now();

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(interval));

        for (int i = vms.size() - 1; i >= 0; i--) {
            const auto &vm = vms[i];
            const auto currentTime = std::chrono::steady_clock::now();
            double elapsedSeconds = std::chrono::duration<double>(currentTime - prevTime).count();

            auto newCpuInfo = vm.getInfo();
            unsigned int cpuUsage = calculateCPUUsage(prevCpuTimes[i], newCpuInfo.cpuTime, newCpuInfo.nrVirtCpu, elapsedSeconds);
            LOG_INFO("{}: {}%", vm.getName(), cpuUsage);

            prevCpuTimes[i] = newCpuInfo.cpuTime;
            prevTime = currentTime;

            if (cpuUsage < minThreshold) {
                return MonitorResult::scaledown(vm);
            }

            if (cpuUsage > maxThreshold) {
                return MonitorResult::scaleup(vm);
            }
        }
    }
}
