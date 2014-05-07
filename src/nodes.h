#ifndef NODES_H
#define NODES_H

#include <iostream>

#include "random.h"

class NodeType
{
public:
    double V;
    double lastSpiked;

    virtual void forceSpike(double current_time);
    virtual void setPreset(int setNumber);

    virtual int step(double current_time, double dt, double I)=0;
    virtual std::string type()=0;
};


NodeType* chooseNodeType(std::string className);


class NullNode: public NodeType
{
public:
    int step(double current_time, double dt, double I);
    std::string type();
};


class NeuronIaf: public NodeType
{
public:
    NeuronIaf();
    int step(double current_time, double dt, double I);
    void forceSpike(double current_time);
    std::string type();
    void setPreset(int setNumber);

private:
    double _V_th, _V_rest, _V_reset, _tau_ref, _tau_m, _C_m, _R_m;
};


class NodePeriodicGenerator: public NodeType
{
public:
    NodePeriodicGenerator();
    int step(double current_time, double dt, double I);
    void forceSpike(double current_time);
    std::string type();

private:
    double _period;
};


class NodePoissonGenerator: public NodeType
{
public:
    NodePoissonGenerator();
    int step(double current_time, double dt, double I);
    void forceSpike(double current_time);
    std::string type();

private:
    double _frequency, _nextSpike;
};

#endif // NODES_H
