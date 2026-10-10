#ifndef LOAD_STATE_HPP
#define LOAD_STATE_HPP

#include <ostream>

enum class LoadState { LOW, HIGH };

inline std::ostream &operator<<(std::ostream &out, const LoadState &state) {
    switch (state) {
        case LoadState::LOW:
            out << "LOW";
        case LoadState::HIGH:
            out << "HIGH";
        default:
            out << "Unknown";
    }

    return out;
}

#endif
