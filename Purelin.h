#ifndef __Purelin__
#define __Purelin__

#include "ActivationFunction.h"

class Purelin : public ActivationFunction
{
public:

double act(double arg);

double use(double arg);

double deriv(double arg, double value);

std::string getName();

};
#endif
