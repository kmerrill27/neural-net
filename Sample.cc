// file:    Sample.cc
// author:  Robert Keller
// purpose: C++ code for Sample class

#include "Sample.h"

#include <assert.h>
#include <stdio.h>


/**
 * Create a sample, with the indicated input dimension (not including
 * the phantom input) and the desired output value.
 */

Sample::Sample(int _outputDim, int _inputDim)
  : Source()
  {
  assert( _outputDim > 0 );
  outputDim = _outputDim;

  inputDim = _inputDim;

  assert( output = new double[outputDim] );
  assert( input = new double[inputDim] );
  }


/**
 * Set the ith output value for this source.
 */

void Sample::setOutput(int i, double _value)
  {
  output[i] = _value;
  }


/**
 * Set the ith input value for this source.
 */

void Sample::setInput(int i, double _value)
  {
  input[i] = _value;
  }


/**
 * Get the value of the ith output.
 */

double Sample::getOutput(int i) const
  {
  return output[i];
  }


/**
 * Get the value of the ith input.
 */

double Sample::getInput(int i) const
  {
  return input[i];
  }


/**
 * Get the output dimension.
 */

int Sample::getOutputDimension() const
  {
  return outputDim;
  }


/**
 * Get the input dimension.
 */

int Sample::getInputDimension() const
  {
  return inputDim;
  }


/**
 * Get the value of the ith input (as a Source).
 */

double Sample::get(int i) const
  {
  return input[i];
  }


/**
 * Show information about this sample on the output stream.
 */

void Sample::show()
  {
  printf("outputs:");

  for( int i = 0; i < outputDim; i++ )
    {
    printf(" % 6.3f", output[i]);
    }

  printf(" inputs:");

  for( int i = 0; i < inputDim; i++ )
    {
    printf(" % 6.3f", input[i]);
    }
  }




/**
 * destructor
 */

Sample::~Sample()
  {
  delete [] output;
  delete [] input;
  }

