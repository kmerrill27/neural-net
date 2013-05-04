// file:    Neuron.cc
// author:  Robert Keller
// modified by: Kim Merrill (5/3/13)
// purpose: C++ code for Neuron class
// $Id: Neuron.cc,v 1.4 2005/05/26 22:22:36 keller Exp keller $

#include "Neuron.h"
#include "Trace.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


/**
 * constructor
 */

Neuron::Neuron()
  {
  }


/**
 * Initialize this neuron, set its index and number of inputs
 * and randomizing the weights.
 */

void Neuron::init(int _layerIndex, int _neuronIndex, ActivationFunction* _type, int _numberOfInputs)
  {
  neuronIndex = _neuronIndex;

  layerIndex = _layerIndex;

  type = _type;

  numberOfInputs = _numberOfInputs;

  double initialUpdate = 0.1;

  // weight[numberOfInputs] will be the bias

  assert( weight = new double[numberOfInputs+1] );

  assert( accumulated = new double[numberOfInputs+1] );

  assert( oldAccumulated = new double[numberOfInputs+1] );

  assert( updateValue = new double[numberOfInputs+1] );

  // Initialize all weights randomly.

  for( int j = 0; j <= numberOfInputs; j++ )
    {
    setWeight(j, (drand48()-0.5));
    updateValue[j] = initialUpdate;
    accumulated[j] = 0;
    oldAccumulated[j] = 0;
    }
  }


/**
 * Fire this neuron using the specified Source as input.
 * Remember the output value and derivative of the activation function.
 */

void Neuron::fire(const Source& source)
  {
  net  = weight[numberOfInputs];	// bias component

  for( int j = 0; j < numberOfInputs; j++ )
    {
    net += weight[j]*source.get(j);	// compute the "net" value
    }

  output = type->act(net);		// set the output

  deriv = type->deriv(net, output);	// set the transfer function derivative
  }


void Neuron::use(const Source& source)
  {
  net  = weight[numberOfInputs];	// bias component

  for( int j = 0; j < numberOfInputs; j++ )
    {
    net += weight[j]*source.get(j);	// compute the "net" value
    }

  output = type->use(net);		// set the output
  }


/**
 * Get the output of this neuron based on the previous firing.
 */

double Neuron::getOutput() const
  {
  return output;
  }


/**
 * Adjust the weights of this neuron, based on the values in source.
 */

void Neuron::adjustWeights(const Source& source, double rate)
  {
  double factor = -rate * sensitivity;
  for( int j = 0 ; j < numberOfInputs; j++ )
    {
    addWeight(j, factor, source.get(j));
    }

  addWeight(numberOfInputs, factor, 1); // bias
  }


/**
 * Accumulate weigh adjustment without changing weight
 */

void Neuron::accumulateWeights(const Source& source, double rate)
  {
  double factor = -rate * sensitivity;
  for( int j = 0 ; j < numberOfInputs; j++ )
    {
    accumulated[j] += factor*source.get(j);
    }

  accumulated[numberOfInputs] += factor;
  }


/**
 * Accumulate gradient adjustment (without learning rate)
 */

void Neuron::accumulateGradient(const Source& source)
  {
  for( int j = 0 ; j < numberOfInputs; j++ )
    {
    accumulated[j] += sensitivity*source.get(j);
    }

  accumulated[numberOfInputs] += sensitivity;
  }


void Neuron::clearAccumulation()
  {
  for( int j = 0 ; j <= numberOfInputs; j++ )
    {
    accumulated[j] = 0;
    }
  }

void Neuron::installAccumulation()
  {
  for( int j = 0 ; j <= numberOfInputs; j++ )
    {
    weight[j] += accumulated[j];
    }
  }

double max(double x, double y)
  {
  return x > y ? x : y;
  }

double min(double x, double y)
  {
  return x > y ? y : x;
  }

double sign(double x)
  {
  return x > 0 ? 1 : x < 0 ? -1 : 0;
  }

void Neuron::adjustByRprop(double etaPlus, double etaMinus)
  {
  double deltaMax = 50;
  double deltaMin = 1e-6;

  for( int j = 0 ; j <= numberOfInputs; j++ )
    {
    double product = oldAccumulated[j]*accumulated[j];

    if( product > 0 )
      {
      updateValue[j] = min(etaPlus*updateValue[j], deltaMax);

      weight[j] -= updateValue[j]*sign(accumulated[j]);

      oldAccumulated[j] = accumulated[j];
      }
    else if( product < 0 )
      {
      updateValue[j] = max(etaMinus*updateValue[j], deltaMin);

      oldAccumulated[j] = 0;

      // note: no weight change
      }
    else
      {
      weight[j] -= updateValue[j]*sign(accumulated[j]);
      oldAccumulated[j] = accumulated[j];
      }

    accumulated[j] = 0;	// reset
    }
  }

/**
 * Get the ith weight of this neuron.
 */

double Neuron::getWeight(int i) const
  {
  return weight[i];
  }


/**
 * Get the product of this Neuron's sensitivity times the ith input weight.
 */

double Neuron::getWeightedSensitivity(int i) const
  {
  return sensitivity * weight[i];
  }


/**
 * Set a specified weight
 */

void Neuron::setWeight(int j, double _weight)
  {
  assert( j <= numberOfInputs );
  weight[j] = _weight;
  }


/**
 * Set the sensitivity of this neuron, first multiplying the argument by the derivative
 * from the most recent activation.
 */

void Neuron::setSensitivity(double factor)
  {
  sensitivity = deriv*factor;
  }

/**
 * Set the sensitivity of this neuron to specified argument - i.e. without first multiplying
 * by the derivative from the most recent activation.
 */
void Neuron::setFixedSensitivity(double s)
{
  sensitivity = s;
}

/**
 * Add an increment to a weight.
 */

void Neuron::addWeight(int weightIndex, double factor, double input)
  {
  double delta = factor*input;
  double newWeight = weight[weightIndex] + delta;
  if( Trace::atLevel(5) )
    {
    printf("backward layer %d neuron %d input: % 7.4f add % 7.4f (from % 7.4f to % 7.4f)\n",
           layerIndex, neuronIndex, input, delta, weight[weightIndex], newWeight);
    }
  weight[weightIndex] = newWeight;
  }


/**
 * Show the sensitivity and weights of this neuron.
 */

void Neuron::showWeights(const char* title)
  {
  printf("layer %d neuron %d %s weights: \n", layerIndex, neuronIndex, title);

  for( int j = 0; j <= numberOfInputs; j++ )
    {
    printf("% 9.4f ", weight[j]);
    }

  printf("(bias) sensitivity: % 7.4f \n", sensitivity);
  }

/**
 * Save the sensitivity and weights to a file.
 */

void Neuron::saveWeights(std::ofstream& weightStream)
  {
  weightStream << layerIndex << std::endl;
  weightStream << neuronIndex << std::endl;
  weightStream << numberOfInputs << std::endl;
  for( int j = 0; j <= numberOfInputs; j++ )
    {
    weightStream << weight[j] << std::endl;
    }
  weightStream << sensitivity << std::endl;
  }

/**
 * Destructor
 */

Neuron::~Neuron()
  {
  delete [] weight;
  delete [] accumulated;
  delete [] updateValue;
  delete [] oldAccumulated;
  }
