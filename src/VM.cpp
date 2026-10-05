#include "VM.hpp"
#include <stdexcept>

VM::~VM() {
    if (domainPtr != nullptr) {
        virDomainFree(domainPtr);
    }
}

VM::VM(VM &&other) noexcept : domainPtr(other.domainPtr) {
    other.domainPtr = nullptr;
}

VM &VM::operator=(VM &&other) noexcept {
    if (this != &other) {
        if (domainPtr != nullptr) {
            virDomainFree(domainPtr);
        }

        domainPtr = other.domainPtr;
        other.domainPtr = nullptr;
    }

    return *this;
}

void VM::start() const {
    if (getState() == VIR_DOMAIN_RUNNING) {
        throw std::runtime_error("Invalid State. Tried to run a VM which is already running.");
    }

    int ret = virDomainCreate(domainPtr);
    if (ret != 0) {
        throw std::runtime_error("Failed to start VM");
    }
}

void VM::shutdown() const {
    if (getState() == VIR_DOMAIN_SHUTOFF) {
        throw std::runtime_error("Invalid State. Tried to shutdown a VM which is already shutdown.");
    }

    int ret = virDomainShutdown(domainPtr);
    if (ret != 0) {
        throw std::runtime_error("Failed to stop VM");
    }
}

int VM::getState() const {
    int state;
    int reason;
    int ret = virDomainGetState(domainPtr, &state, &reason, 0);

    if (ret != 0) {
        throw std::runtime_error("Failed to get VM state");
    }

    return state;
}
