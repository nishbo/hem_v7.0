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

std::string NullSynapse::type()
{
    return "null synapse";
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

std::string SynapseStatic::type()
{
    return "static synapse";
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

std::string SynapseTM::type()
{
    return "tsodyks-markram synapse";
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

/******************** SYNAPSE STDP ****************************************** */
// 
SynapseStdp::SynapseStdp()
{
    setPreset(0);
    reset();
}

void SynapseStdp::reset()
{
    _lastPresynapticSpike = -1000.0;
    _lastPostsynapticSpike = -1000.0;
    _weight = 0.5;
}

std::string SynapseStdp::type()
{
    return "stdp synapse unbound";
}

double SynapseStdp::preSpikeUnbound(double currentTime)
{
    _lastPresynapticSpike = currentTime;
    double dw = 0.0;
    if (_dWeightMinus > 0.0){
        double h = _lastPostsynapticSpike - _lastPresynapticSpike;
        if (-_workingTimeWindowMax <= h <= -_workingTimeWindowMin)
            dw =  _dWeightMinus * exp(h / _tauMinus);
    }
    return dw;
}

double SynapseStdp::postSpikeUnbound(double currentTime)
{
    _lastPostsynapticSpike = currentTime;
    double dw = 0.0;
    if (_dWeightPlus > 0.0){
        double h = _lastPostsynapticSpike - _lastPresynapticSpike;
        if (_workingTimeWindowMin <= h <= _workingTimeWindowMax)
            dw = _dWeightPlus * exp(h / _tauPlus);
    }
    return dw;
}

double SynapseStdp::preSpike(double currentTime)
{
    return _weight - preSpikeUnbound(currentTime);
}

void SynapseStdp::postSpike(double currentTime)
{
    _weight = _weight + postSpikeUnbound(currentTime);
}

double SynapseStdp::weight()
{
    return _weight;
}

void SynapseStdp::setPreset(int setNumber)
{
    switch (setNumber){
        case 0: //e
            _tauPlus = 20.0;
            _tauMinus = 20.0;
            _dWeightPlus = 0.3;
            _dWeightMinus = 0.3105;
            _workingTimeWindowMin = 2.0;
            _workingTimeWindowMax =  60.0;
            break;
        case 1: //i
            _tauPlus = 20.0;
            _tauMinus = 20.0;
            // No changes occure on spike:
            _dWeightPlus = 0.0;
            _dWeightMinus = 0.0;
            // These values do not really matter:
            _workingTimeWindowMin = 20.0;
            _workingTimeWindowMax =  60.0;
            break;
        default:
            setPreset(0);
    }
}

std::vector<double> SynapseStdp::data()
{
    std::vector<double> v;
    v.push_back(_weight);
    return v;
}


/******************** SYNAPSE STDP AGILE BOUNDARIES ************************* */
// 
SynapseStdpAgileBoundaries::SynapseStdpAgileBoundaries()
{
    setPreset(0);
    reset();
}

std::string SynapseStdpAgileBoundaries::type()
{
    return "stdp synapse agile boundaries";
}

double SynapseStdpAgileBoundaries::preSpike(double currentTime)
{
    _weight -= preSpikeUnbound(currentTime) * _weight;
    if (_weight < _weightMin)
        _weight = _weightMin;
    return _weight;
}

void SynapseStdpAgileBoundaries::postSpike(double currentTime)
{
    _weight += postSpikeUnbound(currentTime) * (1.0 - _weight);
    if (_weight > _weightMax)
        _weight = _weightMax;
}

void SynapseStdpAgileBoundaries::setPreset(int setNumber)
{
    switch (setNumber){
        case 0: //e
            _weightMax = 1.0;
            _weightMin = 0.0;

            _tauPlus = 20.0;
            _tauMinus = 30.0;
            _dWeightPlus = 0.3;
            _dWeightMinus = 0.2;
            _workingTimeWindowMin = 0.0;
            _workingTimeWindowMax =  100000.0;
            break;
        case 1: //i
            _weightMax = 1.0;
            _weightMin = 0.0;

            _tauPlus = 20.0;
            _tauMinus = 30.0;
            // No changes occure on spike:
            _dWeightPlus = 0.0;
            _dWeightMinus = 0.0;
            // These values do not really matter:
            _workingTimeWindowMin = 20.0;
            _workingTimeWindowMax =  60.0;
            break;
        default:
            setPreset(0);
    }
}


/******************** SYNAPSE STDP HARD BOUNDARIES ************************** */
// 
SynapseStdpHardBoundaries::SynapseStdpHardBoundaries()
{
    setPreset(0);
    reset();
}

std::string SynapseStdpHardBoundaries::type()
{
    return "stdp synapse hard boundaries";
}

double SynapseStdpHardBoundaries::preSpike(double currentTime)
{
    _weight -= preSpikeUnbound(currentTime);
    if (_weight < _weightMin)
        _weight = _weightMin;
    return _weight;
}

void SynapseStdpHardBoundaries::postSpike(double currentTime)
{
    _weight += postSpikeUnbound(currentTime);
    if (_weight > _weightMax)
        _weight = _weightMax;
}

void SynapseStdpHardBoundaries::setPreset(int setNumber)
{
    switch (setNumber){
        case 0: //e
            _weightMax = 1.0;
            _weightMin = 0.0;

            _tauPlus = 20.0;
            _tauMinus = 20.0;
            _dWeightPlus = 0.3;
            _dWeightMinus = 0.3105;
            _workingTimeWindowMin = 2.0;
            _workingTimeWindowMax =  60.0;
            break;
        case 1: //i
            _weightMax = 1.0;
            _weightMin = 0.0;

            _tauPlus = 20.0;
            _tauMinus = 20.0;
            // No changes occure on spike:
            _dWeightPlus = 0.0;
            _dWeightMinus = 0.0;
            // These values do not really matter:
            _workingTimeWindowMin = 2.0;
            _workingTimeWindowMax =  60.0;
            break;
        default:
            setPreset(0);
    }
}

/******************* CHOOSE STDP ******************************************** */
SynapseType* chooseStdp(std::string stdpType)
{
    if(stdpType.compare("no boundaries") == 0){
        return new SynapseStdp;
    } else if(stdpType.compare("agile boundaries") == 0) {
        return new SynapseStdpAgileBoundaries;
    } else if(stdpType.compare("hard boundaries") == 0) {
        return new SynapseStdpHardBoundaries;
    } else {
        return new SynapseStdp;
    }
}

/******************** SYNAPSE TM AND STDP *********************************** */
// 

SynapseTmAndStdp::SynapseTmAndStdp(std::string stdpType)
{
    _tm = SynapseTM();
    _stdp = chooseStdp(stdpType);
}

void SynapseTmAndStdp::reset()
{
    _tm.reset();
    _stdp->reset();
}

std::string SynapseTmAndStdp::type()
{
    return _tm.type() + " and " + _stdp->type();
}

double SynapseTmAndStdp::preSpike(double currentTime)
{
    return _tm.preSpike(currentTime) * _stdp->preSpike(currentTime);
}

void SynapseTmAndStdp::postSpike(double currentTime)
{
    _tm.postSpike(currentTime);
    _stdp->postSpike(currentTime);
}

double SynapseTmAndStdp::weight()
{
    return _stdp->weight();
}

void SynapseTmAndStdp::setPreset(int setNumber)
{
    _tm.setPreset(setNumber);
    _stdp->setPreset(int(setNumber / 2));
}

std::vector<double> SynapseTmAndStdp::data()
{
    std::vector<double> v = _tm.data();
    std::vector<double> v2 = _stdp->data();
    v.reserve(v.size() + v2.size());
    v.insert(v.end(), v2.begin(), v2.end());
    return v;
}
