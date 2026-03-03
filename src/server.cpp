#include <stdexcept>
#include "server.h"
#include "request.h"
#include "core.h"


Server::Server(int numCores): 
    cores(std::vector<std::unique_ptr<Core>>(numCores))
    {
    if (numCores <= 0)
        throw std::runtime_error("Number of cores should be > 0 for a server");
    for (int i = 0; i < numCores; i++){
        cores[i] = std::make_unique<Core>(*this);
    }
}

ListOfEvents Server::receive(std::shared_ptr<RequestCompletionEvent> event){
    ListOfEvents consequences;
    Core* core = event->request->beingProcessedAt;
    core->stopProcessing();

    ListOfEvents scheduleEvents = scheduleRequestOnCore(event->time);
    ListOfEvents loadEvents = loadRequest(event->time, event->request);

    consequences.insert(consequences.begin(), scheduleEvents.begin(), scheduleEvents.end());
    consequences.insert(consequences.begin(), loadEvents.begin(), loadEvents.end());
    return std::move(consequences);
}

ListOfEvents Server::scheduleRequestOnCore(Time time){
    ListOfEvents consequences;
    Core* core = scheduleCore();
    if (requestQueue.empty())
        return std::move(consequences);

    std::shared_ptr<Request> request = requestQueue.front();

    if (core){
        ListOfEvents scheduledEvents = core->processRequest(time, request);
        requestQueue.pop();
        consequences.insert(consequences.end(), scheduledEvents.begin(), scheduledEvents.end());
    }   
    return std::move(consequences);

}

ListOfEvents Server::receive(std::shared_ptr<RequestUnloadEvent> event) {
    requestQueue.push(event->request);
    return std::move(scheduleRequestOnCore(event->time));
}

ListOfEvents Server::loadRequest(Time time, std::shared_ptr<Request> request){
    ListOfEvents consequences;
    consequences.push_back(std::make_shared<RequestLoadEvent>(
        time, *(this->outgoingLinks[&(request->user)]), request
    ));   
    return std::move(consequences);
}

Core* Server::scheduleCore(){
    for(auto& core: cores){
        if (!core->isBusy())
            return core.get();
    }
    return nullptr;
}