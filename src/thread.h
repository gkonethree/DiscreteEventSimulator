#pragma once

#include <memory>
#include "event.h"

class Core;
class Request;

class Thread{
    std::shared_ptr<Request> requestInService;
    std::shared_ptr<RequestCompletionEvent> completionEvent;
    Time remainingTime;
    Time lastScheduled;
    bool busy;
    
    public:
        Server& server;
        Core& core;
        
        Thread(Server& server, Core& core): server(server), core(core) {}
        inline bool isBusy() const {return busy;}
        
        void completeProcessing(Time);
        void yield(Time);
        void assignRequest(std::shared_ptr<Request>);
        ListOfEvents resumeProcessing(Time);

};