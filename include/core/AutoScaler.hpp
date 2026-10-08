#ifndef AUTO_SCALER_HPP
#define AUTO_SCALER_HPP

#include "VM.hpp"
#include "VMManager.hpp"
#include "scaler/VMCloner.hpp"
#include "scaler/VMDestroyer.hpp"
#include "utils/IDGenerator.hpp"
#include <optional>
#include <vector>

class AutoScaler {
  private:
    static inline std::string baseVMName = "autoscaler-base";
    static inline std::string vmName = "autoscaler-server1";
    static inline std::string cloneVMName = "autoscaler-server2";

    std::vector<VM> vms;

    VMManager manager;
    VMCloner cloner;
    VMDestroyer destroyer;
    IDGenerator idGenerator;

    std::optional<VM> secondVM;

    void listAllVMs() const;
    void printInfo() const;

    void populateVMs();

    void scaleUp();
    void scaleDown(const VM &vm);
    void monitorAndScale();

  public:
    AutoScaler() : manager(), cloner(manager.getConnectionPtr()), destroyer(manager.getConnectionPtr()), idGenerator() {};
    void start();
};

#endif
