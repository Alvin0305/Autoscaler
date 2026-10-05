#include "utils/utils.hpp"

std::string VMStateToString(int state) {
    switch (state) {
        case VIR_DOMAIN_RUNNING:
            return "RUNNING";
        case VIR_DOMAIN_SHUTOFF:
            return "SHUTOFF";
        case VIR_DOMAIN_PAUSED:
            return "PAUSED";
        default:
            return "Unhandled State";
    }
}
