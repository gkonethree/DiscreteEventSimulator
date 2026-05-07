#include "simulator.h"
#include "server.h"
#include "user.h"
#include "link.h"
#include "core.h"
#include "request.h"
#include "thread.h"

int main(){
    Simulator simulator(10000);
    simulator.run();
}