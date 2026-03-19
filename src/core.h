#pragma once

#include <queue>
#include <vector>
#include <memory>
#include <algorithm>
#include "event.h"

class Request;
class Thread;

class Core
{
    std::vector<std::unique_ptr<Thread>> threads;
    Thread *runningThread;
    Server &server;
    const Time timeSlice;
    const Time contextSwitchTime ;
    ListOfEvents scheduleThreadOnCore(Time);

public:
    Time lastIdleStart;
    bool isIdle;
    Time timeFree;
    int numThreadsFree();
    Core(Server &server, int numThreads, Time timeSlice, Time contextSwitchTime);
    ListOfEvents init(Time time);
    ListOfEvents giveRequestAThread(Time, std::shared_ptr<Request>);
    ListOfEvents completeThread(Time);
    ListOfEvents receive(std::shared_ptr<TimerInterruptEvent>);
    double getUtilization(Time totalTime) const;
};
