#include <iostream>
#include <cstring>
#include <vector>
#include <ctime>
#include <cmath>

#include "core.h"
#include "topology.h"
#include "output.h"


double drand()
{
    return ((double) rand() / RAND_MAX);
}


bool isPeriodNow(double t, double period, double dt)
{
    return (t - (floor((t + dt*0.001)/period) * period)) < dt * 0.05;
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    // auto wcb = WideCyclingTimeBuffer();
    // wcb.init(0.1, 1.12, 3);
    // wcb.push(0.5, 5, 0);
    // wcb.push(0.2, 5, 0);
    // wcb.push(1.12, 5, 0);
    // wcb.push(0.6, 5, 1);
    // wcb.push(0.6, 3, 1);
    // wcb.push(0.2, 1, 2);
    // wcb.push(0.3, 1, 2);
    // for (double t=0.0; t<1.8; t+=0.1){
    //     if (fabs(t - 0.3) < 1e-5)
    //         wcb.push(0.35, 20, 2);
    //     if (fabs(t - 0.4) < 1e-5)
    //         wcb.push(1.12, 20, 2);
    //     std::vector<double> a = wcb.pull();
    //     std::cout<<"Time: "<< t;
    //     for( auto d : a ) {
    //         std::cout<<"  \t"<<d;
    //     }
    //     std::cout<<std::endl;
    // }


    double timeMax = 1000.0;
    double dt = 0.1;
    double dumpPeriod = 5.0;

    Node::dt = dt;

    std::vector<Node *> nodes;
    std::vector<Synapse *> synapses;

    for (int i=0; i < 1000; i++){
        nodes.push_back(new Node("neuron_liaf"));
        nodes.back()->I_stim = 20.0 * (0.5 + drand());
        nodes.back()->addPsWaveType(PsWave(3.0), "current", 1.0);
        nodes.back()->initialiseSpikeBuffer(0.3);
    }

    synapses = topology::randomTopology(nodes, "synapse_static", 50);

    // std::cout<<"\ntesting created connections\n";

    // for (auto node : nodes){
    //     std::cout<<std::endl<<std::endl<< node;
    //     node->announceConnections();
    // }
    // std::cout<<std::endl;

    Output output;
    output.openSpikeFile();
    output.openPotentialFile();
    output.nodes = nodes;

    for (double t = 0.0; t < timeMax; t += dt){
        // std::cout<<"t "<<t<< std::endl;
        for (unsigned i=0; i<nodes.size(); i++){
            if (nodes.at(i)->step(t)){
                // std::cout<<"Node "<<i<<" spiked."<< std::endl;
                output.push(t, i);
            }
        }
        if (isPeriodNow(t, dumpPeriod, dt)){
            output.print(t);
        }
        std::cout<<"Finished "<< t / timeMax <<" of the simulation.\r";
    }
    std::cout<<std::endl;
    // output.print();
    output.close();


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