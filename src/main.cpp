#include <iostream>
#include <cstring>
#include <vector>
#include <ctime>
#include <cmath>

// #include "core.h"
// #include "topology.h"
// #include "output.h"
#include "cyctimbuf.h"

double drand()
{
    return ((double) rand() / RAND_MAX);
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    auto wcb = WideCyclingTimeBuffer();
    wcb.init(0.1, 1.12, 3);
    wcb.push(0.5, 5, 0);
    wcb.push(0.2, 5, 0);
    wcb.push(1.12, 5, 0);
    wcb.push(0.6, 5, 1);
    wcb.push(0.6, 3, 1);
    wcb.push(0.2, 1, 2);
    wcb.push(0.3, 1, 2);
    for (double t=0.0; t<1.8; t+=0.1){
        if (fabs(t - 0.3) < 1e-5)
            wcb.push(0.35, 20, 2);
        if (fabs(t - 0.4) < 1e-5)
            wcb.push(1.12, 20, 2);
        std::vector<double> a = wcb.pull();
        std::cout<<"Time: "<< t;
        for( auto d : a ) {
            std::cout<<"  \t"<<d;
        }
        std::cout<<std::endl;
    }


    // int N = 10;
    // double T_max = 1;
    // double dt = 0.1;
    
    // Node::dt = dt;
    // Node::max_delay = 5;

    // std::vector<Node*> node_array;
    // std::vector<Synapse*> allsyn;
    // for(int i=0; i < N; i++){
    //     node_array.push_back(new Node("neuron_liaf"));
    //     node_array[i]->I_stim = 25 * (1 + 0.5*drand());
    // }
    // std::vector<std::string> v {"spikes", "spikes", "potentials"};
    // // output printer (v);

    // topology::randomTopologyOneDelay(node_array, 2, "synapse_static", 0.1, allsyn);

    // for (double t = 0; t < T_max+dt; t+=dt){
    //     for(int i=0; i < N; i++){
    //         if (node_array[i]->evolve(t))
    //             std::cout<< " \nSpike! Neuron: "<< i <<", time:"<<t<<std::endl;
    //     }
    // }

    std::cout<<std::endl;
    return 0;
}