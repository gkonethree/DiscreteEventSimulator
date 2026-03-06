#include <iostream>
#include "simulator.h"
#include "server.h"
#include "user.h"
#include "link.h"
#include "request.h"
#include "thread.h"
#include "core.h"


void Simulator::run(){
    while(!events.empty() && events.top()->time < this->maxTime){
        std::shared_ptr<Event> event = events.top();
        std::cout << "Time: " << " " << event->time << std::endl;
        events.pop();
        std::vector<std::shared_ptr<Event>> new_events = event->execute();
        for(auto new_event : new_events){
            events.push(new_event);
        }
    }
}

Simulator::Simulator(Time maxTime): maxTime(maxTime){
    for (int i = 0; i < 1; i++){
        users.push_back(std::make_unique<User>(1000, 200, 50));
    }
    for(int i = 0; i < 2; i++){
        servers.push_back(std::make_unique<Server>(5));
    }

    
    for(int i = 0; i < 1; i++){
        links.push_back(std::make_unique<UserServerLink>(*users[i], *servers[0]));
        links.push_back(std::make_unique<ServerUserLink>(*servers[1], *users[i]));        
        links.push_back(std::make_unique<ServerUserLink>(*servers[0], *users[i]));        
    }
    links.push_back(std::make_unique<ServerServerLink>(*servers[0], *servers[1], 0.5));
    
    for(int i = 0; i < 1; i++){
        std::shared_ptr<Event> initial_event = std::make_shared<RequestLoadEvent>(
            0, *links[i*3], std::make_shared<Request>(50, *users[i])
        );
        events.push(initial_event);
    }
}