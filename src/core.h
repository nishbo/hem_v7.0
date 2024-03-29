#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <string>
#include <vector>
// #include <functional>

#include "nodes.h"
#include "synapses.h"
#include "cyctimbuf.h"

class Node;
class Synapse;
class PsWave;

class Node{
public:
    static double dt;
    
    double I_stim;
    double I_full;

    double V();
    void setPreset(int setNumber);
    std::string type();
    bool isSpiking(double currentTime);
    void forceSpike(double currentTime);

    Node(std::string className);
    void initialiseSpikeBuffer(double maxDelay);
    void addOutgoingSynapse(Synapse* synapse); //add an outgoing synapse
    void addIncomingSynapse(Synapse* synapse); //add an incoming synapse

    void addPsWaveType(PsWave newWave, std::string base, double modifier);

    double step(double current_time); //node evolves for dt
    void addSpike(double delay, double weight, int waveType = 0);

    void announceConnections();

private:
    NodeType* _nodeEssentials;        //essence of the node - defines it behavior
    std::vector<Synapse *> _outList; // list of outgoing synapses
    std::vector<Synapse *> _incList; // list of incoming synapses
    
    std::vector<PsWave> _psWaves;
    std::vector<std::string> _psBases;
    std::vector<double> _psModifiers;

    WideCyclingTimeBuffer _incSpikes; // incoming spikes are stored here

    Node(); // You do not want to create just a node, define it's type.
};


class PsWave
{
public:
    PsWave(double tau1, double tau2=0.0);
    double step(double dt, double weight);
private:
    int _type;
    double _g;
    double _g_dif;
    double _tau1, _tau2;

    double _stepExponential(double dt, double weight);
    double _stepAlpha(double dt, double weight);
    double _stepDoubleExponential(double dt, double weight);    
};


class Synapse{
public:
    Synapse(std::string className, Node* postNode,\
            std::string stdpType="no boundaries");
    void preSpike(double currentTime); //signal of presynaptic spike
    void postSpike(double currentTime); //signal of postsynaptic spike

    double weight();

    void setPreset(int setNumber);
    void reset();
    std::string type();
    void control(int sequence);

    std::vector<double> data();

    Synapse* duplicate();
    Synapse* duplicate(Node* postNode);

    int waveType;
    double delay;
private:
    Node* _postNode;  // postsynaptic neuron
    SynapseType* _synapseEssentials;  // essence of the synapse - defines it evolve

    Synapse(); // You do not want to create just a synapse! define it's type!
};


struct Spike{
    int node_number;
    double t;
    Spike(double spt, int spnode_number);
};

#endif // CORE_H
