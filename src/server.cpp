#include <stdexcept>
#include <random>
#include "server.h"
#include "request.h"
#include "core.h"
#include "thread.h"

extern 
std::mt19937 gen;


Server::Server(int numCores): 
            cores(std::vector<std::unique_ptr<Core>>(numCores))
{
    if (numCores <= 0)
        throw std::runtime_error("Number of cores should be > 0 for a server");
    for (int i = 0; i < numCores; i++){
        cores[i] = std::make_unique<Core>(*this, 2);
    }
}

ListOfEvents Server::receive(std::shared_ptr<RequestCompletionEvent> event){
    ListOfEvents consequences;
    Thread* thread = event->request->beingProcessedAt;
    thread->stopProcessing();

    ListOfEvents scheduleEvents = scheduleRequestOnCore(event->time);
    ListOfEvents loadEvents = loadRequestOnLink(event->time, event->request);

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
        ListOfEvents scheduledEvents = core->scheduleRequestOnThread(time, request);
        requestQueue.pop();
        consequences.insert(consequences.end(), scheduledEvents.begin(), scheduledEvents.end());
    }   
    return std::move(consequences);

}

ListOfEvents Server::receive(std::shared_ptr<RequestUnloadEvent> event) {
    requestQueue.push(event->request);
    return std::move(scheduleRequestOnCore(event->time));
}

ListOfEvents Server::loadRequestOnLink(Time time, std::shared_ptr<Request> request){
    ListOfEvents consequences;

    int chosen = dist(gen);
    if (chosen == serverLinks.size()){
        consequences.push_back(std::make_shared<RequestLoadEvent>(
            time, *(this->userLinks[&request->user]), request
        ));
    }
    else{
        consequences.push_back(std::make_shared<RequestLoadEvent>(
            time, *(this->serverLinks[chosen].second), request
        ));  
    }
     
    return std::move(consequences);
}

Core* Server::scheduleCore(){
    Core* core = std::min_element(cores.begin(), cores.end(), 
        [](const std::unique_ptr<Core>& a, const std::unique_ptr<Core>& b){
            return a->numThreadsFree() > b->numThreadsFree();
        }
    )->get();
    return core->numThreadsFree() ? core : nullptr;
}


void Server::addServerLink(Server* server, ServerServerLink* link, double weight){
    weights.push_back(weight);
    serverLinks.emplace_back(server, link);
    updateDist();
}

void Server::addUserLink(User* user, ServerUserLink* link){
    userLinks[user] = link;
}

// Throws exception
void Server::updateDist(){
    double sum = std::accumulate(weights.begin(), weights.end(), 0.0);
    if (sum > 1.0){
        throw std::invalid_argument("Sum of weights is greater than 1...aborting");
    }
    std::vector<double> newWeights(weights.begin(), weights.end());
    if (userLinks.size())
        newWeights.push_back(1 - sum);
    std::discrete_distribution<int>::param_type params(newWeights.begin(), newWeights.end());
    dist.param(params);
}