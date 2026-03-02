#pragma once


#include <memory>
#include "event.h"
#include "link.h"


class User: public Linkable{
    Time thinkTime;
    public:
        User(Time thinkTime) : thinkTime(thinkTime){}
        ListOfEvents receive(std::shared_ptr<RequestUnloadEvent> event);
};
