#include "core.h"

double NODE::dt = 0.1;
double NODE::max_delay = 1;

NODE::NODE(std::string _class){
    if( _class.compare("neuron_liaf") != 0 ){
        node_ess = new NEURON_IAF;
    } else if( _class.compare("null_node") != 0 ) {
        node_ess = new NULL_NODE;
    } else {
        node_ess = new NULL_NODE;
    }

    I_stim = 0;
    inc_spikes = new WIDE_CYCLING_TIME_BUFFER (dt, max_delay, node_ess->typesSynapsesSupported()) ;
}

int NODE::addOutgoingSynapse (SYNAPSE* _synapse){
    out_list.push_back(_synapse);
    return 0;
}

int NODE::addIncomingSynapse (SYNAPSE* _synapse){
    inc_list.push_back(_synapse);
    return 0;
}

double NODE::evolve(double _current_time){
    node_ess->evolve()
    return 0;
}

double NODE::addSpike(double _delay, double _weight, int _type){
    inc_spikes->push(_delay, _weight, _type);
    return _weight;
}
/**************************************************************************** */

SYNAPSE::SYNAPSE(std::string _class, NODE* _pn, double _d){
    postneu = _pn;
    delay = _d;

    if( _class.compare("synapse_static") != 0 ){
        syn_ess = new SYNAPSE_STATIC;
    } else if( _class.compare("null_node") != 0 ){
        syn_ess = new NULL_SYNAPSE;
    } else {
        syn_ess = new NULL_SYNAPSE;
    }
}

double SYNAPSE::preSpike(double _current_time){
    return postneu->addSpike( delay, syn_ess->preSpike(_current_time), \
        syn_ess->type() );
}

double SYNAPSE::postSpike(double _current_time){
    return syn_ess->postSpike(_current_time);
}
