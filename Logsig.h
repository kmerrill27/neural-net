#ifndef __Logsig__
#define __Logsig__

#include "ActivationFunction.h"

class Logsig : public ActivationFunction
{
public:

double act(double arg);

double use(double arg);

double deriv(double arg, double value);

std::string getName();

};

#endif
