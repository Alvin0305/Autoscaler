#ifndef RANDOM_GENERATOR_HPP
#define RANDOM_GENERATOR_HPP

#include <random>

class RandomGenerator {
  public:
    static inline unsigned int generate(unsigned int start, unsigned int end) {
        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_int_distribution<> distribution(start, end);
        return distribution(gen);
    }
};

#endif
