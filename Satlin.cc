#include "Satlin.h"
#include <math.h>

#include <iostream>

double Satlin::act(double arg)
  {
  return 1./(1+exp(-arg));
  }

double Satlin::use(double arg)
  {
  return arg >= 1 ? 1
       : arg <= 0 ? 0
       : arg;
  }

double Satlin::deriv(double arg, double out)
  {
  return out*(1-out);
  }

std::string Satlin::getName()
  {
  return "satlin";
  }
