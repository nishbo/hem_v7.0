#include "cyctimbuf.h"

double CYCLING_TIME_BUFFER::bufd = 0;
int CYCLING_TIME_BUFFER::bufi = 0;

CYCLING_TIME_BUFFER::CYCLING_TIME_BUFFER(double _step, double _turn){
    tstep = _step;
    timeturn = _turn;
    leng = timeturn / tstep + 1;
    if( leng <= 0 || tstep <= 0 ){
        std::cout<<"\nError: Bad contruction of 'CYCLING_TIME_BUFFER'.\n";
        std::cout<<"tstep = "<<tstep<<"; timeturn = "<<timeturn<<"; leng = ";
        std::cout<<leng<<std::endl;
        exit(EXIT_ERROR_CYCTIMBUF);
    }
    cur_pos = 0;
    arr = new double[leng];
    for(int i=0; i < leng; i++){
        arr[i] = 0;
    }
}

double CYCLING_TIME_BUFFER::pull(){
    bufd = arr[cur_pos];
    arr[cur_pos] = 0;
    cur_pos++;
    cur_pos %= leng;
    return bufd;
}

double CYCLING_TIME_BUFFER::push(double _tdiffer, double _a){
    if( _tdiffer > timeturn || _tdiffer < 0){
        std::cout<<"Warning! 'CYCLING_TIME_BUFFER': ";
        std::cout<<"Tried to push bad time '_tdiffer' = ";
        std::cout<<"'"<<_tdiffer<<"'"<<std::endl;
        return -1;
    }
    bufi = int( int( _tdiffer / tstep + 1e-8) + cur_pos ) % leng;
    arr[bufi] += _a;
    return arr[bufi];
}

double CYCLING_TIME_BUFFER::peep(){
    return arr[cur_pos];
}

/**************************************************************************** */

WIDE_CYCLING_TIME_BUFFER::WIDE_CYCLING_TIME_BUFFER(double _step, double _turn, \
                                                    int _width){
    width = _width;
    if( width <= 0 ){
        wide_arr = nullptr;
        return;
    }

    wide_arr = (CYCLING_TIME_BUFFER*)std::malloc( \
        width * sizeof(CYCLING_TIME_BUFFER));
    for(int i=0; i < width; i++)
        wide_arr[i] = CYCLING_TIME_BUFFER( _step, _turn);
}

double* WIDE_CYCLING_TIME_BUFFER::pull(){
    if( width <= 0 )
        return nullptr;

    double* arr = new double[width];
    for(int i=0; i < width; i++)
        arr[i] = wide_arr[i].pull();
    return arr;
}

double WIDE_CYCLING_TIME_BUFFER::push(double _tdiffer, double _a, int _bufnum){
    if( _bufnum >= width || _bufnum < 0){
        std::cout<<"Warning! 'WIDE_CYCLING_TIME_BUFFER': ";
        std::cout<<"Tried to push in wrong '_bufnum' = ";
        std::cout<<"'"<<_bufnum<<"'"<<std::endl;
        return -1;
    }
    return wide_arr[_bufnum].push(_tdiffer, _a);
}
