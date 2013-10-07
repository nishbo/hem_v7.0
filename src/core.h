#ifndef CORE_H
#define CORE_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>

#include "nodes.h"
#include "synapses.h"

class LIST;
class NODE;
class SYNAPSE;

class NODE{
    NODE_TYPE* node_ess;
    float* inc_spikes; //buffer for incoming spikes
    int length_of_buffer; //length of spike buffer
    static LIST* runner; //runner to go through LIST lists
    LIST* out_list; // list of outgoing synapses
    float I_ext;
    float I;
    int inc_spikes_iter;

    NODE();
public:
    static float dt; //needed for buffer
    static float max_delay; //maximum of delays of incoming synapses

    NODE(std::string _type);
    float evolve(float _current_time); //node evolves for dt
    int addSynapse(SYNAPSE* _synapse); //add an outgoing synapse for this node
    float addSpike(float _weight, float _time); //spike arrives after _time with _weight amount
};

class SYNAPSE{
    NODE* postneu; //postsynaptic neuron
    SYNAPSE_TYPE* syn_ess;
    float delay;

    SYNAPSE();
public:
    SYNAPSE(std::string _type, NODE* _pn, float _d);
    float preSpike(float _current_time); //signal of presynaptic spike
    float postSpike(float _current_time); //signal of postsynaptic spike
};


class LIST{
//singly linked list of synapses. used in nodes and main
public:
    LIST(SYNAPSE* _synapse, LIST* _oh);
    SYNAPSE* syn; //some synapse
    LIST* next; // next in line
};

#endif // CORE_H
