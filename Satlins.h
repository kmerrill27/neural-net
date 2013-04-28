#ifndef __Satlins__
#define __Satlins__

#include "ActivationFunction.h"

class Satlins : public ActivationFunction
{
public:

double act(double arg);

double use(double arg);

double deriv(double arg, double value);

std::string getName();

};
#endif
