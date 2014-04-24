#include "topology.h"


Synapse* topology::connectN2N(Node* preNode, Node* postNode,\
                              std::string className)
{
    Synapse* synapse = new Synapse(className, postNode);
    postNode->addIncomingSynapse(synapse);
    preNode->addOutgoingSynapse(synapse);
    return synapse;
}

std::vector<Synapse *> topology::randomTopology(\
        std::vector<Node *> nodes, std::string className,\
        int connectionsPerNeuron, int waveType){
    std::vector<Synapse *> synapses;

    if (connectionsPerNeuron > 0){
        for (auto node : nodes) {
            for(int i=0; i < connectionsPerNeuron; i++){
                synapses.push_back(\
                    connectN2N(node, nodes[rand() % nodes.size()], className));
                synapses.back()->waveType = waveType;
            }
        }
    }

    return synapses;
}

std::vector<Synapse *> topology::randomTopology(\
        std::vector<Node *> nodes_from, std::vector<Node *> nodes_to,\
        std::string className, int connectionsPerNeuron, int waveType){
    std::vector<Synapse *> synapses;

    if (connectionsPerNeuron > 0){
        for (auto node : nodes_from) {
            for(int i=0; i < connectionsPerNeuron; i++){
                synapses.push_back(\
                    connectN2N(node, nodes_to[rand() % nodes_to.size()], className));
                synapses.back()->waveType = waveType;
            }
        }
    }

    return synapses;
}
