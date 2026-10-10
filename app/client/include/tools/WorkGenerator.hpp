#ifndef WORK_GENERATOR_HPP
#define WORK_GENERATOR_HPP

#include "model/LoadState.hpp"
#include "model/Work.hpp"

class WorkGenerator {
  public:
    Work generate(const LoadState &state) const;
};

#endif
