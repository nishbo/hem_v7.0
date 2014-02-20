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
    double V, V_th, V_rest, V_reset, tau_ref, tau_m, C_m, last_spiked, R_m;
    double E_rev_plus, E_rev_minus, tau_syn_plus, tau_syn_minus, g_plus, g_minus;
    double I_full;

public:
    NEURON_IAF();
    double synRS(double _g, double _tau_syn);
    int typesSynapsesSupported();
    int evolve(double _current_time, double _dt, double _I, double*  _syn);
    std::string type();
};

#endif // NODES_H
