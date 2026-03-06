#pragma once

#include <memory>
#include "event.h"

class Core;
class Request;

class Thread{
    std::shared_ptr<Request> requestInService;
    bool busy;
    Server& server;

    public:
        Thread(Server& server): server(server){}
        void stopProcessing();
        ListOfEvents processRequest(Time, std::shared_ptr<Request>);
        inline bool isBusy() const{
            return busy;
        }
};