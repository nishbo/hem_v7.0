#ifndef NODES_H
#define NODES_H

#include <iostream>

class NODE_TYPE{
public:
    virtual int typesSynapsesSupported();
    virtual int evolve(double _current_time, double _dt, double _I, double*  _syn)=0;
    virtual std::string type()=0;
};


class NULL_NODE: public NODE_TYPE{
public:
    int evolve(double _current_time, double _dt, double _I, double*  _syn);
    std::string type();
};


class NEURON_IAF: public NODE_TYPE{
private:
    double V, Vth, Vrest, Vreset, tau_ref, tau_m, C_m, last_spiked, Rin;

public:
    NEURON_IAF();
    int typesSynapsesSupported();
    int evolve(double _current_time, double _dt, double _I, double*  _syn);
    std::string type();
};

#endif // NODES_H
