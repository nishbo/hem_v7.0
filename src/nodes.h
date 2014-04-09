#ifndef NODES_H
#define NODES_H

#include <iostream>

class NodeType
{
public:
    double V;
    double lastSpiked;

    virtual void forceSpike(double current_time);

    virtual int step(double current_time, double dt, double I)=0;
    virtual std::string type()=0;
};


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

private:
    double _V_th, _V_rest, _V_reset, _tau_ref, _tau_m, _C_m, _R_m;
    double _last_spiked;
};

#endif // NODES_H
