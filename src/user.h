#pragma once


#include <memory>
#include <random>
#include "event.h"
#include "link.h"

class UserServerLink;


// TODO: Make better design to not allow user to exist without a userserverlink;

// TODO: Where to put service time? In server or user?

class User: public Linkable{   
    UserServerLink*                  serverLink;
    const Time                       requestProcessingTime;
    std::normal_distribution<Time>   thinkTime;
    const Time                       timeout;
    
    public:
        User(const Time thinkMean, const Time thinkVariance, const Time requestProcessingTime, const Time timeout) : 
            requestProcessingTime(requestProcessingTime), 
            serverLink(nullptr), 
            thinkTime(thinkMean, thinkVariance),
            timeout(timeout)
        {}
        void addServerLink(UserServerLink*);
        ListOfEvents receive(std::shared_ptr<RequestUnloadEvent> event) override;
};
