#include "VMManager.hpp"
#include "utils/utils.hpp"
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>

int main() {
    VMManager manager;
    if (not manager.connect()) {
        std::cerr << "Failed to connect" << std::endl;
        return 1;
    }

    const auto vmNames = manager.getAllVMNames();
    for (const auto &vmName : vmNames) {
        std::cout << vmName << std::endl;
    }

    const auto vmName = vmNames[0];

    try {
        std::cout << "State of " << vmName << " is now: " << VMStateToString(manager.getStateOfVM(vmName)) << std::endl;
        std::cout << "Starting " << vmName << std::endl;
        manager.startVM(vmName);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "State of " << vmName << " is now: " << VMStateToString(manager.getStateOfVM(vmName)) << std::endl;
        std::cout << "Stopping " << vmName << std::endl;
        manager.shutdownVM(vmName);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "State of " << vmName << " is now: " << VMStateToString(manager.getStateOfVM(vmName)) << std::endl;
    } catch (std::runtime_error &e) {
        std::cerr << "Runtime Exception occured: " << e.what() << std::endl;
    }

    return 0;
}
