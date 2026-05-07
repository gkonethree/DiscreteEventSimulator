#include "thread.h"
#include "event.h"
#include "request.h"
#include "core.h"


void Thread::completeProcessing(Time time){
    this->busy = false;
    this->requestInService->beingProcessedAt = nullptr;
    this->requestInService.reset();
    this->completionEvent.reset();
    this->remainingTime = 0;
    this->lastScheduled = 0;
}

void Thread::yield(Time time){
    this->completionEvent->outdated = true;
    this->completionEvent.reset();
    this->remainingTime -= time - this->lastScheduled;
}

void Thread::assignRequest(std::shared_ptr<Request> request){
    if (this->requestInService){
        throw std::runtime_error("Invalid argument");
    }
    this->requestInService = request;
    this->remainingTime = request->processingTime;
    this->busy = true;
    this->requestInService->beingProcessedAt = this;
}


ListOfEvents Thread::resumeProcessing(Time time){
    ListOfEvents consequences;
    this->lastScheduled = time;
    this->completionEvent = std::make_shared<RequestCompletionEvent>(
        time + this->remainingTime, this->server, this->requestInService
    );
    consequences.push_back(this->completionEvent);
    return consequences;  
}