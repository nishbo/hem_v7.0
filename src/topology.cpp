#include "topology.h"

int topology::connectN2S(NODE* _from, SYNAPSE* _syn){
    return _from->addOutgoingSynapse(_syn);
}

SYNAPSE* topology::connectS2N(std::string _class_name, NODE* _to, double _delay, \
        std::vector<SYNAPSE*> _allsyn){
    SYNAPSE* buf = new SYNAPSE(_class_name, _to, _delay);
    _allsyn.push_back(buf);
    _to->addIncomingSynapse(buf);
    return buf;
}

int topology::connectN2N(NODE* _from, std::string _class_name, NODE* _to, \
        double _delay, std::vector<SYNAPSE*> _allsyn){
    return connectN2S(_from, connectS2N(_class_name, _to, _delay, _allsyn));
}

int topology::randomTopologyOneDelay(std::vector<NODE*> _node_array, int _m, 
        std::string _class_name, double _delay, std::vector<SYNAPSE*> _allsyn){
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


