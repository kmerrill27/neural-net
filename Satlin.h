#ifndef __Satlin__
#define __Satlin__

#include "ActivationFunction.h"

class Satlin : public ActivationFunction
{
public:

double act(double arg);

double use(double arg);

double deriv(double arg, double value);

std::string getName();

};
#endif
