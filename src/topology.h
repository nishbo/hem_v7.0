#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include "core.h"

namespace topology{
    int connectN2S(NODE* _from, SYNAPSE* _syn);
    SYNAPSE* connectS2N(std::string _class, NODE* _to, double _delay, \
        std::vector<SYNAPSE*> _allsyn);
    int connectN2N(NODE* _from, std::string _class, NODE* _to, \
        double _delay, std::vector<SYNAPSE*> _allsyn);
}


#endif // TOPOLOGY_H