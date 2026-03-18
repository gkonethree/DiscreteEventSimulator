#pragma once

#include "event.h"
#include <vector>
#include <unordered_map>

class Metrics
{
    int successfulReq;
    int unsuccessfulReq;
    int droppedReq;
    std::vector<Time> responseTimes;
    std::unordered_map<int, std::vector<double>> coreUtilizationByServer;
    std::unordered_map<int, double> avgCoreUtilizationByServer;

public:
    void insertGoodEvent(Time t);
    void insertBadEvent(Time t);
    void insertCoreUtilization(int serverId, double avgUtilization, const std::vector<double> &coreUtilizations);
    void printSummary(Time t);
    
};