#include "output.h"


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

// int Output::save_spikes_trigger(){
//     // if( spikes ){
//     //     spikes.close();
//     //     return 0;
//     // } else {
//         spikes = fopen("data/spikes.txt", "w");
//         return 1;
//     // }
// }

// int Output::save_potentials_trigger(std::vector<Node*> n){
//     // if( potentials ){
//     //     potentials.close();
//     //     return 0;
//     // } else {
//         potentials = fopen("data/potentials.txt", "w");
//         node_array = n;
//         return 1;
//     // }
// }

// int Output::save_weights_trigger(std::vector<Synapse*> allsyn){
//     // if( potentials ){
//     //     potentials.close();
//     //     return 0;
//     // } else {
//         potentials = fopen("data/potentials.txt", "w");
//         // node_array = n;
//         return 1;
//     // }
// }

void Output::openSpikeFile(std::string filename)
{
    _spikeFile.open(filename.c_str(), std::ofstream::out);
}

void Output::closeSpikeFile()
{
    _spikeFile.close();
}

void Output::openPotentialFile(std::string filename)
{
    _potentialFile.open(filename.c_str(), std::ofstream::out);
}

void Output::closePotentialFile()
{
    _potentialFile.close();
}

void Output::close()
{
    if (_spikeFile)
        closeSpikeFile();
    if (_potentialFile)
        closePotentialFile();
}

void Output::print()
{
    if (_spikeFile){
        for (auto& spike : _spikes){
            _spikeFile << spike.t <<" "<< spike.node_number <<"\n";
        }
        _spikes.clear();
    }
}

void Output::print(double t)
{
    print();

    if (_potentialFile){
        _potentialFile <<"Time = "<< t <<" ms\n";
        for (auto node : nodes){
            _potentialFile << node->V()<<" ";
        }
        _potentialFile <<std::endl;
    }
}

void Output::push(double t, int node_number)
{
    _spikes.push_back(Spike(t, node_number));
}
