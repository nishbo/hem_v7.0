#include "nodes.h"

int NODE_TYPE::typesSynapsesSupported(){
    return 0;
}

/****************************** NULL NODE *********************************** */
// Class basically represents a node-filler that does nothing.

std::string NULL_NODE::type(){
    return "null_node";
}

int NULL_NODE::evolve(double _current_time, double _dt, double _I, double*  _syn){
    return 0;
}

/****************************** NEURON IAF ********************************** */
// A basic leaky integrate-and-fire neuron.

std::string NEURON_IAF::type(){
    return "neuron_liaf";
}

int NEURON_IAF::typesSynapsesSupported(){
    return 2;
}

NEURON_IAF::NEURON_IAF(){
    V = 0;
    V_th = 15;
    V_rest = 0;
    V_reset = 13.3;
    tau_m = 30;
    tau_ref = 2;
    C_m = 250;
    R_m = 16.66667;

    g_plus = 0;     E_rev_plus = 115;   tau_syn_plus = 3;
    g_minus = 0;    E_rev_minus = -10;  tau_syn_minus = 3;

    last_spiked = -(tau_ref+1);
}

double NEURON_IAF::synRS(double _g, double _tau_syn){
    return  - _g / _tau_syn;
}

int NEURON_IAF::evolve(double _current_time, double _dt, double _I, double*  _syn){
    if(_current_time < last_spiked + tau_ref){
        V = V_reset;
    } else {
        g_plus += _dt * synRS(g_plus, tau_syn_plus) + _syn[0];
        g_minus += _dt * synRS(g_minus, tau_syn_minus) + _syn[1];
        I_full = _I + g_plus * (E_rev_plus - V) + g_minus * (E_rev_minus - V);

        V += _dt * ( - (V - V_rest) + R_m * I_full) / tau_m;

        if(V >= V_th){ // spike
            V = V_reset;
            last_spiked = _current_time;
            return 1;
        }
    }
    return 0;
}

