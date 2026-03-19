#include "simulator.h"
#include "server.h"
#include "user.h"
#include "link.h"
#include "core.h"
#include "request.h"
#include "thread.h"
using namespace std;
int main(int argc, char *argv[])
{
    Time max_time = 100000;
    int numusers = atoi(argv[1]);
    std::string dist(argv[2]);
    Simulator *simulator;
    if (dist == "uniform"){
        simulator = new Simulator(max_time, Distribution::Uniform, numusers);
        cout << "Running with uniform distribution" << endl;
    }
    else if (dist == "exp"){
        simulator = new Simulator(max_time, Distribution::Exponential, numusers);
        cout << "Running with exponential distribution" << endl;
    }
    else if (dist == "constant"){
        simulator = new Simulator(max_time, Distribution::Constant, numusers);
        cout << "Running with constant distribution" << endl;
    }
    else
    {
        std::cout << "Enter correct distribution" << std::endl;
    }

    simulator->run();
    simulator->metrics->printSummary(simulator->lastTime);
}