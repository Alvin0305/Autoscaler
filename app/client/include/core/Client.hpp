#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "core/LoadBalancer.hpp"
#include "lib/httplib.hpp"
#include "model/LoadState.hpp"
#include "tools/WorkGenerator.hpp"
#include <atomic>
#include <mutex>
#include <thread>

class Client {
  private:
    std::atomic<bool> running{false};
    std::mutex mtx;

    httplib::Server server;

    LoadState state{LoadState::LOW};
    LoadBalancer balancer;

    WorkGenerator workGenerator;

    std::thread workSenderThread;
    std::thread messageReceiverThread;

    void workSender();
    void messageReceiver();

    void cli();

    void increaseLoad();
    void decreaseLoad();

  public:
    Client() : balancer(), workGenerator() {};

    void start();
};

#endif
