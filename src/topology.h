#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include "core.h"

// #include <random>

namespace topology{

    Synapse* connectN2N(Node* preNode, Node* postNode, std::string className,\
        std::string stdpType="no boundaries");

    void connectN2N(Node* preNode, Node* postNode, Synapse* synapse);

    std::vector<Synapse *> randomTopology(std::vector<Node *> nodes,\
        std::string className, int connectionsPerNeuron, int waveType=0,\
        std::string stdpType="no boundaries");

    std::vector<Synapse *> randomTopology(std::vector<Node *> nodes_from,\
        std::vector<Node *> nodes_to,\
        std::string className, int connectionsPerNeuron, int waveType=0,\
        std::string stdpType="no boundaries");

}


#endif // TOPOLOGY_H