#ifndef CYCTIMBUF_H
#define CYCTIMBUF_H

#include <cstdlib>
#include <iostream>
#include <vector>


class CyclingTimeBuffer
{
public:
    void init(double dt, double maxDelay);

    double  pull();
    void    push(double delay, double amplitude);
    double  peep();

    ~CyclingTimeBuffer () = default;

private:
    std::vector<double> _array;
    double _maxDelay;
    double _dt;
    int _current_position;

    void _localError(int localErrno, std::string localErrmsg);
    void _localWarning(int localWarno, std::string localWarmsg);
};


class WideCyclingTimeBuffer
{
public:
    void init(double dt, double maxDelay, int width);

    std::vector<double> pull();
    void    push(double delay, double amplitude, int bufnum);
    double  peep(int bufnum);
    
    ~WideCyclingTimeBuffer () = default;

private:
    std::vector<CyclingTimeBuffer> _wideArray;

    void _localWarning(int localWarno, std::string localWarmsg);
};

#endif // CYCTIMBUF_H