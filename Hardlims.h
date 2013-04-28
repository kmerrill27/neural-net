#ifndef __Hardlims__
#define __Hardlims__

#include "ActivationFunction.h"

class Hardlims : public ActivationFunction
{
public:

double act(double arg);

double use(double arg);

double deriv(double arg, double value);

std::string getName();

};
#endif
