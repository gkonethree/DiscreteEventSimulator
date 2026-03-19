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
    std::string dist(argv[2]);
    Simulator* simulator;
    if (dist == "uniform")
        simulator = new Simulator(max_time, Distribution::Uniform, numusers);
    else if (dist == "exp")
        simulator = new Simulator(max_time, Distribution::Exponential, numusers);
    else if (dist == "constant")
        simulator = new Simulator(max_time, Distribution::Constant, numusers);
    else{
        std::cout << "Enter correct distribution" << std::endl;
    }

    simulator->run();
    simulator->metrics->printSummary(simulator->lastTime); 
}