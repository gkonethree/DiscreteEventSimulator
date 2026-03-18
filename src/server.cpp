#include <stdexcept>
#include <random>
#include "server.h"
#include "request.h"
#include "core.h"
#include "thread.h"

extern 
std::mt19937 gen;


Server::Server(int numCores, int numThreadsPerCore, Time timeSlice): 
            cores(std::vector<std::unique_ptr<Core>>(numCores))
{
    numRequestsDropped = 0;
    if (numCores <= 0)
        throw std::runtime_error("Number of cores should be > 0 for a server");
    for (int i = 0; i < numCores; i++){
        cores[i] = std::make_unique<Core>(*this, numThreadsPerCore, timeSlice);
    }
}

ListOfEvents Server::receive(std::shared_ptr<RequestCompletionEvent> event){
    ListOfEvents consequences;

    ListOfEvents completionEvents = event->request->beingProcessedAt->core.completeThread(event->time);
    ListOfEvents scheduleEvents = this->assignRequestToCore(event->time);
    ListOfEvents loadEvents = loadRequestOnLink(event->time, event->request);

    consequences.insert(consequences.end(), completionEvents.begin(), completionEvents.end());
    consequences.insert(consequences.end(), scheduleEvents.begin(), scheduleEvents.end());
    consequences.insert(consequences.end(), loadEvents.begin(), loadEvents.end());

    return consequences;
}

ListOfEvents Server::assignRequestToCore(Time time){
    ListOfEvents consequences;
    Core* core = scheduleCore();
    if (requestQueue.empty())
        return consequences;

    std::shared_ptr<Request> request = requestQueue.front();

    if (core){
        ListOfEvents scheduledEvents = core->giveRequestAThread(time, request);
        requestQueue.pop();
        consequences.insert(consequences.end(), scheduledEvents.begin(), scheduledEvents.end());
    }   
    return consequences;

}

ListOfEvents Server::receive(std::shared_ptr<RequestUnloadEvent> event) {
    this->requestQueue.push(event->request);
    return this->assignRequestToCore(event->time);
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
     
    return consequences;
}

Core* Server::scheduleCore(){
    Core* core = std::min_element(cores.begin(), cores.end(), 
        [](const std::unique_ptr<Core>& a, const std::unique_ptr<Core>& b){
            return a->numThreadsFree() > b->numThreadsFree();
        }
    )->get();
    if(core->numThreadsFree() == 0)
        numRequestsDropped++;
    return core->numThreadsFree() ? core : nullptr;
}

ListOfEvents Server::init(Time time){
    ListOfEvents initEvents;
    for(auto& core: cores){
        ListOfEvents coreInit = core->init(time);
        initEvents.insert(initEvents.end(), coreInit.begin(), coreInit.end());
    }
    return initEvents;
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

double Server::getAverageCoreUtilization(Time totalTime) const {
    if (cores.empty()) return 0.0;
    double sum = 0.0;
    for (const auto& core : cores) {
        sum += core->getUtilization(totalTime);
    }
    return sum / cores.size();
}

std::vector<double> Server::getCoreUtilizations(Time totalTime) const {
    std::vector<double> utilizations;
    for (const auto& core : cores) {
        utilizations.push_back(core->getUtilization(totalTime));
    }
    return utilizations;
}