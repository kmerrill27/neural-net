// file:    Trace.h
// author:  Robert Keller
// purpose: Header file for Trace class

#ifndef __Trace__
#define __Trace__

/**
 * Trace is a "static" class that holds the current trace level so that it can
 * be accessed globally without being passed explicitly.
 */

class Trace
{
private:

/**
 * the current trace level
 */

static int level;


public:

/**
 * Get the current trace level.
 */

static bool atLevel(int value);


/**
 * Set the current trace level.
 */

static void setLevel(int value);


/**
 * Get the current trace level.
 */

static int getLevel();

}; // class Trace

#endif
