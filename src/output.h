#ifndef OUTPUT_H
#define OUTPUT_H

// #include <sstream>
#include <fstream>

#include "core.h"

class Output{
public:
    void push(double time, Node* node);

    void print();

    void openSpikeFile(std::string filename="data/spikes.txt");
    void closeSpikeFile();

private:
    std::vector<Spike> _spikes;

    std::vector<Node *> _nodes;
    std::vector<Synapse *> _synapses;

    std::ofstream _spikeFile;
};


#endif // OUTPUT_H