#include <iostream>
#include "simulator.h"
#include "server.h"
#include "user.h"
#include "link.h"
#include "request.h"
#include "thread.h"
#include "core.h"
#include "params.h"

void Simulator::run(){
    while(!events.empty() && events.top()->time < this->maxTime){
        std::shared_ptr<Event> event = events.top();
        events.pop();
        if (event->outdated){
            lastTime=event->time;
            continue;
        }
            
        //std::cout << "Time: " << " " << event->time << std::endl;
        
        std::vector<std::shared_ptr<Event>> new_events = event->execute();
        for(auto new_event : new_events){
            events.push(new_event);
        }
        if(events.empty()){
            lastTime=event->time;
        }
        if(events.top()->time >= this->maxTime){
            lastTime=this->maxTime;
        }
    }
    
    for (size_t i = 0; i < servers.size(); i++) {
        double avgUtil = servers[i]->getAverageCoreUtilization(lastTime);
        auto coreUtils = servers[i]->getCoreUtilizations(lastTime);
        metrics->insertCoreUtilization(i, avgUtil, coreUtils);
        metrics->insertNumDroppedReq(servers[i]->numRequestsDropped);
    }
}

Simulator::Simulator(Time maxTime, Distribution serviceTimeDistribution, int numusers): maxTime(maxTime){
    metrics = new Metrics;
    if (serviceTimeDistribution == Distribution::Constant){
        for (int i = 0; i < numusers; i++){
            users.push_back(std::make_unique<User>(thinkMean, thinkVariance, constantServiceTime, timeout, metrics));
        }
    }
    else if (serviceTimeDistribution == Distribution::Exponential){
        std::exponential_distribution<Time> exp(expServiceTimeMean);
        for (int i = 0; i < numusers; i++){
            users.push_back(std::make_unique<User>(thinkMean, thinkVariance, exp, timeout, metrics));
        }
    }
    else{
        std::uniform_real_distribution<Time> uniform_dist(uniformLow, uniformHigh);
        for (int i = 0; i < numusers; i++){
            users.push_back(std::make_unique<User>(thinkMean, thinkVariance, uniform_dist, timeout, metrics));
        }
    }
    for(int i = 0; i < numservers; i++){
        servers.push_back(std::make_unique<Server>(numCores, numThreadsPerCore, timeSlice, contextSwitchTime, bufferSize));
    }

    
    for(int i = 0; i < numusers; i++){
        links.push_back(std::make_unique<UserServerLink>(*users[i], *servers[0],propDelay));       
        links.push_back(std::make_unique<ServerUserLink>(*servers[0], *users[i],propDelay));        
    }


    // links.push_back(std::make_unique<ServerServerLink>(*servers[0], *servers[1], 0.5));
    
    //assuming init time is zero, ek baar confirm kar lena
    for(int i = 0; i < numusers; i++){
        ListOfEvents initial_events = users[i]->init(0);
        for(auto event: initial_events)
            events.push(initial_event);
    }
    for(int i = 0; i < numservers; i++){
        ListOfEvents initevents = servers[i]->init(0);
        for(auto event: initevents)
            events.push(event);
    }
}