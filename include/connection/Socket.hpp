#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string>
class Socket {
  private:
    std::string host;
    unsigned int port;
    int FD;

    void connect();

  public:
    Socket(const std::string &host, unsigned int port);
    ~Socket();

    Socket(const Socket &other) = delete;
    Socket &operator=(const Socket &other) = delete;

    Socket(Socket &&other) noexcept;
    Socket &operator=(Socket &&other) noexcept;

    void send(const std::string &bytes) const;
};

#endif
