#ifndef SYNAPSES_H
#define SYNAPSES_H

#include <iostream>

class SYNAPSE_TYPE{
public:
    virtual float preSpike(float _current_time)=0;
    virtual float postSpike(float _current_time)=0;
};

class SYNAPSE_STATIC: public SYNAPSE_TYPE{
    float weight;
public:
    float preSpike(float _current_time);
    float postSpike(float _current_time);
};


#endif // SYNAPSES_H