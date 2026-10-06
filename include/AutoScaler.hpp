#ifndef AUTO_SCALER_HPP
#define AUTO_SCALER_HPP

#include "VMCloner.hpp"
#include "VMManager.hpp"

class AutoScaler {
  private:
    static inline std::string baseVMName = "autoscaler-base";

    VMManager manager;
    VMCloner cloner;

  public:
    AutoScaler() : manager(), cloner(manager.getConnectionPtr()) {};
    void start() const;
};

#endif
