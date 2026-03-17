#include "simulator.h"
#include "server.h"
#include "user.h"
#include "link.h"
#include "core.h"
#include "request.h"
#include "thread.h"

int main(){
    Time max_time=10000;
    Simulator simulator(max_time);
    simulator.run();
    simulator.metrics->printSummary(simulator.lastTime);
}