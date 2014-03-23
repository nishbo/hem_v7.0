#ifndef SYNAPSES_H
#define SYNAPSES_H

#include <iostream>

class SynapseType{
public:
    virtual std::string getClassNick()=0;
    virtual double preSpike(double current_time);
    virtual void postSpike(double current_time);
};


class NullSynapse: public SynapseType{
public:
    std::string getClassNick();
};


class SynapseStatic: public SynapseType{
public:
    SynapseStatic();

    std::string getClassNick();
    double preSpike(double current_time);
private:
    double _weight;
};


#endif // SYNAPSES_H