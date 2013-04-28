#include "Satlins.h"
#include <math.h>

#include <iostream>

double Satlins::act(double arg)
  {
  return tanh(arg);
  }

double Satlins::use(double arg)
  {
  return arg >=  1 ? 1
       : arg <= -1 ? -1
       : arg;
  }

double Satlins::deriv(double arg, double out)
  {
  return 1 - out*out;
  }

std::string Satlins::getName()
  {
  return "satlins";
  }
