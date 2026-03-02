#include "simulator.h"
#include "server.h"
#include "user.h"
#include "link.h"
#include "request.h"

int main(){
    Simulator simulator(10000);
    simulator.run();
}