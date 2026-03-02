#include <stdexcept>
#include "server.h"
#include "request.h"
#include "core.h"


Server::Server(int numCores): 
    cores(std::vector<std::unique_ptr<Core>>(numCores)),
    coreScheduler(std::make_unique<CoreComparator>(cores)),
    coreQueue(std::priority_queue<int, std::vector<int>, CoreComparator>(*coreScheduler))
    {
    if (numCores <= 0)
        throw std::runtime_error("Number of cores should be > 0 for a server");
    for (int i = 0; i < numCores; i++){
        cores[i] = std::make_unique<Core>(*this);
    }
    for(int i = 0; i < numCores; i++){
        coreQueue.push(i);
    }
}


ListOfEvents Server::receive(std::shared_ptr<RequestUnloadEvent> event) {
    ListOfEvents consequences;
    const std::unique_ptr<Core>& core = cores[coreQueue.top()];
    coreQueue.pop();
    if (core->queueLength() == 0){
        std::shared_ptr<Event> new_event = std::make_shared<RequestCompletionEvent>(
            event->time + event->request->processingTime, *core, event->request
        );
        consequences.push_back(new_event);
    }
    core->addRequest(event->request);
    return consequences;
}

ListOfEvents Server::loadRequest(Time time, std::shared_ptr<Request> request) const {
    ListOfEvents consequences;
    consequences.push_back(std::make_shared<RequestLoadEvent>(
        time, *(this->outgoingLinks[0]), request
    ));   
    return consequences;
}