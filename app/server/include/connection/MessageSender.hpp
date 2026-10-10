#ifndef MESSAGE_SENDER_HPP
#define MESSAGE_SENDER_HPP

#include "lib/httplib.hpp"
#include "model/Message.hpp"
#include <string>

class MessageSender {
  private:
    std::string host;
    unsigned port;
    httplib::Client client;

    const std::string buildMessageBody(const Message &message) const;

  public:
    explicit MessageSender(const std::string &host, unsigned port) : host(host), port(port), client(host, port) {};

    MessageSender(const MessageSender &other) = delete;
    MessageSender &operator=(const MessageSender &other) = delete;

    void send(const Message &message);
};

#endif
