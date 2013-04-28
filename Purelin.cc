#include "Purelin.h"
#include <math.h>

#include <iostream>

double Purelin::act(double arg)
  {
  return arg;
  }

double Purelin::use(double arg)
  {
  return arg;
  }

double Purelin::deriv(double arg, double out)
  {
  return 1;
  }

std::string Purelin::getName()
  {
  return "purelin";
  }

