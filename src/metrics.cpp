#include "metrics.h"
#include <iostream>
#include <iomanip>

void Metrics::insertGoodEvent(Time t)
{
    successfulReq += 1;
    rCycle.push_back(t);
}

void Metrics::insertBadEvent(Time t)
{
    unsuccessfulReq += 1;
    rCycle.push_back(t);
}

void Metrics::insertCoreUtilization(int serverId, double avgUtilization, const std::vector<double> &coreUtilizations)
{
    avgCoreUtilizationByServer[serverId] = avgUtilization;
    coreUtilizationByServer[serverId] = coreUtilizations;
}

void Metrics::insertNumDroppedReq(int numDropped)
{
    droppedReq += numDropped;
}

void Metrics::insertRsysEvent(Time t)
{
    rSys.push_back(t);
}

void Metrics::printSummary(Time t)
{
    Time avgResTime = 0;
    for (Time i : rCycle)
    {
        avgResTime += i;
    }
    if (rCycle.size() > 0)
        avgResTime /= (rCycle.size());

    Time avgRsysTime = 0;
    for (Time i : rSys)
    {
        avgRsysTime += i;
    }
    if (rSys.size() > 0)
        avgRsysTime /= (rSys.size());


    std::cout << "Average Cycle Time: " << avgResTime << std::endl;
    std::cout << "Average Rsys Time: " << avgRsysTime << std::endl;
    std::cout << "Throughput: " << (successfulReq + unsuccessfulReq) / t << std::endl;
    std::cout << "Goodput: " << (successfulReq) / t << std::endl;
    std::cout << "Badput: " << unsuccessfulReq / t << std::endl;
    std::cout << "Fraction of Dropped Requests: " << droppedReq / (double)(successfulReq + unsuccessfulReq + droppedReq) << std::endl;

 
    for (const auto &entry : avgCoreUtilizationByServer)
    {
        int serverId = entry.first;
        double avgUtil = entry.second;
        const auto &coreUtils = coreUtilizationByServer[serverId];

        std::cout << "Server " << serverId << ":" << std::endl;
        std::cout << "Average Core Utilization: " << std::fixed << std::setprecision(2) << (avgUtil * 100) << "%" << std::endl;
        std::cout << "Individual Core Utilizations: ";
        for (size_t i = 0; i < coreUtils.size(); i++)
        {
            std::cout << std::fixed << std::setprecision(2) << (coreUtils[i] * 100) << "%";
            if (i < coreUtils.size() - 1)
                std::cout << ", ";
        }
        std::cout << std::endl;
    }
}