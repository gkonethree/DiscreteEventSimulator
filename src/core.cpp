#include <iostream>
#include "core.h"
#include "server.h"
#include "request.h"
#include "event.h"

void Core::stopProcessing(){
    busy = false;
    requestInService->beingProcessedAt = nullptr;
    requestInService.reset();
}

ListOfEvents Core::processRequest(Time time, std::shared_ptr<Request> request){
    ListOfEvents consequences;
    busy = true;
    requestInService = request;
    requestInService->beingProcessedAt = this;

    std::shared_ptr<Event> new_event = std::make_shared<RequestCompletionEvent>(
        time + request->processingTime, this->server, request
    );
    consequences.push_back(new_event);
    return std::move(consequences);   
}