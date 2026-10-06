#include "VMMonitor.hpp"
#include <chrono>
#include <iostream>
#include <thread>

unsigned int VMMonitor::calculateCPUUsage(unsigned long long cpuTime1, unsigned long long cpuTime2, unsigned int numCPUs,
                                          double elapsedTime) const {
    const double cpuTimeDelta = static_cast<double>(cpuTime2 - cpuTime1);
    const double cpuSeconds = static_cast<double>(cpuTimeDelta / 1'000'000'000.0);
    const double usage = (cpuSeconds / (numCPUs * elapsedTime)) * 100;

    std::cout << "\tCPU Time Delta: " << cpuTimeDelta << "\n\tCPU Seconds: " << cpuSeconds
              << "\n\tElapsed Seconds: " << elapsedTime << "\n\tvCPUs: " << numCPUs << std::endl;
    return static_cast<int>(usage);
}

bool VMMonitor::monitorUntilMax(unsigned int threshold) {
    std::cout << "Started monitoring VM" << std::endl;

    auto info = vm.getInfo();
    auto prevTime = std::chrono::steady_clock::now();
    auto lastCPUTime = info.cpuTime;

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(interval));

        const auto currentTime = std::chrono::steady_clock::now();
        double elapsedSeconds = std::chrono::duration<double>(currentTime - prevTime).count();

        info = vm.getInfo();
        unsigned int cpuUsage = calculateCPUUsage(lastCPUTime, info.cpuTime, info.nrVirtCpu, elapsedSeconds);
        std::cout << "Usage: " << cpuUsage << "%\n";

        lastCPUTime = info.cpuTime;
        prevTime = currentTime;

        if (cpuUsage > threshold) {
            return true;
        }
    }
}
