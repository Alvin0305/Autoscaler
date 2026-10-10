#include "utils/RandomGenerator.hpp"
#include <random>

unsigned int RandomGenerator::generate(unsigned int start, unsigned int end) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> distribution(start, end);
    return distribution(gen);
}
