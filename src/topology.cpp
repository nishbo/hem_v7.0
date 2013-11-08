#include "topology.h"

int topology::connectN2S(NODE* _from, SYNAPSE* _syn){
    return _from->addOutgoingSynapse(_syn);
}

SYNAPSE* topology::connectS2N(std::string _class, NODE* _to, double _delay, \
                                std::vector<SYNAPSE*> _allsyn){
    SYNAPSE* buf = new SYNAPSE(_class, _to, _delay);
    _allsyn.push_back(buf);
    _to->addIncomingSynapse(buf);
    return buf;
}

int topology::connectN2N(NODE* _from, std::string _class, NODE* _to, \
                    double _delay, std::vector<SYNAPSE*> _allsyn){
    return connectN2S(_from, connectS2N(_class, _to, _delay, _allsyn));
}




