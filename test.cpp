#include <vector>
#include <iostream>
#include <algorithm>
#include <random>



#include <random>

std::random_device rd;
std::mt19937 gen(rd());

class Obj{
    int x;
    public:
        Obj(int x): x(x){}
        ~Obj(){
            std::cout << "Destroyed " << x << std::endl;
        }
};

int main(){
    std::discrete_distribution<int> dist({1, 2});
    int count = 0;
    for(int i = 0; i < 200; i++){
        std::cout << dist(gen) << std::endl;
    }
    std::cout << count << std::endl;
}