#include <cstdio>
#include <cstdlib>
#include <iostream>

/*********************************** */
class LIST;
class NODE;
class SYNAPSE;
/***************************** CLASSES ************************************** */

class NODE_TYPE{
public:
    virtual int evolve(double _current_time)=0;
    virtual std::string getType()=0;
};

class NEURON_IAF: public NODE_TYPE{
    double V, Vth, Vrest, Vreset, tau_ref, tau_m, C_m;

public:
    NEURON_IAF();
    int evolve(double _current_time);
    std::string getType();
};

class NODE{
    NODE_TYPE* node_ess;
    double* inc_spikes; //buffer for incoming spikes
    int length_of_buffer; //length of spike buffer
    static LIST* runner; //runner to go through LIST lists
    LIST* out_list; // list of outgoing synapses
public:
    static double dt; //needed for buffer
    static double max_delay; //maximum of delays of incoming synapses

    NODE(std::string _type);
    double evolve(double _current_time); //node evolves for dt
    int addSynapse(SYNAPSE* _synapse); //add an outgoing synapse for this node
    int addSpike(double _weight, double _time); //spike arrives after _time with _weight amount
};
LIST* NODE::runner = NULL;
double NODE::dt = 0.1;
double NODE::max_delay = 10;
/*********************************** */

class SYNAPSE{
    double weight;
    double delay;

    NODE* postneu; //postsynaptic neuron
public:
    SYNAPSE(NODE* _neuron);
    double preSpike(); //signal of presynaptic spike
    double postSpike(); //signal of postsynaptic spike
};
/*********************************** */

class LIST{
//singly linked list of synapses. used in nodes and main
public:
    LIST(SYNAPSE* _synapse, LIST* _oh);
    SYNAPSE* syn; //some synapse
    LIST* next; // next in line
};
/************************ FUNCTIONS ***************************************** */

NODE::NODE(std::string _type){
    out_list = NULL;
    length_of_buffer = max_delay / dt + 1;
    inc_spikes = new double [length_of_buffer];
    for (int i = 0; i < length_of_buffer; i++)
        inc_spikes[i] = 0;
    switch (_type) {
    case "neuron_liaf":
        node_ess = new NEURON_IAF;
    default:
        node_ess = NULL;
    }

}

int NODE::addSynapse (SYNAPSE* _synapse){
    out_list = new LIST (_synapse, out_list);
    return 0;
}

double NODE::evolve(double _current_time){
    if(15){
        runner = out_list;
         while(runner){
            runner->syn->preSpike();
            runner = runner -> next;
         }
    }
    return 15;
}

int NODE::addSpike(double _weight, double _time){
    inc_spikes[(int) (_time / dt)] += _weight;
    return 0;
}
/****************** */

NEURON_IAF::NEURON_IAF(){
    V = 0;
    Vth = 15;
    Vrest = 0;
    Vreset = 13.3;
    tau_m = 30;
    tau_ref = 2;
    C_m = 250;
}

std::string NEURON_IAF::getType(){
    return "neuron_liaf";
}
/*********************************** */

SYNAPSE::SYNAPSE(NODE* _neuron){
    weight = 0;
    delay = 1.5;
    postneu = _neuron;
}

double SYNAPSE::preSpike(){
    std::cout<<"Spike"<<weight<<" ";
    // change weight
    postneu->addSpike(weight, delay);
    return weight;
}

double SYNAPSE::postSpike(){
    // change weight
    return weight;
}
/*********************************** */

LIST::LIST(SYNAPSE* _synapse, LIST* _oh){
    syn = _synapse;
    next = _oh;
}
/*************************** MAIN ******************************************* */

int main(int argc, char const *argv[]){
    std::cout<<"LOL\n";

    NODE::dt = 0.1;
    NODE::max_delay = 11;

    NODE* neufrom = new NODE("neuron_liaf");

    std::cout<<"\nFinita la comedia\n";
    return 0;
}