#include "synapses.h"


double SynapseType::preSpike(double currentTime)
{
    return 0.0;
}

void SynapseType::postSpike(double currentTime)
{
}

double SynapseType::weight()
{
    return 0.0;
}

void SynapseType::setPreset(int setNumber)
{
}

void SynapseType::reset()
{
}

std::vector<double> SynapseType::data()
{
    return std::vector<double>();
}

/******************** NULL SYNAPSE ****************************************** */
// Class basically represents a synapse-filler that does nothing.

std::string NullSynapse::getClassNick()
{
    return "null_synapse";
}

/******************** SYNAPSE STATIC **************************************** */
// A static synapse that does not change weight.

SynapseStatic::SynapseStatic()
{
    _weight = 1.0; //2.0 * ((double) rand() / RAND_MAX);
}

double SynapseStatic::weight()
{
    return _weight;
}

std::string SynapseStatic::getClassNick()
{
    return "static_synapse";
}

double SynapseStatic::preSpike(double currentTime)
{
    return _weight; // = 2.0 * ((double) rand() / RAND_MAX);
}

/******************** SYNAPSE TSODYKS-MARKRAM CORRECTED MODEL *************** */
// 
SynapseTM::SynapseTM()
{
    setPreset(0);
    reset();
}

void SynapseTM::reset()
{
    _lastPresynapticSpike = -(_D + 10000.0) * (_F + 10000.0);
    _u = _U;
    _r = 0.01;//1.0; // REMOVED OVERBURST AT THE BEGINNING
}

std::string SynapseTM::getClassNick()
{
    return "tsodyks-markram_synapse";
}

double SynapseTM::preSpike(double currentTime)
{
        double h = currentTime - _lastPresynapticSpike;
        double u_buf = _u;
        double r_buf = _r;
        _u = _U + u_buf * (1.0 - _U) * exp(-h/_F);
        _r = 1.0 + (r_buf - u_buf*r_buf - 1.0) * exp(-h/_D);
        _lastPresynapticSpike = currentTime;

    return _A * _r * _u;
}

double SynapseTM::weight()
{
    return _A * _u * _r;
}

void SynapseTM::setPreset(int setNumber)
{
    switch (setNumber){
        case 0: //ee
            _U = 0.5;
            _D = 1100.0;
            _F = 50.0;
            _A = 3.8;
            break;
        case 1: //ei
            _U = 0.5;
            _D = 1100.0;
            _F = 50.0;
            _A = 5.4;
            break;
        case 2: //ie
            _U = 0.25;
            _D = 700.0;
            _F = 20.0;
            _A = 7.2;
            break;
        case 3: //ii
            _U = 0.25;
            _D = 700.0;
            _F = 20.0;
            _A = 7.2;
            break;
        default:
            setPreset(0);
    }
}

std::vector<double> SynapseTM::data()
{
    std::vector<double> v;
    v.push_back(_r);
    v.push_back(_u);
    return v;
}
