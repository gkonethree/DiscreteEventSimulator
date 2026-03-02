#include <iostream>
#include "core.h"
#include "server.h"
#include "request.h"
#include "event.h"

ListOfEvents Core::receive(std::shared_ptr<RequestCompletionEvent> event){
    ListOfEvents consequences;
    std::shared_ptr<Request> request = requestQueue.front();
    requestQueue.pop();
    ListOfEvents loadEvents = server.loadRequest(event->time, request);
    consequences.insert(consequences.end(), loadEvents.begin(), loadEvents.end());

    if (requestQueue.empty()){
        return consequences;
    }

    std::shared_ptr<Request> next_request = requestQueue.front();
    std::shared_ptr<Event> next_event = std::make_shared<RequestCompletionEvent>(
        event->time + next_request->processingTime, *this, next_request
    );
    consequences.push_back(next_event);
    return consequences;   
}