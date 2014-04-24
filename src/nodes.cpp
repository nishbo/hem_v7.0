#include "nodes.h"


void NodeType::forceSpike(double currentTime)
{
    lastSpiked = currentTime;
}

void NodeType::setPreset(int setNumber)
{
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

    _V_rest = 0.0;
    _C_m = 250; // NOT USED
    _R_m = 1.0;  //16.66667;

    setPreset(0);

    lastSpiked = -(_tau_ref + 1.0);
}

void NeuronIaf::setPreset(int setNumber)
{
    switch (setNumber){
        case 0: //e
            _V_th = 15.0;
            _V_reset = 13.5;
            _tau_m = 30.0;
            _tau_ref = 2.0;
            break;
        case 1: //i
            _V_th = 15.0;
            _V_reset = 13.5;
            _tau_m = 30.0;
            _tau_ref = 3.0;
            break;
        default:
            setPreset(0);
    }

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

/****************************** Periodic Generator ************************** */
// 

std::string NodePeriodicGenerator::type()
{
    return "node_periodic_generator";
}

NodePeriodicGenerator::NodePeriodicGenerator()
{
    V = 0.0;
    _period = 10.0;

    lastSpiked = 0.0;
}

int NodePeriodicGenerator::step(double currentTime, double dt, double I)
{
    if (currentTime - lastSpiked >= _period){
        lastSpiked = currentTime;
        return 1;
    }
    return 0;
}

void NodePeriodicGenerator::forceSpike(double currentTime)
{
    lastSpiked = currentTime;
}
