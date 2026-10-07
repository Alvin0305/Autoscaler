#ifndef AUTO_SCALER_HPP
#define AUTO_SCALER_HPP

#include "VM.hpp"
#include "VMManager.hpp"
#include "scaler/VMCloner.hpp"
#include "scaler/VMDestroyer.hpp"
#include <optional>

class AutoScaler {
  private:
    static inline std::string baseVMName = "autoscaler-base";
    static inline std::string vmName = "autoscaler-server1";
    static inline std::string cloneVMName = "autoscaler-server2";

    VMManager manager;
    VMCloner cloner;
    VMDestroyer destroyer;

    std::optional<VM> secondVM;

    void listAllVMs() const;
    void printInfo() const;

    void scaleUp();
    void scaleDown();
    void monitorAndScale();

  public:
    AutoScaler() : manager(), cloner(manager.getConnectionPtr()), destroyer(manager.getConnectionPtr()) {};
    void start();
};

#endif
