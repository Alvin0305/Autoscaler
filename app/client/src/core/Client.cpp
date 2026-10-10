#include "core/Client.hpp"
#include "Constants.hpp"
#include "lib/httplib.hpp"
#include "logger/Logger.hpp"
#include "model/LoadState.hpp"
#include "model/Message.hpp"
#include "model/Work.hpp"
#include "tools/WorkGenerator.hpp"
#include <chrono>
#include <iostream>
#include <mutex>
#include <ostream>
#include <string>
#include <thread>

void Client::decreaseLoad() {
    LOG_INFO("State changed to LOW");
    state = LoadState::LOW;
}

void Client::increaseLoad() {
    LOG_INFO("State changed to HIGH");
    state = LoadState::HIGH;
}

void Client::workSender() {
    while (running) {
        Work work = workGenerator.generate(state);

        switch (state) {
            case LoadState::LOW:
                std::this_thread::sleep_for(std::chrono::milliseconds(ClientConstants::WORK_INTERVAL_IN_LOW_LOAD));
                break;
            case LoadState::HIGH:
                std::this_thread::sleep_for(std::chrono::milliseconds(ClientConstants::WORK_INTERVAL_IN_HIGH_LOAD));
                break;
        }

        balancer.balance(work);
    }
}

void Client::messageReceiver() {
    server.Post("/scale", [&](const httplib::Request &req, httplib::Response &res) {
        LOG_INFO("Received message : {} from autoscaler", req.body);
        Message message = Message::fromString(req.body);
        std::string response = "{ message: did nothing }";
        LOG_INFO("Parsed message: {}", message);

        switch (message.action) {
            case Message::Action::VM_ADDED:
                LOG_INFO("VM {} with IP {} added", message.name, message.ip);
                balancer.addIP(message.ip);
                response = "{ message: IP added }";
                break;
            case Message::Action::VM_DESTROYED:
                LOG_INFO("VM {} with IP {} removed", message.name, message.ip);
                balancer.removeIP(message.ip);
                response = "{ message: IP removed }";
                break;
            case Message::Action::NONE:
                LOG_WARN("Received None Action");
                break;
            default:
                LOG_ERROR("Unhandled Message Action");
                break;
        }

        res.set_content(response, "application/json");
    });

    LOG_INFO("Server starting on port {}...", ClientConstants::PORT);
    if (not server.listen("0.0.0.0", ClientConstants::PORT)) {
        LOG_ERROR("Failed to start server!");
        return;
    }
}

void Client::cli() {
    while (running) {
        std::cout << "Enter any of the following options" << std::endl;
        std::cout << "1. increase (i)" << std::endl;
        std::cout << "2. decrease (d)" << std::endl;
        std::cout << "3. exit (e)" << std::endl;

        std::string input;
        std::cin >> input;

        if (input == "increase" or input == "i") {
            increaseLoad();
        } else if (input == "decrease" or input == "d") {
            decreaseLoad();
        } else if (input == "exit" or input == "e") {
            std::cout << "Exiting Autoscaler client" << std::endl;
            {
                std::lock_guard<std::mutex> guard(mtx);
                server.stop();
            }
            running = false;
        } else {
            std::cout << "Invalid command" << std::endl;
        }
    }
}

void Client::start() {
    running = true;

    workSenderThread = std::thread(&Client::workSender, this);
    messageReceiverThread = std::thread(&Client::messageReceiver, this);

    cli();

    workSenderThread.join();
    messageReceiverThread.join();
}
