#pragma once

#include <vector>
#include <queue>
#include <memory>
#include "event.h"
#include "link.h"
#include "core.h"

class Core;
class CoreComparator;

class Server : public Linkable{
    private:
        std::vector<std::unique_ptr<Core>> cores;
        std::unique_ptr<CoreComparator> coreScheduler;
        std::priority_queue<int, std::vector<int>, CoreComparator> coreQueue;
    public:
        Server(int numCores);
        ListOfEvents receive(std::shared_ptr<RequestUnloadEvent>);
        ListOfEvents loadRequest(Time, std::shared_ptr<Request>) const;
};