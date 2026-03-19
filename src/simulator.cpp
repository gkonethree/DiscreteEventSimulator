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

Simulator::Simulator(Time maxTime, int numusers): maxTime(maxTime){
    metrics = new Metrics;
    for (int i = 0; i < numusers; i++){
        users.push_back(std::make_unique<User>(thinkMean, thinkVariance, requestProcessingTime, timeout, metrics));
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
        std::shared_ptr<Event> initial_event = std::make_shared<RequestLoadEvent>(
            0, *links[i*2], std::make_shared<Request>(requestProcessingTime, *users[i], 0)
        );
        events.push(initial_event);
    }
    for(int i = 0; i < numservers; i++){
        ListOfEvents initevents = servers[i]->init(0);
        for(auto event: initevents)
            events.push(event);
    }
}