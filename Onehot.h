#ifndef __Onehot__
#define __Onehot__

#include "ActivationFunction.h"

class Onehot : public ActivationFunction	// dummy, used only for name
{
public:

double act(double arg);

double use(double arg);

double deriv(double arg, double value);

std::string getName();

private:
};
#endif
