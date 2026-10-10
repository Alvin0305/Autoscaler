#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

namespace ClientConstants {
    const unsigned int WORKER_SERVER_PORT = 8080;

    const unsigned int PORT = 8000;

    const unsigned int WORK_INTERVAL_IN_LOW_LOAD = 1000;
    const unsigned int WORK_INTERVAL_IN_HIGH_LOAD = 100;
    const unsigned int WORK_VALUE_IN_LOW_LOAD = 20;
    const unsigned int WORK_VALUE_IN_HIGH_LOAD = 42;
} // namespace Constants

#endif
