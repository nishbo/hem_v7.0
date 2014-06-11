#include "nodes.h"


NodeType* chooseNodeType(std::string className)
{
    if(className.compare("leaky iaf") == 0){
        return new NeuronIaf;
    } else if(className.compare("null node") == 0) {
        return new NullNode;
    } else if(className.compare("periodic generator") == 0) {
        return new NodePeriodicGenerator;
    } else if(className.compare("poisson generator") == 0) {
        return new NodePoissonGenerator;
    } else {
        return new NullNode;
    }
}


void NodeType::forceSpike(double currentTime)
{
    lastSpiked = currentTime;
}

void NodeType::setPreset(int setNumber)
{
}

bool NodeType::isSpiking(double currentTime)
{
    return false;
}

/****************************** NULL NODE *********************************** */
// Class basically represents a node-filler that does nothing.

std::string NullNode::type()
{
    return "null node";
}

int NullNode::step(double currentTime, double dt, double I)
{
    return 0;
}

/****************************** NEURON IAF ********************************** */
// A basic leaky integrate-and-fire neuron.

std::string NeuronIaf::type()
{
    return "neuron liaf";
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
    if (not isSpiking(currentTime)){
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

bool NeuronIaf::isSpiking(double currentTime)
{
    if (currentTime < lastSpiked + _tau_ref)
        return true;
    return false;
}

/****************************** Periodic Generator ************************** */
// 

std::string NodePeriodicGenerator::type()
{
    return "periodic generator";
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

/****************************** Poisson Generator ************************** */
// 

std::string NodePoissonGenerator::type()
{
    return "poisson generator";
}

NodePoissonGenerator::NodePoissonGenerator()
{
    V = 0.0;
    _frequency = 10.0;
    _nextSpike = genran::poissonProcess(_frequency);

    lastSpiked = 0.0;
}

int NodePoissonGenerator::step(double currentTime, double dt, double I)
{
    if (currentTime >= _nextSpike){
        lastSpiked = currentTime;
        _nextSpike = currentTime + genran::poissonProcess(_frequency);
        return 1;
    }
    return 0;
}

void NodePoissonGenerator::forceSpike(double currentTime)
{
    lastSpiked = currentTime;
}
