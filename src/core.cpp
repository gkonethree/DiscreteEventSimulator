#include <iostream>
#include "core.h"
#include "server.h"
#include "request.h"
#include "event.h"
#include "thread.h"

Core::Core(Server& server, int numThreads): server(server){
    for(int i = 0; i < numThreads; i++)
        threads.push_back(std::make_unique<Thread>(server));
}


ListOfEvents Core::scheduleRequestOnThread(Time time, std::shared_ptr<Request> request){
    ListOfEvents consequences;
    for(auto& thread: threads){
        if (!thread->isBusy()){
            return std::move(thread->processRequest(time, request));
        }
    }
    throw std::runtime_error("Trying to schedule on a core with all busy threads");
    return std::move(consequences);   
}

int Core::numThreadsFree(){
    return std::count_if(threads.begin(), threads.end(), 
                [](const std::unique_ptr<Thread>& thread){return !thread->isBusy();});
}