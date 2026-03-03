#include "thread.h"
#include "event.h"
#include "request.h"
#include "core.h"


void Thread::stopProcessing(){
    busy = false;
    requestInService->beingProcessedAt = nullptr;
    requestInService.reset();
}

ListOfEvents Thread::processRequest(Time time, std::shared_ptr<Request> request){
    ListOfEvents consequences;
    this->busy = true;
    this->requestInService = request;
    this->requestInService->beingProcessedAt = this;

    std::shared_ptr<Event> new_event = std::make_shared<RequestCompletionEvent>(
        time + request->processingTime, this->server, request
    );
    consequences.push_back(new_event);
    return std::move(consequences);   
}