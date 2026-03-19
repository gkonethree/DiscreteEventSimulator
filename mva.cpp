#include "src/params.h"
#include <vector>

using namespace std;

void computeMVA(int M,
                vector<double>& Ns,
                vector<double>& Ts,
                vector<Time>& Rs_sys,
                vector<Time>& Rs_server)
{
    Ns.resize(M+1);
    Ts.resize(M+1);
    Rs_sys.resize(M+1);
    Rs_server.resize(M+1);

    Ns[0] = 0;  

    for(int m = 1; m <= M; m++)
    {
        Rs_server[m] = (Ns[m-1] + 1) * requestProcessingTime;
        Rs_sys[m] = Rs_server[m] + thinkMean;
        Ts[m] = m / Rs_sys[m];
        Ns[m] = Ts[m] * Rs_server[m];
    }
}

int main(){
    int M = 20; 
    vector<double> Ns, Ts;
    vector<Time> Rs_sys, Rs_server;
    computeMVA(M, Ns, Ts, Rs_sys, Rs_server);
    for(int m = 1; m <= M; m++) {

        cout << "m=" << m <<"Rs_sys=" << Rs_sys[m]  << " T=" << Ts[m]<< " Ns=" << Ns[m]<< endl;
    }
}