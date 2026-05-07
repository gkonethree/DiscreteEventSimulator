#include <iostream>
#include "core.h"
#include "server.h"
#include "request.h"
#include "event.h"
#include "thread.h"


// Private

ListOfEvents Core::scheduleThreadOnCore(Time time){
    for(auto& thread: threads){
        if (thread->isBusy()){
            runningThread = thread.get();
            return thread->resumeProcessing(time);
        }
    }   
    runningThread = nullptr;
    return ListOfEvents();
}


// Public
Core::Core(Server& server, int numThreads, Time timeSlice): server(server), timeSlice(timeSlice), runningThread(nullptr) {
    for(int i = 0; i < numThreads; i++)
        threads.push_back(std::make_unique<Thread>(server, *this));
}


int Core::numThreadsFree(){
    return std::count_if(threads.begin(), threads.end(), 
                [](const std::unique_ptr<Thread>& thread){return !thread->isBusy();});
}


ListOfEvents Core::giveRequestAThread(Time time, std::shared_ptr<Request> request){

    for(auto& thread: threads){
        if (!thread->isBusy()){
            thread->assignRequest(request);
            return this->runningThread ? ListOfEvents() : scheduleThreadOnCore(time);
        }
    }
    throw std::runtime_error("Trying to schedule on a core with all busy threads");
}

ListOfEvents Core::init(Time time){
    return {std::make_shared<TimerInterruptEvent>(
        time, *this
    )};
}

ListOfEvents Core::receive(std::shared_ptr<TimerInterruptEvent> event){
    ListOfEvents consequences;
    
    if (runningThread)
        runningThread->yield(event->time);
    ListOfEvents schedulingEvents = this->scheduleThreadOnCore(event->time);
    consequences.insert(consequences.end(), schedulingEvents.begin(), schedulingEvents.end());

    consequences.push_back(std::make_shared<TimerInterruptEvent>(
        event->time + timeSlice, *this
    ));
    return consequences;
}


ListOfEvents Core::completeThread(Time time){
    this->runningThread->completeProcessing(time);
    return scheduleThreadOnCore(time);
}