#include "synapses.h"

/******************** NULL SYNAPSE ****************************************** */
// Class basically represents a synapse-filler that does nothing.

std::string NULL_SYNAPSE::type(){
    return "null_synapse";
}

double NULL_SYNAPSE::preSpike(double _current_time){
    return 0;
}

double NULL_SYNAPSE::postSpike(double _current_time){
    return 0;
}

/******************** SYNAPSE STATIC **************************************** */
// A static synapse that does not change weight.

std::string SYNAPSE_STATIC::type(){
    return "static_synapse";
}

double SYNAPSE_STATIC::preSpike(double _current_time){
    return weight;
}

double SYNAPSE_STATIC::postSpike(double _current_time){
    return weight;
}
