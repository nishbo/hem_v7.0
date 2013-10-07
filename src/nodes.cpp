#include "nodes.h"

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

int NEURON_IAF::evolve(float _current_time, float _dt, float _I){
    if(_current_time < last_spiked + tau_ref){
        V = Vreset;
    } else {
        V += _dt * ( - (V - Vrest) + Rin * _I) / tau_m;
        if(V >= Vth){ // spike
            V = Vreset;
            last_spiked = _current_time;
            return 1;
        }
    }
    return 0;
}

std::string NEURON_IAF::getType(){
    return "neuron_liaf";
}

