#include "utils/IDGenerator.hpp"
#include "utils/RandomGenerator.hpp"
#include <ios>
#include <sstream>
#include <string>

std::string IDGenerator::generateMAC() const {
    std::stringstream ss;
    ss << MAC_STARTING;

    for (int i = 0; i < 3; i++) {
        int randomNum = RandomGenerator::generate(0, 15);
        ss << std::hex << randomNum;
        randomNum = RandomGenerator::generate(0, 15);
        ss << std::hex << randomNum << std::dec;

        if (i != 2) ss << ":";
    }

    return ss.str();
}

std::string IDGenerator::generateUUID() const {
    std::stringstream ss;
    const int groups[] = {8, 4, 4, 4, 12};

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < groups[i]; j++) {
            int randomNum = RandomGenerator::generate(0, 15);
            ss << std::hex << randomNum << std::dec;
        }

        if (i != 4) {
            ss << "-";
        }
    }

    return ss.str();
}

std::string IDGenerator::generateVMName() const {
    static int count = 1;
    return "autoscaler-server" + std::to_string(++count);
}
