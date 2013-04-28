#ifndef __Tansig__
#define __Tansig__

#include "ActivationFunction.h"

class Tansig : public ActivationFunction
{
public:

double act(double arg);

double use(double arg);

double deriv(double arg, double value);

std::string getName();

};
#endif
