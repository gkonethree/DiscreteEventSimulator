#include "user.h"
#include "request.h"

extern
std::mt19937 gen;


ListOfEvents User::receive(std::shared_ptr<RequestUnloadEvent> event) {
    
    Time serviceTime = generateServiceTime();
    Time thinkTime = generateThinkTime();

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
        serviceTime, *this, event->time + thinkTime
    );
    std::shared_ptr<Event> new_event = std::make_shared<RequestLoadEvent>(
        event->time +thinkTime, *(this->serverLink), request
    );
    consequences.push_back(new_event);
    return std::move(consequences);
}

Time User::generateServiceTime(){
    Time serviceTime;
    if (distribution == Distribution::Constant){
        serviceTime = constantServiceTime;
    }
    else if (distribution == Distribution::Exponential){
        serviceTime = std::max((Time) 0, (Time) expServiceTime(gen));
    }
    else{
        serviceTime = std::max((Time) 0, uniformServiceTime(gen));
    }
    return serviceTime;
}


Time User::generateThinkTime(){
    return std::max((Time)0, (Time)this->thinkTime(gen));
}

ListOfEvents User::init(Time time) {

    ListOfEvents consequences;
    Time serviceTime = generateServiceTime();
    Time thinkTime = generateThinkTime();

    std::shared_ptr<Request> request = std::make_shared<Request>(
        serviceTime, *this, time
    );
    std::shared_ptr<Event> new_event = std::make_shared<RequestLoadEvent>(
        time +thinkTime, *(this->serverLink), request
    );
    consequences.push_back(new_event);
    return std::move(consequences);
}


void User::addServerLink(UserServerLink* link){
    if (this->serverLink)
        throw std::invalid_argument("UserServerLink can only be set once");
    this->serverLink = link;
}