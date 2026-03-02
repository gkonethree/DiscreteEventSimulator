#pragma once

#include <queue>
#include <vector>
#include <memory>
#include "event.h"

class Request;

class Core{
    std::queue<std::shared_ptr<Request>> requestQueue;
    const Server& server;
    public:
        Core(const Server& server): server(server) {}
        inline void addRequest(std::shared_ptr<Request> request){
            requestQueue.push(request);
        }
        inline int queueLength(){
            return requestQueue.size();
        }
        ListOfEvents receive(std::shared_ptr<RequestCompletionEvent>);
};

class CoreComparator{
    const std::vector<std::unique_ptr<Core>>& cores;
    public:
        CoreComparator(std::vector<std::unique_ptr<Core>> & cores): cores(cores){}
        inline bool operator()(int a, int b) const{
            return cores[a]->queueLength() < cores[b]->queueLength();
        }
};
