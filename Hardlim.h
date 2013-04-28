#ifndef __Hardlim__
#define __Hardlim__

#include "ActivationFunction.h"

class Hardlim : public ActivationFunction
{
public:

double act(double arg);

double use(double arg);

double deriv(double arg, double value);

std::string getName();

};
#endif
