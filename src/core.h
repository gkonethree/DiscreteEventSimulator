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
    Server& server;
    public:
        Core(Server& server, int numThreads);
        ListOfEvents scheduleRequestOnThread(Time, std::shared_ptr<Request>);
        int numThreadsFree();
};
