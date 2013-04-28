// file:    Layer.h
// author:  Robert Keller
// purpose: Header file for Layer class

#ifndef __Layer__
#define __Layer__

#include "ActivationFunction.h"
#include "Neuron.h"
#include "Sample.h"
#include "Source.h"

/**
 * A Layer is a layer of Neurons
 */

class Layer : public Source
{
protected:

/**
 * the number of Neurons in the layer
 */

int numberInLayer;

/**
 * the array of Neurons in the layer
 */

Neuron* neuron;


/**
 * the index of this layer (for tracing purposes)
 */

int layerIndex;

public:

/**
 * constructor
 */

Layer();


/**
 * constructor
 */

Layer(int _layerIndex, int _numberInLayer, ActivationFunction* type, int _numberInputs);


/**
 * Initialize the layer by specifying the number of Neurons and
 * the number of inputs of each.  
 *
 * Randomize the weights of each Neuron.
 */

void init(int _layerIndex, int _numberInLayer, ActivationFunction* type, int _numberInputs);


/**
 * Return the index of this layer.
 */

int getIndex() const;


/**
 * Return the number of neurons in this Layer.
 */

int getSize() const;


/**
 * Get the output value of the ith neuron in this layer.
 */

virtual double get(int i) const;


/**
 * Get the sum of the weighted sensitivities from the ith neuron of the previous layer.
 */

double getSumWeightedSensitivity(int i) const;


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

virtual void fire(const Source& source);

virtual void use(const Source& source);


/**
 * Return a reference to the neuron at the specified index.
 */

Neuron& operator[](int index);


/**
 * Adjust the weights on each neuron in this layer.
 */

void adjustWeights(const Source& source, double rate);


void accumulateWeights(const Source& source, double rate);

void accumulateGradient(const Source& source);

void clearAccumulation();

void installAccumulation();

void adjustByRprop(double etaPlus, double etaMinus);


/**
 * Show the weights on each neuron in this layer on the standard output stream.
 */

void showWeights(const char* title) const;


/**
 * Show the output on the standard output stream.
 */

virtual void showOutput() const;


virtual void setSensitivity(const Layer& nextLayer);

virtual void setSensitivity(const Sample& sample);

virtual double computeError(const Sample& sample) const;


/**
 * Destructor
 */

virtual ~Layer();

}; // class Layer

#endif
