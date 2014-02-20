#include <iostream>
#include <cstring>
#include <vector>

#include "core.h"
#include "topology.h"

int main(int argc, char const *argv[]){

    int N = 10;
    double T_max = 1;
    double dt = 0.1;
    
    NODE::dt = dt;
    NODE::max_delay = 5;

    std::vector<NODE*> node_array;
    std::vector<SYNAPSE*> allsyn;
    for(int i=0; i < N; i++){
        node_array.push_back(new NODE("neuron_liaf"));
        node_array[i]->I_stim = 25;
    }

    topology::randomTopologyOneDelay(node_array, 2, "synapse_static", 0.1, allsyn);

    for (double t = 0; t < T_max+dt; t+=dt){
        for(int i=0; i < N; i++){
            if (node_array[i]->evolve(t))
                std::cout<< "\nSpike! Neuron: "<< i <<", time:"<<t;
        }
    }

    std::cout<<std::endl;
    return 0;
}