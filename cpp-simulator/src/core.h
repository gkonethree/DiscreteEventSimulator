#pragma once

#include <queue>
#include <vector>
#include <memory>
#include <algorithm>
#include "event.h"

class Request;
class Thread;

class Core{
    std::vector<std::unique_ptr<Thread>> threads;
    Thread* runningThread;
    Server& server;
    const Time timeSlice;
    
    ListOfEvents scheduleThreadOnCore(Time);
    
    public:
        int numThreadsFree();
        Core(Server& server, int numThreads, Time timeSlice);
        ListOfEvents init(Time time);
        ListOfEvents giveRequestAThread(Time, std::shared_ptr<Request>);
        ListOfEvents completeThread(Time);
        ListOfEvents receive(std::shared_ptr<TimerInterruptEvent>);
};
