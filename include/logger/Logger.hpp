#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "Constants.hpp"
#include "utils/Formatter.hpp"
#include <format>
#include <iostream>
#include <mutex>
#include <string>

#define LOG_INFO(...) Logger::info(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...) Logger::warn(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(...) Logger::debug(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) Logger::error(__FILE__, __LINE__, __VA_ARGS__)

class Logger {
  public:
    static std::size_t terminalWidth();

    static void line() {
        std::lock_guard<std::mutex> lock(logMutex);
        size_t termWidth = terminalWidth();
        std::string bar;
        std::string hline = "\u2500";
        bar.reserve(hline.size() * termWidth);
        for (size_t i = 0; i < termWidth; i++)
            bar += hline;
        std::cerr << Gray << bar << Reset << std::endl;
    }

    template <typename... Args> static void info(const char *file, int line, std::format_string<Args...> fmt, Args &&...args) {
        print(std::cerr, "INFO", Green, file, line, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args> static void warn(const char *file, int line, std::format_string<Args...> fmt, Args &&...args) {
        print(std::cerr, "WARN", Yellow, file, line, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args> static void error(const char *file, int line, std::format_string<Args...> fmt, Args &&...args) {
        print(std::cerr, "ERROR", Red, file, line, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args> static void debug(const char *file, int line, std::format_string<Args...> fmt, Args &&...args) {
        const char *debugEnabled = std::getenv(Constants::DEBUG_ENABLE_ENV_VARNAME.c_str());

        if (debugEnabled) {
            print(std::cerr, "DEBUG", Cyan, file, line, std::format(fmt, std::forward<Args>(args)...));
        }
    }

  private:
    static inline std::mutex logMutex;
    static void print(std::ostream &output, const char *level, const char *color, const char *file, int lineNo,
                      const std::string &message);

    static constexpr const char *Reset = "\033[0m";
    static constexpr const char *Bold = "\033[1m";
    static constexpr const char *Gray = "\033[90m";
    static constexpr const char *Red = "\033[31m";
    static constexpr const char *Yellow = "\033[33m";
    static constexpr const char *Green = "\033[32m";
    static constexpr const char *Cyan = "\033[36m";
};

#endif
