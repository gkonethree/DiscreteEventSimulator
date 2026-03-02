#include "user.h"
#include "request.h"


ListOfEvents User::receive(std::shared_ptr<RequestUnloadEvent> event){
    ListOfEvents consequences;
    std::shared_ptr<Request> request = std::make_shared<Request>(
        50, *this
    );
    std::shared_ptr<Event> new_event = std::make_shared<RequestLoadEvent>(
        event->time + thinkTime, *(this->outgoingLinks[0]), request
    );
    consequences.push_back(new_event);
    return consequences;
}