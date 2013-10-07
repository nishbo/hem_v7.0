#ifndef NODES_H
#define NODES_H

#include <iostream>

class NODE_TYPE{
public:
    virtual int evolve(float _current_time, float _dt, float _I)=0;
    virtual std::string getType()=0;
};


class NEURON_IAF: public NODE_TYPE{
private:
    float V, Vth, Vrest, Vreset, tau_ref, tau_m, C_m, last_spiked, Rin;

public:
    NEURON_IAF();
    int evolve(float _current_time, float _dt, float _I);
    std::string getType();
};

#endif // NODES_H
