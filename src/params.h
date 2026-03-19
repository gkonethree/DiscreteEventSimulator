#include "event.h"

// const Time thinkMean = 5000;
// const Time thinkVariance = 2000;
// const Time timeout=10000000;
// const int numCores = 1;
// const int numThreadsPerCore = 10;
// const Time timeSlice = 10;
// const int numservers = 1;
// const Time contextSwitchTime = 1;
// const Time propDelay = 1;
// const int bufferSize = 50;
// const Time uniformLow = 1;
// const Time uniformHigh = 10;
// const Time expServiceTimeMean = 10;
// const Time constantServiceTime = 100;

const Time thinkMean = 1000;
const Time thinkVariance = 0;
const Time requestProcessingTime = 7.5;
const Time timeout=10000000;
const int numCores = 1;
const int numThreadsPerCore = 10;
const Time timeSlice = 1000;
const int numservers = 1;
const Time contextSwitchTime = 0;
const Time propDelay = 0;
const int bufferSize = 140;
