#ifndef __ActivationFunction__
#define __ActivationFunction__

#include <string>

class ActivationFunction
{
public: 

virtual double act(double arg) = 0;

virtual double use(double arg) = 0;

virtual double deriv(double arg, double value) = 0;

virtual std::string getName() = 0;

virtual ~ActivationFunction() {}

};

#endif
