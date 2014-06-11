#include "synapses.h"


SynapseType* chooseSynapseType(std::string className, std::string stdpType)
{
    if (className.compare("static") == 0){
        return new SynapseStatic;
    } else if(className.compare("null synapse") == 0){
        return new NullSynapse;
    } else if(className.compare("tsodyks-markram") == 0){
        return new SynapseTM;
    } else if(className.compare("stdp") == 0){
        return chooseStdp(stdpType);
    } else if(className.compare("stdp and tm") == 0){
        return new SynapseTmAndStdp(stdpType);
    } else {
        return new NullSynapse;
    }
}


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

void SynapseType::control(int sequence)
{
}

/******************** NULL SYNAPSE ****************************************** */
// Class basically represents a synapse-filler that does nothing.

std::string NullSynapse::type()
{
    return "null synapse";
}

SynapseType* NullSynapse::duplicate()
{
    return new NullSynapse;
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

SynapseType* SynapseStatic::duplicate()
{
    SynapseStatic* synapse = new SynapseStatic;
    synapse->_weight = _weight;
    return synapse;
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
    _r = 1.0; // REMOVED OVERBURST AT THE BEGINNING
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

void SynapseTM::shuffle()
{
    _A = genran::gauss(_A, _A * 0.1, 0.0, 10.0 * _A);
    _U = genran::gauss(_U, _U * 0.1, 0.0, 1.0);
    _D = genran::gauss(_D, _D * 0.1, 0.0, 10.0 * _D);
    _F = genran::gauss(_F, _F * 0.1, 0.0, 10.0 * _F);
}

void SynapseTM::control(int sequence)
{
    switch (sequence){
        case 0:
            shuffle();
            _A = 1.0;
            break;
        case 1:
            break;
        case 2:
            shuffle();
            break;
    }
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
            exit(1102);
    }
}

std::vector<double> SynapseTM::data()
{
    std::vector<double> v;
    v.push_back(_r);
    v.push_back(_u);
    return v;
}

SynapseType* SynapseTM::duplicate()
{
    SynapseTM* synapse = new SynapseTM;
    synapse->_A = _A;
    synapse->_D = _D;
    synapse->_F = _F;
    synapse->_U = _U;
    synapse->_lastPresynapticSpike = _lastPresynapticSpike;
    synapse->_r = _r;
    synapse->_u = _u;

    return synapse;
}

/******************** SYNAPSE STDP ****************************************** */
// 
SynapseStdp::SynapseStdp()
{
    _weight = 0.5;
    setPreset(0);
    reset();
}

void SynapseStdp::reset()
{
    _lastPresynapticSpike = -1000.0;
    _lastPostsynapticSpike = -1000.0;
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
        case 2: //i
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
            exit(1102);
    }
}

std::vector<double> SynapseStdp::data()
{
    std::vector<double> v;
    v.push_back(_weight);
    return v;
}

SynapseType* SynapseStdp::duplicate()
{
    SynapseStdp* synapse = new SynapseStdp;
    synapse->_tauPlus = _tauPlus;
    synapse->_tauMinus = _tauMinus;
    synapse->_dWeightPlus = _dWeightPlus;
    synapse->_dWeightMinus = _dWeightMinus;
    synapse->_workingTimeWindowMin = _workingTimeWindowMin;
    synapse->_workingTimeWindowMax = _workingTimeWindowMax;
    synapse->_weight = _weight;
    synapse->_lastPostsynapticSpike = _lastPostsynapticSpike;
    synapse->_lastPresynapticSpike = _lastPresynapticSpike;
    
    return synapse;
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
        case 2: //i
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
            exit(1102);
    }
}

SynapseType* SynapseStdpAgileBoundaries::duplicate()
{
    SynapseStdpAgileBoundaries* synapse = new SynapseStdpAgileBoundaries;
    synapse->_tauPlus = _tauPlus;
    synapse->_tauMinus = _tauMinus;
    synapse->_dWeightPlus = _dWeightPlus;
    synapse->_dWeightMinus = _dWeightMinus;
    synapse->_workingTimeWindowMin = _workingTimeWindowMin;
    synapse->_workingTimeWindowMax = _workingTimeWindowMax;
    synapse->_weight = _weight;
    synapse->_lastPostsynapticSpike = _lastPostsynapticSpike;
    synapse->_lastPresynapticSpike = _lastPresynapticSpike;
    synapse->_weightMin = _weightMin;
    synapse->_weightMax = _weightMax;
    
    return synapse;
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
        case 2: //i
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
            exit(1102);
    }
}

SynapseType* SynapseStdpHardBoundaries::duplicate()
{
    SynapseStdpHardBoundaries* synapse = new SynapseStdpHardBoundaries;
    synapse->_tauPlus = _tauPlus;
    synapse->_tauMinus = _tauMinus;
    synapse->_dWeightPlus = _dWeightPlus;
    synapse->_dWeightMinus = _dWeightMinus;
    synapse->_workingTimeWindowMin = _workingTimeWindowMin;
    synapse->_workingTimeWindowMax = _workingTimeWindowMax;
    synapse->_weight = _weight;
    synapse->_lastPostsynapticSpike = _lastPostsynapticSpike;
    synapse->_lastPresynapticSpike = _lastPresynapticSpike;
    synapse->_weightMin = _weightMin;
    synapse->_weightMax = _weightMax;
    
    return synapse;
}

void SynapseStdpHardBoundaries::control(int sequence)
{
    switch (sequence){
        case 0:
            _weightMax = genran::normalVariate(54.0, 10.8, 21.6, 86.4);
            if (rand() % 2)
                _weight = _weightMax;
            else
                _weight = 0.0;
            _dWeightPlus = 0.0;
            _dWeightMinus = 0.0;
            break;
        case 1:
            _weight = genran::gammaVariate(90.0/7.0, 0.7);

            _dWeightPlus = 0.3;
            _dWeightMinus = 0.3105;
            break;
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
    _tm = new SynapseTM;
    _stdp = chooseStdp(stdpType);
}

void SynapseTmAndStdp::reset()
{
    _tm->reset();
    _stdp->reset();
}

std::string SynapseTmAndStdp::type()
{
    return _tm->type() + " and " + _stdp->type();
}

double SynapseTmAndStdp::preSpike(double currentTime)
{
    return _tm->preSpike(currentTime) * _stdp->preSpike(currentTime);
}

void SynapseTmAndStdp::postSpike(double currentTime)
{
    _tm->postSpike(currentTime);
    _stdp->postSpike(currentTime);
}

double SynapseTmAndStdp::weight()
{
    return _stdp->weight();
}

void SynapseTmAndStdp::setPreset(int setNumber)
{
    _tm->setPreset(setNumber);
    _stdp->setPreset(setNumber);
}

std::vector<double> SynapseTmAndStdp::data()
{
    std::vector<double> v = _tm->data();
    std::vector<double> v2 = _stdp->data();
    v.reserve(v.size() + v2.size());
    v.insert(v.end(), v2.begin(), v2.end());
    return v;
}

SynapseType* SynapseTmAndStdp::duplicate()
{
    SynapseTmAndStdp* synapse = new SynapseTmAndStdp;
    synapse->_tm = _tm->duplicate();
    synapse->_stdp = _stdp->duplicate();
    
    return synapse;
}

void SynapseTmAndStdp::control(int sequence)
{
    _tm->control(sequence);
    _stdp->control(sequence);
}
