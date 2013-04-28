// file:    OnehotLayer.h
// author:  Robert Keller
// purpose: Header file for Layer class
// $Id: OnehotLayer.h,v 1.1 2005/05/26 22:22:36 keller Exp keller $

#ifndef __OnehotLayer__
#define __OnehotLayer__

#include "Layer.h"

/**
 * A OnehotLayer is a layer of Neurons
 */

class OnehotLayer : public Layer
{
private:

int maxIndex;


public:

/**
 * constructor
 */

OnehotLayer(int _layerIndex, int _numberInOnehotLayer, ActivationFunction* type, int _numberInputs);


/**
 * Initialize the layer by specifying the number of Neurons and
 * the number of inputs of each.  
 *
 * Randomize the weights of each Neuron.
 */

void init(int _layerIndex, int _numberInLayer, ActivationFunction* type, int _numberInputs);


/**
 * Get the output value of the ith neuron in this layer.
 */

virtual double get(int i) const;


/**
 * Set the weight to a specific value.
 */

void setWeight(int i, int j, double weight);


/**
 * Fire all the Neurons in this layer.
 *
 * based on the Source, setting the output value
 * and derivative evaluated at the net value.
 */

void fire(const Source& source);

void use(const Source& source);


/**
 * Return a reference to the neuron at the specified index.
 */

Neuron& operator[](int index);


/**
 * Adjust the weights on each neuron in this layer.
 */

void adjustWeights(const Source& source, double rate);


/**
 * Show the weights on each neuron in this layer on the standard output stream.
 */

void showWeights(const char* title) const;


/**
 * Show the output on the standard output stream.
 */

void showOutput() const;


void setSensitivity(const Sample& sample);

double computeError(const Sample& sample) const;


/**
 * Destructor
 */

virtual ~OnehotLayer();

}; // class OnehotLayer

#endif
