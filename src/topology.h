#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include "core.h"

// #include <random>

namespace topology{
    // void connectN2S(Node* preNode, Synapse* synapse);

    // Synapse* connectS2N(std::string class_name, Node* postNode, double delay, \
    //     std::vector<Synapse*> allsyn);

    Synapse* connectN2N(Node* preNode, Node* postNode, std::string className);

    std::vector<Synapse *> randomTopology(\
        std::vector<Node *> nodes, std::string className,\
        int connectionsPerNeuron);

    // void connectN2N(Node* _from, std::string _class_name, Node* _to, \
    //     double _delay, std::vector<Synapse*> _allsyn);

    // int randomTopologyOneDelay(std::vector<Node*> _node_array, int _m, \
    //     std::string _class_name, double _delay, \
    //     std::vector<Synapse*> _allsyn);
}


#endif // TOPOLOGY_H