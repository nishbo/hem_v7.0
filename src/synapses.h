#ifndef SYNAPSES_H
#define SYNAPSES_H

#include <iostream>

class SYNAPSE_TYPE{
public:
    virtual std::string classNick()=0;
    virtual int setData(double* _arr);
    virtual int type();
    virtual double preSpike(double _current_time)=0;
    virtual double postSpike(double _current_time)=0;
};

class NULL_SYNAPSE: public SYNAPSE_TYPE{
public:
    std::string classNick();
    double preSpike(double _current_time);
    double postSpike(double _current_time);
};

class SYNAPSE_STATIC: public SYNAPSE_TYPE{
    double weight;
    int excinh; // 0 exc, 1 inh
public:
    std::string classNick();
    int setData(double* _arr);
    int type();
    double preSpike(double _current_time);
    double postSpike(double _current_time);
};


#endif // SYNAPSES_H