#include "core/VMMonitor.hpp"
#include "logger/Logger.hpp"
#include <chrono>
#include <thread>

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
    LOG_INFO("Started monitoring VM");

    auto info = vm.getInfo();
    auto prevTime = std::chrono::steady_clock::now();
    auto lastCPUTime = info.cpuTime;

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(interval));

        const auto currentTime = std::chrono::steady_clock::now();
        double elapsedSeconds = std::chrono::duration<double>(currentTime - prevTime).count();

        info = vm.getInfo();
        unsigned int cpuUsage = calculateCPUUsage(lastCPUTime, info.cpuTime, info.nrVirtCpu, elapsedSeconds);
        LOG_INFO("Usage: {}%", cpuUsage);

        lastCPUTime = info.cpuTime;
        prevTime = currentTime;

        if (cpuUsage < minThreshold) {
            return MonitorResult::SCALE_DOWN;
        }

        if (cpuUsage > maxThreshold) {
            return MonitorResult::SCALE_UP;
        }
    }
}
