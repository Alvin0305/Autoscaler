#ifndef VM_MONITOR_HPP
#define VM_MONITOR_HPP

#include "VM.hpp"
#include <vector>

struct MonitorResult {
    enum Action { SCALE_UP, SCALE_DOWN };

    Action action;
    const VM &vm;

  private:
    MonitorResult(const VM &vm, const Action &action) : vm(vm), action(action) {};

  public:
    static MonitorResult scaleup(const VM &vm) {
        return MonitorResult(vm, SCALE_UP);
    }

    static MonitorResult scaledown(const VM &vm) {
        return MonitorResult(vm, SCALE_DOWN);
    }
};

class VMMonitor {
  private:
    const std::vector<VM> &vms;
    unsigned int interval;

    unsigned int calculateCPUUsage(unsigned long long cpuTime1, unsigned long long cpuTime2, unsigned int numCPUs,
                                   double elapsedTime) const;

  public:
    explicit VMMonitor(const std::vector<VM> &vms, unsigned int interval) : vms(vms), interval(interval) {};

    MonitorResult monitorUntilMaxOrMin(unsigned int maxThreshold, unsigned int minThreshold);
};

#endif
