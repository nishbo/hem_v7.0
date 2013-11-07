#ifndef SYNAPSES_H
#define SYNAPSES_H

#include <iostream>

class SYNAPSE_TYPE{
public:
    virtual std::string type()=0;
    virtual double preSpike(double _current_time)=0;
    virtual double postSpike(double _current_time)=0;
};

class NULL_SYNAPSE: public SYNAPSE_TYPE{
public:
    std::string type();
    double preSpike(double _current_time);
    double postSpike(double _current_time);
};

class SYNAPSE_STATIC: public SYNAPSE_TYPE{
    double weight;
public:
    std::string type();
    double preSpike(double _current_time);
    double postSpike(double _current_time);
};


#endif // SYNAPSES_H