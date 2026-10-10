#ifndef WORK_HPP
#define WORK_HPP

#include <ostream>
#include <string>

struct Work {
    unsigned int value;

    std::string serialize() const;
};

inline std::ostream &operator<<(std::ostream &out, const Work &work) {
    out << "{ value: " << work.value << " }";
    return out;
}

#endif
