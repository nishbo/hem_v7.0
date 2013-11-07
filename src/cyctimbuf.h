#ifndef CYCTIMBUF_H
#define CYCTIMBUF_H

#include <cstdlib>
#include <iostream>

#define EXIT_ERROR_CYCTIMBUF 996

class CYCLING_TIME_BUFFER{
    double *arr;
    double timeturn;
    double tstep;
    int cur_pos;
    int leng;
    CYCLING_TIME_BUFFER();

    static double bufd;
    static int bufi;
public:
    CYCLING_TIME_BUFFER(double _step, double _turn);
    double pull();
    double push(double _tdiffer, double _a);
    double peep();
};

class WIDE_CYCLING_TIME_BUFFER{
    CYCLING_TIME_BUFFER* wide_arr;
    int width;
public:
    WIDE_CYCLING_TIME_BUFFER(double _step, double _turn, int _width);
    double* pull();
    double push(double _tdiffer, double _a, int _bufnum);
    double peep(int _bufnum);
};


#endif // CYCTIMBUF_H