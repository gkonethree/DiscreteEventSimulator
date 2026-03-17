#include "metrics.h"
#include<iostream>

void Metrics::insertGoodEvent(Time t){
    successfulReq+=1;
    responseTimes.push_back(t);
}

void Metrics::insertBadEvent(Time t){
    unsuccessfulReq+=1;
    responseTimes.push_back(t);
}

void Metrics::printSummary(Time t){
    Time avgResTime;
    for(Time i: responseTimes){
        avgResTime+=i;
    }
    avgResTime/=(responseTimes.size());
    std::cout<<"Average Res Time "<<avgResTime<<std::endl;
    std::cout<<"Throughtput "<<(successfulReq+unsuccessfulReq)/t<<std::endl;
    std::cout<<"Goodput "<<(successfulReq)/t<<std::endl;
    std::cout<<"Badput "<<unsuccessfulReq/t<<std::endl;

}