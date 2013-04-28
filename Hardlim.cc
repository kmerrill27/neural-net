#include "Hardlim.h"
#include <math.h>

#include <iostream>

double Hardlim::act(double arg)
  {
  return 1./(1+exp(-arg));	// to center for training
  }

double Hardlim::use(double arg)
  {
  return arg > 0;
  }

double Hardlim::deriv(double arg, double out)
  {
  return out*(1-out);
  }

std::string Hardlim::getName()
  {
  return "hardlim";
  }
