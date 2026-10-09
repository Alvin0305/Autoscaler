#ifndef MESSAGE_SENDER_HPP
#define MESSAGE_SENDER_HPP

#include "Message.hpp"
#include "Socket.hpp"
#include <string>

class MessageSender {
  private:
    std::string host;
    unsigned port;
    Socket socket;

    const std::string buildMessageBody(const Message &message) const;
    const std::string buildMessage(const std::string &body) const;

  public:
    explicit MessageSender(const std::string &host, unsigned port) : host(host), port(port), socket(host, port) {};

    void send(const Message &message) const;
};

#endif

/*
 * {
 *  "event": "vm_add",
 *  "ip": "192.168.122.222"
 * }
 */
