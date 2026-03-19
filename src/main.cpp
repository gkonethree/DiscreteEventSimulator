#include "simulator.h"
#include "server.h"
#include "user.h"
#include "link.h"
#include "core.h"
#include "request.h"
#include "thread.h"

int main(int argc, char* argv[]){
    Time max_time=100000;
    int numusers=atoi(argv[1]);
    Simulator simulator(max_time, numusers);
    simulator.run();
    simulator.metrics->printSummary(simulator.lastTime); 
}