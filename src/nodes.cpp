#include "nodes.h"


void NodeType::forceSpike(double currentTime)
{
    lastSpiked = currentTime;
}

/****************************** NULL NODE *********************************** */
// Class basically represents a node-filler that does nothing.

std::string NullNode::type()
{
    return "null_node";
}

int NullNode::step(double currentTime, double dt, double I)
{
    return 0;
}

/****************************** NEURON IAF ********************************** */
// A basic leaky integrate-and-fire neuron.

std::string NeuronIaf::type()
{
    return "neuron_liaf";
}

NeuronIaf::NeuronIaf()
{
    V = 0.0;

    _V_th = 15.0;
    _V_rest = 0.0;
    _V_reset = 14.2;
    _tau_m = 30.0;
    _tau_ref = 2.0;
    _C_m = 250;
    _R_m = 1.0;  //16.66667;

    lastSpiked = -(_tau_ref + 1.0);
}

int NeuronIaf::step(double currentTime, double dt, double I)
{
    if (currentTime >= lastSpiked + _tau_ref){
        V += dt * ( - (V - _V_rest) + _R_m * I) / _tau_m;

        if (V >= _V_th){  // spike
            V = _V_reset;
            lastSpiked = currentTime;
            return 1;
        }
    }
    return 0;
}

void NeuronIaf::forceSpike(double currentTime)
{
    V = _V_reset;
    lastSpiked = currentTime;
}
