#include <iostream>
#include <cstring>

#include "core.h"
#include "topology.h"

int main(int argc, char const *argv[]){

    int N = 2;
    NODE** node_array = Malloc(N, NODE*);
    for(int i=0; i < N; i++){
        node_array[i] = new NODE("neuron_liaf");
    }
    std::vector<SYNAPSE*> syn_array;

    topology::connectN2N(node_array[0], "static_synapse", node_array[1], 0.1, syn_array);

    // WIDE_CYCLING_TIME_BUFFER a(0.1, 2, 3);
    // a.push(1.2, 5, 0);
    // a.push(2.0, 0.1, 1);
    // a.push(2.0, 4, 3);
    // a.push(3.2, 1, 0);
    // for(int i=0; i < 25; i++){
    //     if( i==5 ){
    //         std::cout<<"NOW! :\n";
    //         a.push(1, 1, 1);
    //     }
    //     double* b = a.pull();
    //     std::cout<<i*0.1<<"\t"<<b[0]<<"\t"<<b[1]<<"\t"<<b[2]<<std::endl;
    // }

    // WIDE_CYCLING_TIME_BUFFER* a;
    // a = new WIDE_CYCLING_TIME_BUFFER(0.1, 2, 3);


    return 0;
}