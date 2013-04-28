#include "Logsig.h"
#include <math.h>

#include <iostream>

double Logsig::act(double arg)
  {
  return 1./(1+exp(-arg));
  }

double Logsig::use(double arg)
  {
  return 1./(1+exp(-arg));
  }

double Logsig::deriv(double arg, double out)
  {
  return out*(1-out);
  }

std::string Logsig::getName()
  {
  return "logsig";
  }
