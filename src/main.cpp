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

int diploma()
{
    srand(time(NULL));

    double timeMax = 1000.0;
    double dt = 0.1;
    double dumpPeriod = 100.0;

    Node::dt = dt;

    std::vector<Node *> nodes;
    std::vector<Node *> inh_nodes;
    std::vector<Synapse *> synapses;

    for (int i=0; i < 800; i++){
        nodes.push_back(new Node("leaky iaf"));
        nodes.back()->I_stim = 14.0 + 2.0*drand(); // 14.0-16.0
        nodes.back()->addPsWaveType(PsWave(3.0), "current", 1.0); // exc input
        nodes.back()->addPsWaveType(PsWave(3.0), "current", -1.0); // inh input
        nodes.back()->initialiseSpikeBuffer(0.1);
    }

    for (int i=0; i < 200; i++){
        inh_nodes.push_back(new Node("leaky iaf"));
        inh_nodes.back()->I_stim = 14.0 + 2.0*drand(); // 14.0-16.0
        inh_nodes.back()->addPsWaveType(PsWave(3.0), "current", 1.0); // exc input
        inh_nodes.back()->addPsWaveType(PsWave(3.0), "current", -1.0); // inh input
        inh_nodes.back()->initialiseSpikeBuffer(0.1);
        inh_nodes.back()->setPreset(1); // inh neuron params
    }
    nodes.reserve(nodes.size() + inh_nodes.size());
    nodes.insert(nodes.end(), inh_nodes.begin(), inh_nodes.end());

    std::vector<Synapse *> synapses_ee = topology::randomTopology(\
        nodes, "stdp and tm", 80, 0, "agile boundaries");
    std::vector<Synapse *> synapses_ei = topology::randomTopology(\
        nodes, inh_nodes, "stdp and tm", 20, 0, "agile boundaries");
    for (auto synapse : synapses_ei){
        synapse->setPreset(1);
        synapse->reset();
    }
    std::vector<Synapse *> synapses_ie = topology::randomTopology(\
        inh_nodes, nodes, "stdp and tm", 80, 1, "agile boundaries");
    for (auto synapse : synapses_ie){
        synapse->setPreset(2);
        synapse->reset();
    }
    std::vector<Synapse *> synapses_ii = topology::randomTopology(\
        inh_nodes, "stdp and tm", 20, 1, "agile boundaries");
    for (auto synapse : synapses_ii){
        synapse->setPreset(3);
        synapse->reset();
    }

    synapses.reserve(synapses.size() + synapses_ee.size() + synapses_ei.size()\
                    + synapses_ie.size() + synapses_ii.size());
    synapses.insert(synapses.end(), synapses_ee.begin(), synapses_ee.end());
    synapses.insert(synapses.end(), synapses_ei.begin(), synapses_ei.end());
    synapses.insert(synapses.end(), synapses_ie.begin(), synapses_ie.end());
    synapses.insert(synapses.end(), synapses_ii.begin(), synapses_ii.end());

    // nodes.push_back(new Node("periodic generator"));
    // nodes.push_back(new Node("neuron liaf"));
    // nodes.back()->I_stim = 0.0;
    // nodes.back()->addPsWaveType(PsWave(3.0), "current", 1.0);
    // nodes.back()->initialiseSpikeBuffer(0.1);

    // std::vector<Synapse *> synapses;
    // for (int i=0; i < 100; i++){
    //     synapses.push_back(topology::connectN2N(nodes[0], nodes[1], "synapse tm"));
    //     synapses.back()->waveType = 0;
    // }

    Output output;
    // output.open();
    output.openSpikeFile();
    // output.openSynapticCurrentFile();
    // output.openSynapticWeightFile();
    output.nodes = nodes;
    output.synapses = synapses;
    output.printEssentialData(timeMax);

    for (double t = 0.0; t <= timeMax; t += dt){
        // std::cout<<"t "<<t<< std::endl;
        for (unsigned i=0; i<nodes.size(); i++)
            if (nodes.at(i)->step(t))
                output.push(t, i);

        // for (unsigned i=0; i<inh_nodes.size(); i++)
        //     if (inh_nodes.at(i)->step(t))
        //         output.push(t, i+400);

        if (isPeriodNow(t, dumpPeriod, dt))
            output.print(t);

        std::cout<<"Finished "<< t / timeMax <<" of the simulation.\t\t\r";
    }
    std::cout<<std::endl;
    output.close();

    std::cout<<std::endl;
    return 0;
}

int main(int argc, char const *argv[])
{
    diploma();
    return 0;
}