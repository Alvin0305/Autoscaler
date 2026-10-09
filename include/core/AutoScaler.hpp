#ifndef AUTO_SCALER_HPP
#define AUTO_SCALER_HPP

#include "VM.hpp"
#include "VMManager.hpp"
#include "connection/MessageSender.hpp"
#include "scaler/VMCloner.hpp"
#include "scaler/VMDestroyer.hpp"
#include "utils/IDGenerator.hpp"
#include <vector>

class AutoScaler {
  private:
    static inline std::string baseVMName = "autoscaler-base";

    std::vector<VM> vms;

    VMManager manager;
    VMCloner cloner;
    VMDestroyer destroyer;
    IDGenerator idGenerator;
    MessageSender sender;

    void listAllVMs() const;
    void printInfo() const;

    void populateVMs();

    void scaleUp();
    void scaleDown(const VM &vm);
    void monitorAndScale();

  public:
    AutoScaler()
        : manager(), cloner(manager.getConnectionPtr()), destroyer(manager.getConnectionPtr()), idGenerator(),
          sender("127.0.0.1", 8080) {};
    void start();
};

#endif
