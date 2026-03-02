#include "server.h"
#include "request.h"


ListOfEvents Server::receive(std::shared_ptr<RequestUnloadEvent> event) {
    ListOfEvents consequences;
    if (requestQueue.empty()){
        std::shared_ptr<Event> new_event = std::make_shared<RequestCompletionEvent>(
            event->time + event->request->processingTime, *this, event->request
        );
        consequences.push_back(new_event);
    }
    requestQueue.push(event->request);
    return consequences;
}

ListOfEvents Server::receive(std::shared_ptr<RequestCompletionEvent> event) {
    ListOfEvents consequences;
    requestQueue.pop();
    std::shared_ptr<Event> load_event = std::make_shared<RequestLoadEvent>(
        event->time, *(this->outgoingLinks[0]), event->request
    );
    consequences.push_back(load_event);
    if (requestQueue.empty()){
        return consequences;
    }
    std::shared_ptr<Request> request = requestQueue.front();
    std::shared_ptr<Event> new_event = std::make_shared<RequestCompletionEvent>(
        event->time + request->processingTime, *this, event->request
    );
    consequences.push_back(new_event);
    return consequences;
}