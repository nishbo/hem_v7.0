#include "topology.h"

void topology::connectN2S(Node* preNeuron, Synapse* synapse){
    preNeuron->addOutgoingSynapse(synapse);
}

Synapse* topology::connectS2N(std::string class_name, Node* postNode,\
    double delay, std::vector<Synapse*> allsyn){
    Synapse* buf = new Synapse(class_name, postNode, delay);
    allsyn.push_back(buf);
    postNode->addIncomingSynapse(buf);
    return buf;
}

void topology::connectN2N(Node* _from, std::string _class_name, Node* _to, \
        double _delay, std::vector<Synapse*> _allsyn){
    connectN2S(_from, connectS2N(_class_name, _to, _delay, _allsyn));
}

int topology::randomTopologyOneDelay(std::vector<Node*> _node_array, int _m, 
        std::string _class_name, double _delay, std::vector<Synapse*> _allsyn){
    if( _m <= 0 )
        return 2;
    
    // std::default_random_engine generator;
    // std::uniform_int_distribution<int> distribution(0, _node_array.size()-1);

    for(int i=0; i < _node_array.size(); i++){
        for(int j=0; j < _m; j++){
            int k = rand() % _node_array.size();
            // int k = distribution(generator);
            std::cout<<"\nConnecting "<<i<<" to "<< k;
            connectN2N(_node_array[i], _class_name, \
                _node_array[k], _delay, _allsyn);
        }
    }

    return 0;
}


