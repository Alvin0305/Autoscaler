#include "core/LoadBalancer.hpp"
#include "Constants.hpp"
#include "lib/httplib.hpp"
#include "logger/Logger.hpp"
#include <algorithm>
#include <string>

void LoadBalancer::addIP(const std::string &ip) {
    ips.push_back(ip);

    LOG_INFO("New Connection establishing with worker VM {}:{}", ip, ClientConstants::WORKER_SERVER_PORT);

    httplib::Client workerServer(ip, ClientConstants::WORKER_SERVER_PORT);
    workerServers.push_back(std::move(workerServer));
}

void LoadBalancer::removeIP(const std::string &ip) {
    const auto ipIt = std::find(ips.begin(), ips.end(), ip);
    if (ipIt != ips.end()) {
        ips.erase(ipIt);
    }

    const auto socketIt = std::find_if(workerServers.begin(), workerServers.end(),
                                       [ip](const httplib::Client &client) { return client.host() == ip; });
    if (socketIt != workerServers.end()) {
        workerServers.erase(socketIt);
    }
}

void LoadBalancer::showIps() const {
    LOG_INFO("IPs: ");
    for (const auto &ip : ips) {
        LOG_INFO("\t- {}", ip);
    }
}

void LoadBalancer::balance(const Work &work) {
    if (ips.empty()) {
        LOG_WARN("There is no server to do load balancing");
        return;
    }

    lastWorkedIndex = (lastWorkedIndex + 1) % ips.size();
    LOG_INFO("Sending work {} to {}th Server: {}", work, lastWorkedIndex, ips[lastWorkedIndex]);

    httplib::Client &workerServer = workerServers[lastWorkedIndex];
    auto response = workerServer.Post("/work", work.serialize(), "application/json");

    if (not response) {
        auto error = response.error();
        LOG_ERROR("Request failed: {}", httplib::to_string(error));
        return;
    }

    LOG_INFO("HTTP Status: {}", response->status);
    LOG_INFO("Response body: {}", response->body);
    LOG_INFO("Content-Type: {}", response->get_header_value("Content-Type"));
}
