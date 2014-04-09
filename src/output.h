#ifndef OUTPUT_H
#define OUTPUT_H

// #include <sstream>
#include <fstream>

#include "core.h"

class Output{
public:
    std::vector<Node *> nodes;
    std::vector<Synapse *> synapses;
    
    void print();
    void print(double t);

    void push(double time, int node_number);
    void openSpikeFile(std::string filename="data/spikes.txt");
    void closeSpikeFile();

    void openPotentialFile(std::string filename="data/potentials.txt");
    void closePotentialFile();

    void close();

private:
    std::vector<Spike> _spikes;

    std::ofstream _spikeFile;
    std::ofstream _potentialFile;
};


#endif // OUTPUT_H