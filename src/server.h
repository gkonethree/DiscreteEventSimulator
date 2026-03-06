#pragma once

#include <vector>
#include <set>
#include <memory>
#include <unordered_map>
#include <random>
#include "event.h"
#include "link.h"

class Core;
class CoreComparator;
class Request;
class User;
class ServerServerLink;
class ServerUserLink;


class Server : public Linkable{
    private:
        std::vector<std::pair<const Server*, ServerServerLink*>>  serverLinks;
        std::unordered_map<const User*, ServerUserLink*>          userLinks;
        std::vector<double>                                       weights;
        std::discrete_distribution<int>                           dist;
        std::vector<std::unique_ptr<Core>>                        cores;
        std::queue<std::shared_ptr<Request>>                      requestQueue;
    public:
        Server(int numCores);
        void  addServerLink(Server*, ServerServerLink*, double weight);
        void  addUserLink(User*, ServerUserLink*);
        ListOfEvents receive(std::shared_ptr<RequestUnloadEvent>) override;
        ListOfEvents receive(std::shared_ptr<RequestCompletionEvent>);
    private:
        ListOfEvents loadRequestOnLink(Time, std::shared_ptr<Request>);
        ListOfEvents scheduleRequestOnCore(Time);
        Core*        scheduleCore();
        void         updateDist();
};