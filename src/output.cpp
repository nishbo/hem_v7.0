#include "output.h"

_Spike::_Spike(double _time, int _nn){
    sptime = _time;
    nn = _nn;
}

// int Output::add_output(std::string line){
//     if (! currently_printing.count(line)){
//         std::cout<<" adding new line "<< line << std::endl;
//         currently_printing.insert(line);
//     } else {
//         std::cout<<" tried to import line that existed "<< line << std::endl;
//         return 2;
//     }

//     if (line.compare("spikes") == 0){
//         spikes = fopen("data/spikes.txt", "w");
//         return 0;
//     } else if (line.compare("potentials") == 0){
//         potentials = fopen("data/potentials.txt", "w");
//         return 0;
//     }

//     return 1;
// }

// int Output::add_output(std::vector<std::string> lines){
//     int buf = 0;
//     for(std::string line : lines)
//         buf += add_output(line);
//     return buf;
// }

int Output::save_spikes_trigger(){
    // if( spikes ){
    //     spikes.close();
    //     return 0;
    // } else {
        spikes = fopen("data/spikes.txt", "w");
        return 1;
    // }
}

int Output::save_potentials_trigger(std::vector<Node*> n){
    // if( potentials ){
    //     potentials.close();
    //     return 0;
    // } else {
        potentials = fopen("data/potentials.txt", "w");
        node_array = n;
        return 1;
    // }
}

int Output::save_weights_trigger(std::vector<Synapse*> allsyn){
    // if( potentials ){
    //     potentials.close();
    //     return 0;
    // } else {
        potentials = fopen("data/potentials.txt", "w");
        // node_array = n;
        return 1;
    // }
}

Output::Output(std::vector<std::string> lines){

}

Output::~Output(){
    // if( spikes ){
    //     spikes.close();
    // }
}

int Output::push_spike(double sptime, int nn){
    _Spike buf(sptime, nn);
    spike_buffer.push_back(buf);
    return spike_buffer.size();
}
