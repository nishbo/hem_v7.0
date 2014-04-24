#ifndef OUTPUT_H
#define OUTPUT_H

// #include <sstream>
#include <fstream>

#include "core.h"

class Output{
public:
    std::vector<Node *> nodes;
    std::vector<Synapse *> synapses;

    void open();
    void print();
    void print(double t);
    void close();

    void openSpikeFile(std::string filename="data/spikes.txt");
    void push(double time, int node_number);
    void closeSpikeFile();

    void openPotentialFile(std::string filename="data/potentials.txt");
    void printPotentials(double t);
    void closePotentialFile();

    void openSynapticCurrentFile(std::string filename="data/synaptic_currents.txt");
    void printSynapticCurrents(double t);
    void closeSynapticCurrentFile();

    void openSynapticWeightFile(std::string filename="data/synaptic_weights.txt");
    void printSynapticWeights(double t);
    void closeSynapticWeightFile();

    void openSynapticDataFile(std::string filename="data/synaptic_data.txt");
    void printSynapticData(double t);
    void closeSynapticDataFile();

    void printEssentialData(double length=-1.0,\
                            std::string filename="data/simulation.txt");

private:
    std::vector<Spike> _spikes;

    std::ofstream _spikeFile;
    std::ofstream _potentialFile;
    std::ofstream _synapticCurrentFile;
    std::ofstream _synapticWeightFile;
    std::ofstream _synapticDataFile;
};


#endif // OUTPUT_H