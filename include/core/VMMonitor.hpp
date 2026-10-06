#ifndef VM_MONITOR_HPP
#define VM_MONITOR_HPP

#include "VM.hpp"

enum MonitorResult { SCALE_UP, SCALE_DOWN };

class VMMonitor {
  private:
    const VM &vm;
    unsigned int interval;

  public:
    explicit VMMonitor(const VM &vm, unsigned int interval) : vm(vm), interval(interval) {};

    unsigned int calculateCPUUsage(unsigned long long cpuTime1, unsigned long long cpuTime2, unsigned int numCPUs,
                                   double elapsedTime) const;
    MonitorResult monitorUntilMaxOrMin(unsigned int maxThreshold, unsigned int minThreshold);
};

#endif
