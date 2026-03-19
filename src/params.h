#include "event.h"

const Time thinkMean = 5000;
const Time thinkVariance = 2000;
const Time requestProcessingTime = 100;
const Time timeout=10000000;
const int numCores = 1;
const int numThreadsPerCore = 1;
const Time timeSlice = 10;
const int numservers = 1;
const Time contextSwitchTime = 1;
const Time propDelay = 1;
const int bufferSize = 50;