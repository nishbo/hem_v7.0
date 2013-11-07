#include "nodes.h"

int NODE_TYPE::typesSynapsesSupported(){
    return 0;
}

/******************** NULL NODE ********************************************* */
// Class basically represents a node-filler that does nothing.

std::string NULL_NODE::type(){
    return "null_node";
}

int NULL_NODE::evolve(double _current_time, double _dt, double _I, double*  _syn){
    return 0;
}

/******************** NEURON IAF ******************************************** */
// A basic leaky integrate-and-fire neuron.

std::string NEURON_IAF::type(){
    return "neuron_liaf";
}

int NEURON_IAF::typesSynapsesSupported(){
    return 2;
}

NEURON_IAF::NEURON_IAF(){
    V = 0;
    Vth = 15;
    Vrest = 0;
    Vreset = 13.3;
    tau_m = 30;
    tau_ref = 2;
    C_m = 250;
    Rin = 16.66667;
    last_spiked = -tau_ref;
}

int NEURON_IAF::evolve(double _current_time, double _dt, double _I, double*  _syn){
    // if(_current_time < last_spiked + tau_ref){
    //     V = Vreset;
    // } else {
    //     V += _dt * ( - (V - Vrest) + Rin * _I) / tau_m;
    //     if(V >= Vth){ // spike
    //         V = Vreset;
    //         last_spiked = _current_time;
    //         return 1;
    //     }
    // }
    return 0;
}

