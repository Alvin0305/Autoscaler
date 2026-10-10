#include "tools/WorkGenerator.hpp"
#include "Constants.hpp"
#include "model/LoadState.hpp"
#include "model/Work.hpp"
#include "utils/RandomGenerator.hpp"

Work WorkGenerator::generate(const LoadState &state) const {
    unsigned int num;
    switch (state) {
        case LoadState::LOW:
            num = RandomGenerator::generate(0, ClientConstants::WORK_VALUE_IN_LOW_LOAD);
            return Work{num};
        case LoadState::HIGH:
            num = RandomGenerator::generate(ClientConstants::WORK_VALUE_IN_LOW_LOAD, ClientConstants::WORK_VALUE_IN_HIGH_LOAD);
            return Work{num};
        default:
            return Work{0};
    }
}
