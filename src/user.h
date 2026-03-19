#pragma once


#include <memory>
#include <random>
#include "event.h"
#include "link.h"
#include "metrics.h"

class UserServerLink;


// TODO: Make better design to not allow user to exist without a userserverlink;

// TODO: Where to put service time? In server or user?


enum class Distribution{
    Exponential,
    Constant,
    Uniform
};

class User: public Linkable{   
    UserServerLink*                      serverLink;
    std::uniform_real_distribution<Time> uniformServiceTime;
    std::exponential_distribution<Time>  expServiceTime;
    Time                                 constantServiceTime;
    Distribution                         distribution;
    std::normal_distribution<Time>       thinkTime;
    const Time                           timeout;
    Metrics*                             metrics;

    private:
        Time generateServiceTime();
        Time generateThinkTime();
    
    public:
        User(const Time thinkMean, const Time thinkVariance, std::exponential_distribution<Time> expServiceTime, const Time timeout, Metrics* metrics) : 
            expServiceTime(expServiceTime), 
            serverLink(nullptr), 
            thinkTime(thinkMean, thinkVariance),
            timeout(timeout),
            metrics(metrics)
        {
            distribution = Distribution::Exponential;
        }
        User(const Time thinkMean, const Time thinkVariance, std::uniform_real_distribution<Time> uniformServiceTime, const Time timeout, Metrics* metrics) : 
            uniformServiceTime(uniformServiceTime),   
            serverLink(nullptr), 
            thinkTime(thinkMean, thinkVariance),
            timeout(timeout),
            metrics(metrics)
        {
            distribution = Distribution::Uniform;
        }
        User(const Time thinkMean, const Time thinkVariance, const Time constantServiceTime, const Time timeout, Metrics* metrics) : 
            constantServiceTime(constantServiceTime), 
            serverLink(nullptr), 
            thinkTime(thinkMean, thinkVariance),
            timeout(timeout),
            metrics(metrics)
        {
            distribution = Distribution::Constant;
        }
        void addServerLink(UserServerLink*);
        ListOfEvents receive(std::shared_ptr<RequestUnloadEvent> event) override;
        ListOfEvents init(Time);
};
