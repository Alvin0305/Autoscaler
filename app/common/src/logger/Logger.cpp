#include "logger/Logger.hpp"

#include <chrono>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <ostream>
#include <sstream>
#include <sys/ioctl.h>
#include <unistd.h>

std::string timestamp() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t time = std::chrono::system_clock::to_time_t(now);

    std::tm localTime{};

    localtime_r(&time, &localTime);

    std::ostringstream stream;
    stream << std::put_time(&localTime, "%H:%M:%S");

    return stream.str();
}

void Logger::print(std::ostream &output, const char *level, const char *color, const char *file, int lineNo,
                   const std::string &message) {
    std::istringstream stream(message);
    std::string line;
    std::ostringstream formattedBuffer;

    const char *filename = std::strrchr(file, '/');
    filename = filename ? filename + 1 : file;

    while (std::getline(stream, line)) {
        formattedBuffer << Gray << "[" << timestamp() << "] " << Reset << Bold << "[" << filename << ":" << lineNo << "] "
                        << color << "[" << level << "]\t" << Reset << line << std::endl;
    }

    {
        std::lock_guard<std::mutex> lock(logMutex);
        output << formattedBuffer.str() << std::flush;
    }
}

std::size_t Logger::terminalWidth() {
    struct winsize size{};

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == 0 && size.ws_col > 0) {
        return size.ws_col;
    }

    return 80;
}
