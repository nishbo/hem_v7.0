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
    _V_reset = 13.3;
    _tau_m = 30.0;
    _tau_ref = 2.0;
    _C_m = 250;
    _R_m = 16.66667;

    _g_plus = 0.0;  _E_rev_plus = 115.0;    _tau_syn_plus = 3.0;
    _g_minus = 0.0; _E_rev_minus = -10.0;   _tau_syn_minus = 3.0;

    lastSpiked = -(_tau_ref + 1.0);
}

// double NeuronIaf::_synRS(double g, double tau_syn)
// {
//     return  - g / tau_syn;
// }

int NeuronIaf::step(double currentTime, double dt, double I)
{
    if (currentTime >= lastSpiked + _tau_ref){
        // _g_plus += dt * _synRS(_g_plus, _tau_syn_plus) + syn[0];
        // _g_minus += dt * _synRS(_g_minus, _tau_syn_minus) + syn[1];
        // I_full = I + _g_plus * (_E_rev_plus - V) + _g_minus * (_E_rev_minus - V);

        V += dt * ( - (V - _V_rest) + _R_m * I) / _tau_m;

        if (V >= _V_th){ // spike
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
