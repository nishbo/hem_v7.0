#include "core.h"

double NODE::dt = 0.1;
double NODE::max_delay = 1;

NODE::NODE(std::string _type){
    if( _type.compare("neuron_liaf") != 0 ){
        node_ess = new NEURON_IAF;
    } else if( _type.compare("null_node") != 0 ) {
        node_ess = new NULL_NODE;
    } else {
        node_ess = new NULL_NODE;
    }

    I_stim = 0;
    inc_spikes = WIDE_CYCLING_TIME_BUFFER (dt, max_delay, node_ess->typesSynapsesSupported()) ;
}

int NODE::addSynapse (SYNAPSE* _synapse){
    out_list.push_back(_synapse);
    return 0;
}

double NODE::evolve(double _current_time){

    return 0;
}

double NODE::addSpike(double _delay, double _weight, int _type){
    inc_spikes.push(_delay, _weight, _type);
    return _weight;
}
/**************************************************************************** */

SYNAPSE::SYNAPSE(std::string _type, NODE* _pn, double _d){
    postneu = _pn;
    delay = _d;

    if( _type.compare("synapse_static") != 0 ){
        syn_ess = new SYNAPSE_STATIC;
    } else if( _type.compare("null_node") != 0 ){
        syn_ess = new NULL_SYNAPSE;
    } else {
        syn_ess = new NULL_SYNAPSE;
    }
}

double SYNAPSE::preSpike(double _current_time){
    return postneu->addSpike(syn_ess->preSpike(_current_time), delay);
}

double SYNAPSE::postSpike(double _current_time){
    return syn_ess->postSpike(_current_time);
}
