#pragma once

#include <vector>
#include <set>
#include <memory>
#include "event.h"
#include "link.h"
#include "core.h"

class Core;
class CoreComparator;
class Request;

class Server : public Linkable{
    private:
        // Dont dereference vector elements by pointers, the elements may get reallocated
        std::vector<std::unique_ptr<Core>> cores;
        std::queue<std::shared_ptr<Request>> requestQueue;
    public:
        Server(int numCores);
        ListOfEvents receive(std::shared_ptr<RequestUnloadEvent>);
        ListOfEvents receive(std::shared_ptr<RequestCompletionEvent>);
    private:
        ListOfEvents loadRequest(Time, std::shared_ptr<Request>);
        ListOfEvents scheduleRequestOnCore(Time);
        void changePriority(Core*);
        Core* scheduleCore();
};