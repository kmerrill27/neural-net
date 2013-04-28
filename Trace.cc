// file:    Trace.cc
// author:  Robert Keller
// purpose: C++ code for Trace class

#include "Trace.h"

int Trace::level = 0;

/**
 * Get the current trace level.
 */

bool Trace::atLevel(int value)
  {
  return level >= value;
  }


/**
 * Set the current trace level.
 */

void Trace::setLevel(int value)
  {
  level = value;
  }


/**
 * Get the current trace level.
 */

int Trace::getLevel()
  {
  return level;
  }



