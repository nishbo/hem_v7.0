#include "core.h"


/******************************* Node *************************************** */
double Node::dt = 0.1;

Node::Node(std::string class_name)
{
    if(class_name.compare("leaky_iaf") == 0){
        _nodeEssentials = new NeuronIaf;
    } else if(class_name.compare("null_node") == 0) {
        _nodeEssentials = new NullNode;
    } else if(class_name.compare("periodic_generator") == 0) {
        _nodeEssentials = new NodePeriodicGenerator;
    } else {
        _nodeEssentials = new NullNode;
    }

    I_stim = 0.0;
    I_full = 0.0;
}

void Node::setPreset(int setNumber)
{
    _nodeEssentials->setPreset(setNumber);
}

double Node::V(){
    return _nodeEssentials->V;
}

void Node::initialiseSpikeBuffer(double maxDelay)
{
    _incSpikes.init(dt, maxDelay, _psWaves.size());
}

void Node::addOutgoingSynapse(Synapse* synapse)
{
    _outList.push_back(synapse);
}

void Node::addIncomingSynapse(Synapse* synapse)
{
    _incList.push_back(synapse);
}

void Node::addPsWaveType(PsWave newWave, std::string base, double modifier){
    _psWaves.push_back(newWave);
    _psBases.push_back(base);
    _psModifiers.push_back(modifier);
}

double Node::step(double current_time){
    std::vector<double> v = _incSpikes.pull();
    I_full = I_stim;
    for (unsigned i=0; i < v.size(); i++){
        if (_psBases[i].compare("current") == 0){
            I_full += _psModifiers[i] * _psWaves[i].step(dt, v[i]);
        } else if (_psBases[i].compare("conductance") == 0){
            I_full += _psWaves[i].step(dt, v[i]) * \
                (_psModifiers[i] - _nodeEssentials->V);
        }
    }

    int sp = _nodeEssentials->step(current_time, dt, I_full);

    if (sp){
        for (auto synapse : _outList)
            synapse->preSpike(current_time);
        for (auto synapse : _incList)
            synapse->postSpike(current_time);
    }

    return sp;
}

void Node::addSpike(double delay, double weight, int waveType){
    _incSpikes.push(delay, weight, waveType);
}

void Node::announceConnections()
{
    std::cout<<"\nIncoming synapses\n";
    for (auto synapse : _incList){
        std::cout<< synapse<<"   ";
    }
    std::cout<<"\nOutgoing synapses\n";
    for (auto synapse : _outList){
        std::cout<< synapse<<"   ";
    }
}

/******************************* PsWave ************************************* */
PsWave::PsWave(double tau1, double tau2)
{
    // using namespace std::placeholders;
    // auto step = std::bind (_stepExponential, _1, _2)
    if (tau1 > 0.0 and tau2 > 0.0){
        if (tau1 == tau2)
            _type = 2;
        else
            _type = 1;
        _tau1 = tau1;
        _tau2 = tau2;
    } else if (tau1 >= 0.0){
        _type = 0;
        _tau1 = tau1;
        _tau2 = 0.0;
    } else if (tau2 >= 0.0){
        _type = 0;
        _tau1 = tau2;
        _tau2 = 0.0;
    } else {
        exit(12);
    }
    _g = 0.0;
    _g_dif = 0.0;
}

double PsWave::step(double dt, double weight)
{
    switch (_type){
    case 0:
        return _stepExponential(dt, weight);
        break;
    case 1:
        return _stepDoubleExponential(dt, weight);
        break;
    case 2:
        return _stepAlpha(dt, weight);
        break;
    default:
    exit(123);
    }
}

double PsWave::_stepExponential(double dt, double weight)
{
    _g += weight;
    _g -= dt * _g / _tau1;
    return _g;
}

double PsWave::_stepAlpha(double dt, double weight)
{// NOT WORKING
    _g += weight;
    _g -= dt * _g / _tau1;
    return 0.0;
}

double PsWave::_stepDoubleExponential(double dt, double weight)
{// NOT WORKING
    _g += weight;
    _g -= dt * _g / _tau1;
    return 0.0;
}


/******************************* Synapse ************************************ */
Synapse::Synapse(std::string className, Node* postNode)
{
    _postNode = postNode;

    delay = _postNode->dt;
    waveType = 0;
    
    if (className.compare("static") == 0){
        _synapseEssentials = new SynapseStatic;
    } else if(className.compare("null_node") == 0){
        _synapseEssentials = new NullSynapse;
    } else if(className.compare("tsodyks_markram") == 0){
        _synapseEssentials = new SynapseTM;
    } else if(className.compare("stdp") == 0){
        _synapseEssentials = new SynapseStdp;
    } else if(className.compare("stdp_and_tm") == 0){
        _synapseEssentials = new SynapseTmAndStdp;
    } else {
        _synapseEssentials = new NullSynapse;
    }
}

void Synapse::preSpike(double currentTime)
{
    _postNode->addSpike(delay, _synapseEssentials->preSpike(currentTime), \
        waveType);
}

void Synapse::postSpike(double currentTime)
{
    _synapseEssentials->postSpike(currentTime);
}

double Synapse::weight()
{
    return _synapseEssentials->weight();
}

void Synapse::setPreset(int setNumber)
{
    _synapseEssentials->setPreset(setNumber);
}

void Synapse::reset()
{
    _synapseEssentials->reset();
}

std::vector<double> Synapse::data()
{
    return _synapseEssentials->data();
}


/******************************* Spike ************************************** */
Spike::Spike(double spt, int spnode_number)
{
    t = spt;
    node_number = spnode_number;
}