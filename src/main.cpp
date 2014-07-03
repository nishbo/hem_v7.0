#include <cmath>
#include <ctime>

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

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

int maass()
{
/*
зависимость выходной частоты от тока в первом эксперименте
зависимость порога от характера сигнала
зависимость порога от реализации
зависимость частоты выходного сигнала от порога
использовать сигнал кости
*/

    srand(time(NULL));

    double timeMax = 50.0;
    double dt = 0.01;
    double dumpPeriod = 10.0;

    Node::dt = dt;

    std::vector<Node *> generators;
    std::vector<Synapse *> synapses1;
    std::vector<Synapse *> synapses2;

    std::cout<<"Creating neurons..."<<std::endl;

    Node node1 = Node("leaky iaf");
    node1.I_stim = genran::uniform(14.5, 15.5);
    node1.addPsWaveType(PsWave(3.0), "current", 1.0); // exc input
    node1.addPsWaveType(PsWave(6.0), "current", -1.0); // inh input
    node1.initialiseSpikeBuffer(dt);

    Node node2 = Node("leaky iaf");
    node2.I_stim = node1.I_stim;
    node2.addPsWaveType(PsWave(3.0), "current", 1.0); // exc input
    node2.addPsWaveType(PsWave(6.0), "current", -1.0); // inh input
    node2.initialiseSpikeBuffer(dt);

    std::cout<<"Creating generators..."<<std::endl;

    for (int i=0; i < 90; i++){
        generators.push_back(new Node("poisson generator"));
        Synapse* synapse1 = new Synapse("stdp and tm", &node1, "hard boundaries");
        synapse1->setPreset(0); // excitatory constants
        synapse1->control(0); // disable stdp and shuffle tm
        synapse1->reset();
        topology::connectN2N(generators.back(), &node1, synapse1);
        synapses1.push_back(synapse1);

        Synapse* synapse2 = synapse1->duplicate(&node2); //copy everything
        synapse2->control(1); // enable stdp with gamma, don't touch tm
        synapse2->reset();
        topology::connectN2N(generators.back(), &node2, synapse2);
        synapses2.push_back(synapse2);

        std::cout<<"\t\t generators created: "<< i << "%\r";
    }

    for (int i=0; i < 10; i++){
        generators.push_back(new Node("poisson generator"));

        Synapse* synapse1 = new Synapse("stdp and tm", &node1, "hard boundaries");
        synapse1->setPreset(2); // inhibitory constants
        synapse1->control(0); // disable stdp and shuffle tm
        synapse1->reset();
        synapse1->waveType = 1; // inhibitory
        topology::connectN2N(generators.back(), &node1, synapse1);
        // synapses1.push_back(synapse1);

        Synapse* synapse2 = synapse1->duplicate(&node2); //copy everything
        topology::connectN2N(generators.back(), &node2, synapse2);
        // synapses2.push_back(synapse2);

        std::cout<<"\t\t generators created: "<< i+90 << "%\r";
    }
    std::cout<<std::endl;

    std::cout<<"Start output..."<<std::endl;

    Output output;
    output.openSpikeFile();

    output.openSynapticWeightFile("data/target.txt");
    output.synapses = synapses1;
    output.printSynapticWeights(0.0);
    output.closeSynapticWeightFile();

    output.openSynapticWeightFile("data/start.txt");
    output.synapses = synapses2;
    output.printSynapticWeights(0.0);
    output.closeSynapticWeightFile();

    output.printEssentialData(timeMax);
    std::string s;
    std::stringbuf dumpTimesBuffer;
    std::ostream dumpTimes (&dumpTimesBuffer);

    std::cout<<"Starting simulation..."<<std::endl;

    for (double t = 0.0; t <= timeMax+dt; t += dt){
        for (unsigned i=0; i<generators.size(); i++)
            if (generators.at(i)->step(t))
                output.push(t, i); //generator numbers range from 0 to 99

        if (node2.step(t))
            output.push(t, 199);

        if (node1.step(t) and not node2.isSpiking(t)){
            node2.forceSpike(t);
            output.push(t, 151);
        }

        if (isPeriodNow(t, dumpPeriod, dt) and t>0.0){
            s = std::string("data/dump_") + std::to_string(t)+ ".txt";
            dumpTimes << s <<std::endl;
            output.openSynapticWeightFile(s);
            output.print(t);
            output.closeSynapticWeightFile();
        }

        std::cout<<"Finished "<< t / timeMax <<" of the simulation.\t\t\r";
    }
    std::cout<<std::endl;
    output.close();

    std::cout<<"Finish output..."<<std::endl;

    output.openSynapticWeightFile("data/finish.txt");
    output.printSynapticWeights(timeMax);
    output.closeSynapticWeightFile();

    std::ofstream dumpTimesFile;
    dumpTimesFile.open("data/dump_times.txt", std::ofstream::out);
    dumpTimesFile << dumpTimesBuffer.str();
    dumpTimesFile.close();

    return 0;
}

int main(int argc, char const *argv[])
{
    maass();
    return 0;
}
