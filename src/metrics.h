#pragma once

#include "event.h"
#include <vector>

class Metrics{

        int successfulReq;
        int unsuccessfulReq;
        std::vector<Time> responseTimes; 

    public:
        void insertGoodEvent(Time t);
        void insertBadEvent(Time t);
        void printSummary(Time t);

};