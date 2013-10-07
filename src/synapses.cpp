#include "synapses.h"

float SYNAPSE_STATIC::preSpike(float _current_time){
    return weight;
}

float SYNAPSE_STATIC::postSpike(float _current_time){
    return weight;
}
