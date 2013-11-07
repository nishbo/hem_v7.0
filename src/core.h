#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <string>
#include <vector>

#include "nodes.h"
#include "synapses.h"
#include "cyctimbuf.h"

class NODE;
class SYNAPSE;

class NODE{
    NODE_TYPE* node_ess;        //essence of the node - defines it behavior
    std::vector<SYNAPSE*> out_list; // list of outgoing synapses

    WIDE_CYCLING_TIME_BUFFER inc_spikes;

    double I_stim;

    NODE(); // You do not want to create just a node! define it's type!
public:
    static double dt;
    static double max_delay;

    NODE(std::string _type);
    double evolve(double _current_time); //node evolves for dt
    int addSynapse(SYNAPSE* _synapse); //add an outgoing synapse for this node
    //spike arrives after _time with _weight amount :
    double addSpike(double _delay, double _weight, int _type = 0); 
};

class SYNAPSE{
    NODE* postneu; //postsynaptic neuron
    SYNAPSE_TYPE* syn_ess;  //essence of the synapse - defines it evolve
    double delay;    // some time-constant defined by length of synapse and speed 
                    // of propagation of spike

    SYNAPSE(); // You do not want to create just a synapse! define it's type!
public:
    SYNAPSE(std::string _type, NODE* _pn, double _d);
    double preSpike(double _current_time); //signal of presynaptic spike
    double postSpike(double _current_time); //signal of postsynaptic spike
};

#endif // CORE_H
