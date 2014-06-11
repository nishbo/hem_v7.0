#ifndef SYNAPSES_H
#define SYNAPSES_H

#include <iostream>
#include <cmath>
#include <vector>

#include "random.h"


class SynapseType{
public:
    virtual double weight();
    virtual std::string type()=0;
    virtual SynapseType* duplicate()=0;
    virtual double preSpike(double currentTime);
    virtual void postSpike(double currentTime);
    virtual void setPreset(int setNumber);
    virtual void reset();
    virtual std::vector<double> data();
    virtual void control(int sequence);
};

SynapseType* chooseSynapseType(std::string className, std::string stdpType);

class NullSynapse: public SynapseType{
public:
    std::string type();
    SynapseType* duplicate();
};


class SynapseStatic: public SynapseType{
public:
    SynapseStatic();
    SynapseType* duplicate();

    std::string type();
    double preSpike(double currentTime);
    double weight();
private:
    double _weight;
};


class SynapseTM: public SynapseType{
public:
    SynapseTM();
    SynapseType* duplicate();

    std::string type();
    double preSpike(double currentTime);
    double weight();
    void setPreset(int setNumber);
    void reset();
    std::vector<double> data();

    void shuffle();
    void control(int sequence);
private:
    double _U, _D, _F, _A;
    double _r, _u, _lastPresynapticSpike;
};


class SynapseStdp: public SynapseType{
public:
    SynapseStdp();
    virtual SynapseType* duplicate();

    double postSpikeUnbound(double currentTime);
    double preSpikeUnbound(double currentTime);
    double weight();
    virtual void reset();
    virtual std::vector<double> data();

    virtual std::string type();
    virtual void postSpike(double currentTime);
    virtual double preSpike(double currentTime);
    virtual void setPreset(int setNumber);
protected:
    double _weight, _tauPlus, _tauMinus, _dWeightPlus, _dWeightMinus;
    double _lastPresynapticSpike, _lastPostsynapticSpike;
    double _workingTimeWindowMin, _workingTimeWindowMax;
};


class SynapseStdpAgileBoundaries: public SynapseStdp{
public:
    SynapseStdpAgileBoundaries();
    SynapseType* duplicate();

    std::string type();
    void postSpike(double currentTime);
    double preSpike(double currentTime);
    void setPreset(int setNumber);
private:
    double _weightMax, _weightMin;
};


class SynapseStdpHardBoundaries: public SynapseStdp{
public:
    SynapseStdpHardBoundaries();
    SynapseType* duplicate();

    std::string type();
    void postSpike(double currentTime);
    double preSpike(double currentTime);
    void setPreset(int setNumber);

    void control(int sequence);
private:
    double _weightMax, _weightMin;
};

SynapseType* chooseStdp(std::string stdpType);

class SynapseTmAndStdp: public SynapseType{
public:
    SynapseTmAndStdp(std::string stdpType="no boundaries");
    SynapseType* duplicate();

    std::string type();
    void postSpike(double currentTime);
    double preSpike(double currentTime);
    double weight();
    void setPreset(int setNumber);
    void reset();
    std::vector<double> data();
    void control(int sequence);
private:
    SynapseType* _stdp;
    SynapseType* _tm;
};


#endif // SYNAPSES_H