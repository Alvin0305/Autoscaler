#ifndef AUTO_SCALER_HPP
#define AUTO_SCALER_HPP

#include "VMCloner.hpp"
#include "VMManager.hpp"

class AutoScaler {
  private:
    static inline std::string baseVMName = "autoscaler-base";
    static inline std::string vmName = "autoscaler-server1";
    static inline std::string cloneVMName = "autoscaler-server2";

    VMManager manager;
    VMCloner cloner;

    void listAllVMs() const;
    void printInfo() const;
    void monitorAndScale() const;

  public:
    AutoScaler() : manager(), cloner(manager.getConnectionPtr()) {};
    void start() const;
};

#endif
