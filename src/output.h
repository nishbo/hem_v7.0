#ifndef OUTPUT_H
#define OUTPUT_H

#include <set>
#include <cstdio>

#include "core.h"

struct _Spike{
    int nn;
    double sptime;
    _Spike(double _time, int _nn);
};

class Output{
public:
    Output(std::vector<std::string> lines);
    ~Output();

    int push_spike(double time, int nn);
    // int add_output(std::string line);
    // int add_output(std::vector<std::string> lines);

    int save_spikes_trigger();
    int save_potentials_trigger(std::vector<Node*> n);
    int save_weights_trigger(std::vector<Synapse*> n);

    int print();

private:
    std::set<std::string> currently_printing;
    std::vector<_Spike> spike_buffer;

    std::vector<Node*> node_array;
    std::vector<Synapse*> allsyn;

    FILE* spikes;
    FILE* potentials;
};


#endif // OUTPUT_H