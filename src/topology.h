#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include "core.h"

// #include <random>

namespace topology{

    Synapse* connectN2N(Node* preNode, Node* postNode, std::string className);

    std::vector<Synapse *> randomTopology(std::vector<Node *> nodes,\
        std::string className, int connectionsPerNeuron, int waveType=0);

    std::vector<Synapse *> randomTopology(std::vector<Node *> nodes_from,\
        std::vector<Node *> nodes_to,\
        std::string className, int connectionsPerNeuron, int waveType=0);

}


#endif // TOPOLOGY_H