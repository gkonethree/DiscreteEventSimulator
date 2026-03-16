#pragma once

#include <memory>
#include "user.h"

class Thread;

class Request{
    public:
        const Time processingTime;
        const User& user;
        Thread* beingProcessedAt;
        Time startTime;
        Time endTime;
        Request(Time processingTime, const User& user) : processingTime(processingTime), user(user), beingProcessedAt(nullptr) {}
        Request(Time processingTime, const User& user, Time startTime) : processingTime(processingTime), user(user), startTime(startTime), beingProcessedAt(nullptr) {}
        
        Request(const Request&) = delete;
        Request(Request&&) = delete;
        Request operator=(const Request&) = delete;
        Request operator=(Request&&) = delete;
};
