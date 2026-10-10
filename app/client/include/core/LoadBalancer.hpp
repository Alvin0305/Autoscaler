#ifndef LOAD_BALANCER_HPP
#define LOAD_BALANCER_HPP

#include "lib/httplib.hpp"
#include "model/Work.hpp"
#include <string>
#include <vector>

class LoadBalancer {
  private:
    int lastWorkedIndex{0};

    std::vector<std::string> ips;
    std::vector<httplib::Client> workerServers;

  public:
    LoadBalancer() = default;

    void balance(const Work &work);

    void addIP(const std::string &ip);
    void removeIP(const std::string &ip);

    void showIps() const;
};

#endif
