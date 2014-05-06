#include "output.h"

void Output::open()
{
    openSpikeFile();
    openPotentialFile();
    openSynapticWeightFile();
    openSynapticDataFile();
    openSynapticCurrentFile();
}

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

void Output::openSynapticCurrentFile(std::string filename)
{
    _synapticCurrentFile.open(filename.c_str(), std::ofstream::out);
}

void Output::closeSynapticCurrentFile()
{
    _synapticCurrentFile.close();
}

void Output::openSynapticWeightFile(std::string filename)
{
    _synapticWeightFile.open(filename.c_str(), std::ofstream::out);
}

void Output::closeSynapticWeightFile()
{
    _synapticWeightFile.close();
}

void Output::openSynapticDataFile(std::string filename)
{
    _synapticDataFile.open(filename.c_str(), std::ofstream::out);
}

void Output::closeSynapticDataFile()
{
    _synapticDataFile.close();
}

void Output::close()
{
    if (_spikeFile.is_open())
        closeSpikeFile();
    if (_potentialFile.is_open())
        closePotentialFile();
    if (_synapticWeightFile.is_open())
        closeSynapticWeightFile();
    if (_synapticDataFile.is_open())
        closeSynapticDataFile();
    if (_synapticCurrentFile.is_open())
        closeSynapticCurrentFile();
}

void Output::print()
{
    if (_spikeFile.is_open()){
        for (auto& spike : _spikes){
            _spikeFile << spike.t <<" "<< spike.node_number <<"\n";
        }
        _spikes.clear();
    }
}

void Output::printPotentials(double t)
{
    if (_potentialFile.is_open()){
        _potentialFile <<"Time = "<< t <<" ms\n";
        for (auto node : nodes){
            _potentialFile << node->V()<<" ";
        }
        _potentialFile <<std::endl;
    }
}

void Output::printSynapticCurrents(double t)
{
    if (_synapticCurrentFile.is_open()){
        _synapticCurrentFile <<"Time = "<< t <<" ms\n";
        for (auto node : nodes){
            _synapticCurrentFile << node->I_full - node->I_stim<<" ";
        }
        _synapticCurrentFile <<std::endl;
    }
}

void Output::printSynapticWeights(double t)
{
    if (_synapticWeightFile.is_open()){
        _synapticWeightFile <<"Time = "<< t <<" ms\n";
        for (auto synapse : synapses){
            _synapticWeightFile << synapse->weight()<<" ";
        }
        _synapticWeightFile <<std::endl;
    }
}

void Output::printSynapticData(double t)
{
    if (_synapticDataFile.is_open()){
        _synapticDataFile <<"Time = "<< t <<" ms\n";
        for (auto synapse : synapses){
            for (auto& i : synapse->data()){
                _synapticDataFile << i << " ";
            }
            _synapticDataFile << "\n";
        }
        _synapticDataFile <<std::endl;
    }
}

void Output::print(double t)
{
    print();
    printPotentials(t);
    printSynapticWeights(t);
    printSynapticData(t);
    printSynapticCurrents(t);
}

void Output::push(double t, int node_number)
{
    _spikes.push_back(Spike(t, node_number));
}

void Output::printEssentialData(double length, std::string filename)
{
    std::ofstream f;
    f.open(filename.c_str(), std::ofstream::out);
    f<<"Amount_of_neurons "<< nodes.size()<<std::endl;
    f<<"Amount_of_synapses "<< synapses.size()<<std::endl;
    f<<"Timelength_of_simulation "<< length <<std::endl;
    f<<"dt "<< Node::dt <<std::endl;
    f<<"Sample_node_type "<< nodes.at(0)->type() <<std::endl;
    f<<"Sample_synapse_type "<< synapses.at(0)->type() <<std::endl;
    f.close();
}

