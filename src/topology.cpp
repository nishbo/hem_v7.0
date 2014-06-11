#include "topology.h"


Synapse* topology::connectN2N(Node* preNode, Node* postNode,\
                              std::string className, std::string stdpType)
{
    Synapse* synapse = new Synapse(className, postNode, stdpType);
    connectN2N(preNode, postNode, synapse);
    return synapse;
}

void topology::connectN2N(Node* preNode, Node* postNode, Synapse* synapse)
{
    postNode->addIncomingSynapse(synapse);
    preNode->addOutgoingSynapse(synapse);
}

std::vector<Synapse *> topology::randomTopology(\
        std::vector<Node *> nodes, std::string className,\
        int connectionsPerNeuron, int waveType, std::string stdpType){

    return randomTopology(nodes, nodes, className, connectionsPerNeuron, waveType,\
        stdpType);
}

std::vector<Synapse *> topology::randomTopology(\
        std::vector<Node *> nodes_from, std::vector<Node *> nodes_to,\
        std::string className, int connectionsPerNeuron, int waveType,\
        std::string stdpType){
    std::vector<Synapse *> synapses;

    if (connectionsPerNeuron > 0){
        for (auto node : nodes_from) {
            for(int i=0; i < connectionsPerNeuron; i++){
                synapses.push_back(\
                    connectN2N(node, nodes_to[rand() % nodes_to.size()],\
                               className, stdpType));
                synapses.back()->waveType = waveType;
            }
        }
    }

    return synapses;
}
