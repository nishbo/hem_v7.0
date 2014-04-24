#ifndef SYNAPSES_H
#define SYNAPSES_H

#include <iostream>
#include <cmath>
#include <vector>


class SynapseType{
public:
    virtual double weight();
    virtual std::string getClassNick()=0;
    virtual double preSpike(double currentTime);
    virtual void postSpike(double currentTime);
    virtual void setPreset(int setNumber);
    virtual void reset();
    virtual std::vector<double> data();
};


class NullSynapse: public SynapseType{
public:
    std::string getClassNick();
};


class SynapseStatic: public SynapseType{
public:
    SynapseStatic();

    std::string getClassNick();
    double preSpike(double currentTime);
    double weight();
private:
    double _weight;
};


class SynapseTM: public SynapseType{
public:
    SynapseTM();

    std::string getClassNick();
    double preSpike(double currentTime);
    double weight();
    void setPreset(int setNumber);
    void reset();
    std::vector<double> data();
private:
    double _U, _D, _F, _A;
    double _r, _u, _lastPresynapticSpike;
};


#endif // SYNAPSES_H