#include "user.h"
#include "request.h"

extern
std::mt19937 gen;


ListOfEvents User::receive(std::shared_ptr<RequestUnloadEvent> event) {

    //ise acche se likhna hai
    if(event->request->endTime - event->request->startTime >= timeout){
        //do timeout
    }
    //add other metric capture

    ListOfEvents consequences;
    std::shared_ptr<Request> request = std::make_shared<Request>(
        requestProcessingTime, *this, event->time
    );
    std::shared_ptr<Event> new_event = std::make_shared<RequestLoadEvent>(
        event->time + thinkTime(gen), *(this->serverLink), request
    );
    consequences.push_back(new_event);
    return std::move(consequences);
}


void User::addServerLink(UserServerLink* link){
    if (this->serverLink)
        throw std::invalid_argument("UserServerLink can only be set once");
    this->serverLink = link;
}