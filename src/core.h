#pragma once

#include <queue>
#include <vector>
#include <memory>
#include "event.h"

class Request;

class Core{
    bool busy;
    std::shared_ptr<Request> requestInService;
    Server& server;
    public:
        Core(Server& server): server(server), busy(false) {}
        inline bool isBusy(){
            return busy;
        }
        ListOfEvents processRequest(Time, std::shared_ptr<Request>);
        void stopProcessing();
};

class CoreComparator{
    public:
        inline bool operator()(Core* a, Core* b) const{
            return a->isBusy();
        }
};
