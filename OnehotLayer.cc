// file:    OnehotLayer.cc
// author:  Robert Keller
// purpose: C++ code for Layer class
// $Id: OnehotLayer.cc,v 1.1 2005/05/26 22:22:36 keller Exp keller $

#include <assert.h>
#include <stdio.h>
#include <iostream>

#include "OnehotLayer.h"
#include "Tansig.h"

/*
 * Note: the activation function type should be Tansig.
 */

/**
 * constructor
 */

OnehotLayer::OnehotLayer(int _layerIndex, int _numberInLayer, ActivationFunction* _type, int _numberOfInputs) : Layer(_layerIndex, _numberInLayer, _type, _numberOfInputs)
  {
  init(_layerIndex, _numberInLayer, _type, _numberOfInputs);
  }


/**
 * Initialize the layer by specifying the number of Neurons and
 * the number of inputs of each.
 *
 * Randomize the weights of each Neuron.
 */

void OnehotLayer::init(int _layerIndex, int _numberInLayer, ActivationFunction* _type, int _numberOfInputs)
  {
  layerIndex = _layerIndex;

  numberInLayer = _numberInLayer;

  assert( neuron = new Neuron[numberInLayer] );

  ActivationFunction* mytype = new Tansig(); // used to implement one-hot

  for( int i = 0; i < numberInLayer; i++ )
    {
    neuron[i].init(layerIndex, i, mytype, _numberOfInputs);	// initialize neuron and weights
    }

  maxIndex = 0;
  }


/**
 * Get the output value of the ith neuron.
 */

double OnehotLayer::get(int i) const
  {
  assert(i == 0);
  return maxIndex;
  }


/**
 * Fire all the Neurons in this layer.
 *
 * based on the Source, setting the output value
 * and derivative evaluated at the net value.
 */

void OnehotLayer::fire(const Source& source)
  {
  maxIndex = 0;
  neuron[maxIndex].fire(source);
  double maxValue = neuron[maxIndex].getOutput();

  for( int i = 1; i < numberInLayer; i++ )
    {
    neuron[i].fire(source);
    double thisValue = neuron[i].getOutput();
    if( thisValue > maxValue )
      {
      maxIndex = i;
      maxValue = thisValue;
      }
    }
  }


void OnehotLayer::use(const Source& source)
  {
  fire(source);
  }


/**
 * Show the outputs on the standard output stream.
 */

void OnehotLayer::showOutput() const
  {
  std::cout << " " << get(0);
  }


/**
 * Set the sensitivity of the Neuron in the Output Layer,
 * based on the value in Sample.
 *
 *@param sample the Sample upon which the sensitivity setting is based
 */

void OnehotLayer::setSensitivity(const Sample& sample)
  {
  int desired = (int)sample.getOutput(0);
  
  assert(desired >= 0);
  assert(desired < numberInLayer);

  for( int i = 0; i < numberInLayer; i++ )
    {
    double value = (i == desired) ? +1 : -1;
    double error = value - neuron[i].getOutput();
    neuron[i].setSensitivity(-2 * error);
    }
  }


/**
 * Compute the error of the Output OnehotLayer,
 * based on the value in Sample.
 *
 *@param sample the Sample upon which the error computation is based
 */

double OnehotLayer::computeError(const Sample& sample) const
  {
  return (get(0) == sample.getOutput(0)) ? 0 : 1;
  }



OnehotLayer::~OnehotLayer()
  {
  delete [] neuron;
  }
