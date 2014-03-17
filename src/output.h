#ifndef OUTPUT_H
#define OUTPUT_H

#include <set>

#include "core.h"

struct _spike{
    int nn;
    double sptime;
    _spike(double _time, int _nn);
};

class output{
    std::set<std::string> currently_printing;
    std::vector<_spike> spike_buffer;

    std::vector<NODE*> node_array;
    std::vector<SYNAPSE*> allsyn;

    FILE* spikes;
    FILE* potentials;
public:
    output(std::vector<std::string> lines);
    ~output();

    int push_spike(double time, int nn);
    // int add_output(std::string line);
    // int add_output(std::vector<std::string> lines);

    int save_spikes_trigger();
    int save_potentials_trigger(std::vector<NODE*> n);
    int save_weights_trigger(std::vector<NODE*> n);

    int print();
};


#endif // OUTPUT_H