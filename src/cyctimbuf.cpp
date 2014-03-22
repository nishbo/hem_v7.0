#include "cyctimbuf.h"


void CyclingTimeBuffer::init(double dt, double maxDelay)
{
    _dt = dt;
    _maxDelay = maxDelay;
    _current_position = 0;

    int l = _maxDelay / _dt + 1;
    if (l <= 0.0 || _dt <= 0.0)
        _localError(996, "Bad construction.");
    _array = std::vector<double>(l, 0.0);
}

double CyclingTimeBuffer::pull()
{
    double bufd = _array[_current_position];

    _array[_current_position] = 0.0;
    _current_position++;
    _current_position %= _array.size();

    return bufd;
}

void CyclingTimeBuffer::push(double delay, double amplitude)
{
    if( delay > _maxDelay || delay < 0.0)
        _localWarning(995, "Tried to push bad time.");

    int bufi = int(delay/_dt + 1e-8 + _current_position) % _array.size();
    _array[bufi] += amplitude;
}

double CyclingTimeBuffer::peep()
{
    return _array[_current_position];
}

void CyclingTimeBuffer::_localError(int localErrno, std::string localErrmsg)
{
    std::cout<<"\nError: Problems with 'CyclingTimeBuffer' #"<< localErrno;
    std::cout<<"\nError message: "<< localErrmsg <<std::endl;
    exit(localErrno);
}

void CyclingTimeBuffer::_localWarning(int localWarno, std::string localWarmsg)
{
    std::cout<<"\nWarning: Problems with 'CyclingTimeBuffer' #"<< localWarno;
    std::cout<<"\nWarning message: "<< localWarmsg <<std::endl;
}


/**************************************************************************** */
void WideCyclingTimeBuffer::init(double dt, double maxDelay, int width)
{
    _width = width;
    _wideArray = std::vector<CyclingTimeBuffer>();
    for(int i=0; i < _width; i++){
        _wideArray.push_back(CyclingTimeBuffer());
        _wideArray[-1].init(dt, maxDelay);
    }
}

std::vector<double> WideCyclingTimeBuffer::pull()
{
    std::vector<double> a;
    for(int i=0; i < _width; i++)
        a.push_back(_wideArray[i].pull());
    return a;
}

void WideCyclingTimeBuffer::push(double delay, double amplitude, int bufnum)
{
    if (bufnum >= _width || bufnum < 0)
        _localWarning(994, "Tried to push in wrong bufnum.");
    _wideArray[bufnum].push(delay, amplitude);
}

void WideCyclingTimeBuffer::_localWarning(int localWarno,
                                          std::string localWarmsg)
{
    std::cout<<"\nWarning: Problems with 'WideCyclingTimeBuffer' #";
    std::cout<< localWarno;
    std::cout<<"\nWarning message: "<< localWarmsg <<std::endl;
}
