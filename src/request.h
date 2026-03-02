#pragma once

#include <memory>
#include "user.h"

class Request{
    public:
        const Time processingTime;
        const User& user;
        Request(Time processingTime, const User& user) : processingTime(processingTime), user(user){}
};
