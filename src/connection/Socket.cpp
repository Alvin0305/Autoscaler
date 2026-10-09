#include "connection/Socket.hpp"
#include "logger/Logger.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

Socket::Socket(const std::string &host, unsigned int port) : host(host), port(port) {
    connect();
}

Socket::~Socket() {
    if (FD != -1) {
        close(FD);
    }
}

Socket::Socket(Socket &&other) noexcept : host(other.host), port(other.port), FD(other.FD) {
    other.port = -1;
    other.host = "";
    other.FD = -1;
}

Socket &Socket::operator=(Socket &&other) noexcept {
    if (this != &other) {
        if (FD != -1) {
            close(FD);
            FD = -1;
        }

        port = other.port;
        host = other.host;
        FD = other.FD;
        other.port = -1;
        other.host = "";
        other.FD = -1;
    }

    return *this;
}

void Socket::connect() {
    if ((FD = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        throw std::runtime_error("Failed to create socket");
    }

    sockaddr_in serverAddress;
    std::memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_port = htons(port);
    serverAddress.sin_family = AF_INET;

    if (inet_pton(AF_INET, host.c_str(), &serverAddress.sin_addr) <= 0) {
        throw std::runtime_error("inet_pton failed");
    }

    LOG_INFO("Connecting to {}:{}", host, port);
    if (::connect(FD, reinterpret_cast<sockaddr *>(&serverAddress), sizeof(serverAddress)) < 0) {
        throw std::runtime_error("connect failed");
    }
}

void Socket::send(const std::string &bytes) const {
    if (::send(FD, bytes.c_str(), bytes.size(), 0) < 0) {
        throw std::runtime_error("send failed");
    }
}
