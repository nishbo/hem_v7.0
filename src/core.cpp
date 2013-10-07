#include "core.h"

LIST* NODE::runner = NULL;
float NODE::dt = 0.1;
float NODE::max_delay = 10;

NODE::NODE(std::string _type){
    out_list = NULL;
    length_of_buffer = max_delay / dt + 1;
    inc_spikes = new float [length_of_buffer];
    for (int i = 0; i < length_of_buffer; i++)
        inc_spikes[i] = 0;
    
    if(!strcmp(_type.c_str(), "neuron_liaf")){
        node_ess = new NEURON_IAF;
    } else {
        node_ess = NULL;
    }
    I = 0;
    I_ext = 0;
}

int NODE::addSynapse (SYNAPSE* _synapse){
    out_list = new LIST (_synapse, out_list);
    return 0;
}

float NODE::evolve(float _current_time){
    I = I_ext + 
    if(node_ess->evolve(_current_time, dt, I)){
        runner = out_list;
        while(runner){
            runner->syn->preSpike(_current_time);
            runner = runner -> next;
        }
        return 1;
    }
    return 0;
}

float NODE::addSpike(float _weight, float _time){
    inc_spikes[(int) (_time / dt)] += _weight;
    return _weight;
}
/**************************************************************************** */

SYNAPSE::SYNAPSE(std::string _type, NODE* _pn, float _d){
    postneu = _pn;
    delay = _d;

    if(!strcmp(_type.c_str(), "synapse_static")){
        syn_ess = new SYNAPSE_STATIC;
    } else {
        syn_ess = NULL;
    }
}

float SYNAPSE::preSpike(float _current_time){
    return postneu->addSpike(syn_ess->preSpike(_current_time), delay);
}

float SYNAPSE::postSpike(float _current_time){
    return syn_ess->postSpike(_current_time);
}
/**************************************************************************** */

LIST::LIST(SYNAPSE* _synapse, LIST* _oh){
    syn = _synapse;
    next = _oh;
}
