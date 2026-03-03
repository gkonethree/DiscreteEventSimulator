#pragma once

#include <memory>
#include "user.h"

class Request{
    public:
        const Time processingTime;
        const User& user;
        Core* beingProcessedAt;
        Request(Time processingTime, const User& user) : processingTime(processingTime), user(user), beingProcessedAt(nullptr) {}
        
        Request(const Request&) = delete;
        Request(Request&&) = delete;
        Request operator=(const Request&) = delete;
        Request operator=(Request&&) = delete;
};
