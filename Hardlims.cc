#include "Hardlims.h"
#include <math.h>

#include <iostream>

double Hardlims::act(double arg)
  {
  return tanh(arg);	// to center for training
  }

double Hardlims::use(double arg)
  {
  return arg > 0 ? 1 : -1;
  }

double Hardlims::deriv(double arg, double out)
  {
  return 1 - out*out;
  }

std::string Hardlims::getName()
  {
  return "hardlims";
  }
