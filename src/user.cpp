#include "user.h"
#include "request.h"

extern
std::mt19937 gen;


ListOfEvents User::receive(std::shared_ptr<RequestUnloadEvent> event) {
    Time thinkTime = std::max((Time)0, (Time)this->thinkTime(gen));
    Time responseTime=event->time + thinkTime - event->request->startTime;
    if( responseTime>= timeout){
        metrics->insertBadEvent(responseTime);
    }
    else{
        metrics->insertGoodEvent(responseTime);
    }
    Time rSysTime = event->time - event->request->startTime;
    metrics->insertRsysEvent(rSysTime);

    ListOfEvents consequences;
    
    std::shared_ptr<Request> request = std::make_shared<Request>(
        requestProcessingTime, *this, event->time + thinkTime
    );
    std::shared_ptr<Event> new_event = std::make_shared<RequestLoadEvent>(
        event->time +thinkTime, *(this->serverLink), request
    );
    consequences.push_back(new_event);
    return std::move(consequences);
}


void User::addServerLink(UserServerLink* link){
    if (this->serverLink)
        throw std::invalid_argument("UserServerLink can only be set once");
    this->serverLink = link;
}