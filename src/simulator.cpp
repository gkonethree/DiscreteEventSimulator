#include <iostream>
#include "simulator.h"
#include "server.h"
#include "user.h"
#include "link.h"
#include "request.h"


void Simulator::run(){
    while(!events.empty() && events.top()->time < this->maxTime){
        std::cout << "Time: " << events.top()->time << std::endl;
        std::shared_ptr<Event> event = events.top();
        events.pop();
        std::vector<std::shared_ptr<Event>> new_events = event->execute();
        for(auto new_event : new_events){
            events.push(new_event);
        }
    }
}

Simulator::Simulator(Time maxTime): maxTime(maxTime){
    users.push_back(std::make_unique<User>(1000));
    servers.push_back(std::make_unique<Server>(1));
    links.push_back(std::make_unique<Link>(*users[0], *servers[0]));
    links.push_back(std::make_unique<Link>(*servers[0], *users[0]));
    servers[0]->addIncomingLink(links[0].get());
    users[0]->addOutgoingLink(links[0].get());
    servers[0]->addOutgoingLink(links[1].get());
    users[0]->addIncomingLink(links[1].get());
    std::shared_ptr<Event> initial_event = std::make_shared<RequestLoadEvent>(
        0, *links[0], std::make_shared<Request>(50, *users[0])
    );
    events.push(initial_event);
}