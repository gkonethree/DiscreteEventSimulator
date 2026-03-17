#pragma once

#include "event.h"
#include "metrics.h"
class User;
class Server;
class Link;

class Simulator{
    private:
        EventQueue events;
        Time maxTime;
        std::vector<std::unique_ptr<Server>> servers;
        std::vector<std::unique_ptr<User>> users;
        std::vector<std::unique_ptr<Link>> links;
    public:
        Metrics* metrics;   
        Time lastTime;
        Simulator(Time maxTime);
        void run();
};