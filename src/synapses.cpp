#include "synapses.h"


double SynapseType::preSpike(double _current_time){
    return 0;
}

void SynapseType::postSpike(double _current_time){
}

/******************** NULL SYNAPSE ****************************************** */
// Class basically represents a synapse-filler that does nothing.

std::string NullSynapse::getClassNick(){
    return "null_synapse";
}

/******************** SYNAPSE STATIC **************************************** */
// A static synapse that does not change weight.

SynapseStatic::SynapseStatic(){
    _weight = 1.0;
}

std::string SynapseStatic::getClassNick(){
    return "static_synapse";
}

double SynapseStatic::preSpike(double _current_time){
    return _weight;
}
