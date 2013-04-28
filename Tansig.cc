#include "Tansig.h"
#include <math.h>

#include <iostream>

double Tansig::act(double arg)
  {
  return tanh(arg);
  }

double Tansig::use(double arg)
  {
  return tanh(arg);
  }

double Tansig::deriv(double arg, double out)
  {
  return 1 - out*out;
  }

std::string Tansig::getName()
  {
  return "tansig";
  }

