#include "synapses.h"

int SYNAPSE_TYPE::type(){
    return 0;
}

int SYNAPSE_TYPE::setData(double* _arr){
    return 1;
}

/******************** NULL SYNAPSE ****************************************** */
// Class basically represents a synapse-filler that does nothing.

std::string NULL_SYNAPSE::classNick(){
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

std::string SYNAPSE_STATIC::classNick(){
    return "static_synapse";
}

int SYNAPSE_STATIC::setData(double* _arr){
    excinh = _arr[0];
    weight = _arr[1];
    return 0;
}

int SYNAPSE_STATIC::type(){
    return excinh;
}

double SYNAPSE_STATIC::preSpike(double _current_time){
    return weight;
}

double SYNAPSE_STATIC::postSpike(double _current_time){
    return weight;
}
