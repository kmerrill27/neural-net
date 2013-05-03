// file:    Layer.cc
// author:  Robert Keller
// purpose: C++ code for Layer class
// $Id: Layer.cc,v 1.3 2005/05/26 22:22:36 keller Exp keller $

#include <assert.h>
#include <stdio.h>
#include <iostream>

#include "Layer.h"

/**
 * constructor
 */

Layer::Layer()
  {
  }


/**
 * constructor
 */

Layer::Layer(int _layerIndex, int _numberInLayer, ActivationFunction* _type, int _numberOfInputs)
  {
  init(_layerIndex, _numberInLayer, _type, _numberOfInputs);
  }


/**
 * Initialize the layer by specifying the number of Neurons and
 * the number of inputs of each.
 *
 * Randomize the weights of each Neuron.
 */

void Layer::init(int _layerIndex, int _numberInLayer, ActivationFunction* _type, int _numberOfInputs)
  {
  layerIndex = _layerIndex;

  numberInLayer = _numberInLayer;

  assert( neuron = new Neuron[numberInLayer] );

  for( int i = 0; i < numberInLayer; i++ )
    {
    neuron[i].init(layerIndex, i, _type, _numberOfInputs);	// initialize neuron and weights
    }
  }


/**
 * Return the index of this Layer.
 */

int Layer::getIndex() const
  {
  return layerIndex;
  }


/**
 * Return the number of neurons in this Layer.
 */

int Layer::getSize() const
  {
  return numberInLayer;
  }


/**
 * Get the output value of the ith neuron.
 */

double Layer::get(int i) const
  {
  return neuron[i].getOutput();
  }


/**
 * Fire all the Neurons in this layer.
 *
 * based on the Source, setting the output value
 * and derivative evaluated at the net value.
 */

void Layer::fire(const Source& source)
  {
  for( int i = 0 ; i < numberInLayer; i++ )
    {
    neuron[i].fire(source);
    }
  }


void Layer::use(const Source& source)
  {
  for( int i = 0 ; i < numberInLayer; i++ )
    {
    neuron[i].use(source);
    }
  }


/**
 * Get the sum of the weighted sensitivities from the ith neuron of the previous layer.
 */

double Layer::getSumWeightedSensitivity(int i) const
  {
  double sum = 0;
  for( int j = 0; j < numberInLayer; j++ )
    {
    sum += neuron[j].getWeightedSensitivity(i);
    }
  return sum;
  }


/**
 * Set weight to a specific values
 */

void Layer::setWeight(int i, int j, double weight)
  {
  assert( i < numberInLayer );
  neuron[i].setWeight(j, weight);
  }


/**
 * Adjust the weights on each neuron in this layer.
 */

void Layer::adjustWeights(const Source& source, double rate)
  {
  for( int i = 0 ; i < numberInLayer; i++ )
    {
    neuron[i].adjustWeights(source, rate);
    }
  }


/**
 * Accumulate the weight changes on each neuron in this layer.
 */

void Layer::accumulateWeights(const Source& source, double rate)
  {
  for( int i = 0 ; i < numberInLayer; i++ )
    {
    neuron[i].accumulateWeights(source, rate);
    }
  }


void Layer::accumulateGradient(const Source& source)
  {
  for( int i = 0 ; i < numberInLayer; i++ )
    {
    neuron[i].accumulateGradient(source);
    }
  }


void Layer::clearAccumulation()
  {
  for( int i = 0 ; i < numberInLayer; i++ )
    {
    neuron[i].clearAccumulation();
    }
  }


void Layer::installAccumulation()
  {
  for( int i = 0 ; i < numberInLayer; i++ )
    {
    neuron[i].installAccumulation();
    }
  }

void Layer::adjustByRprop(double etaPlus, double etaMinus)
  {
  for( int i = 0 ; i < numberInLayer; i++ )
    {
    neuron[i].adjustByRprop(etaPlus, etaMinus);
    }
  }



/**
 * Show the weights on each neuron in this layer on the standard output stream.
 */

void Layer::showWeights(const char* title) const
  {
  for( int i = 0; i < numberInLayer; i++ )
    {
    neuron[i].showWeights(title);
    }
  }

/**
 * Save the sensitivity and weights to a file.
 */

void Layer::saveWeights(std::ofstream& weightStream)
  {
  for( int i = 0; i < numberInLayer; i++ )
    {
    neuron[i].saveWeights(weightStream);
    }
  }

/**
 * Show the outputs on the standard output stream.
 */

void Layer::showOutput() const
  {
  for( int i = 0; i < numberInLayer; i++ )
    {
    std::cout << " " << neuron[i].getOutput();
    }
  }


/**
 * Set the sensitivity of the Neurons in the HiddenLayer,
 * based on the value in the next Layer.
 *
 *@param nextLayer the Layer following this one
 */

void Layer::setSensitivity(const Layer& nextLayer)
  {
  for( int i = 0; i < numberInLayer; i++ )
    {
    neuron[i].setSensitivity(nextLayer.getSumWeightedSensitivity(i));
    }
  }

/**
 * Set the sensitivity of the Neuron in the Output Layer,
 * based on the value in Sample.
 *
 *@param sample the Sample upon which the sensitivity setting is based
 */

void Layer::setSensitivity(const Sample& sample)
  {
  for( int i = 0; i < numberInLayer; i++ )
    {
    double error = sample.getOutput(i) - neuron[i].getOutput();
    neuron[i].setSensitivity(-2 * error);
    }
  }

void Layer::setFixedSensitivity(int i, double sensitivity)
  {
  assert( i < numberInLayer );
  neuron[i].setFixedSensitivity(sensitivity);
  }

/**
 * Compute the error of the Output Layer,
 * based on the value in Sample.
 *
 *@param sample the Sample upon which the error computation is based
 */

double Layer::computeError(const Sample& sample) const
  {
  double sse = 0;
  for( int i = 0; i < numberInLayer; i++ )
    {
    double error = sample.getOutput(i) - neuron[i].getOutput();
    sse += error*error;
    }
  return sse;
  }



Layer::~Layer()
  {
  delete [] neuron;
  }
