// file:    Neuron.h
// author:  Robert Keller
// purpose: Header file for Neuron class
// $Id: Neuron.h,v 1.2 2005/05/26 22:22:36 keller Exp keller $

#ifndef __Neuron__
#define __Neuron__

#include "ActivationFunction.h"
#include "Source.h"
#include <fstream>

/**
 * A Neuron has a set of weights.
 * It includes additional information for computation and backpropagation.
 * A Neurons gets specialized to HiddenNeuron or OutputNeuron, depending on how
 * its sensitivity gets set.
 */

class Neuron
{
protected:

ActivationFunction* type;

/**
 * the number of inputs to this neuron, not including the bias
 */

int numberOfInputs;

/**
 * array of weights for this neuron.  The last weight in the array is
 * a bias.
 */

double* weight;


/**
 * accumulated weights, in the case of batch processing 
 * or accumulated gradient in the case of rprop
 */

double* accumulated;


/**
 * old summed gradient weights, in the case of rprop
 */

double* oldAccumulated;


/**
 * for Rprop
 */

double* updateValue;


/**
 * the output value of this neuron based on the last firing
 */

double output;


/**
 * the senstivity of this neuron
 */

double sensitivity;


/**
 * activation or "net" value of this neuron
 */

double net;


/**
 * the value of the derivative of the activation function for this neuron
 */

double deriv;


/**
 * the index for this neuron (0, 1, 2, ...) used for identification
 * purposes and for backpropagation of sensitivities.
 */

int neuronIndex;

/**
 * the index for this neuron's layer  (0, 1, 2, ...) used for identification
 * purposes.
 */

int layerIndex;

public:

/**
 * constructor
 */

Neuron();


/**
 * Initialize this neuron, set its index and number of inputs
 * and randomizing the weights.
 */

void init(int _layerIndex, int _neuronIndex, ActivationFunction* _type, int _numberOfInputs);


/**
 * Fire this neuron using the specified Source as input.
 * Remember the output value and derivative of the activation function.
 */

void fire(const Source& source);

void use(const Source& source);


/**
 * Get the output of this neuron based on the previous firing.
 */

double getOutput() const;


/**
 * Get the net derivative based on the previous firing.
 */

double getDerivative() const;


/**
 * Adjust the weights of this neuron, based on the values in source.
 */

void adjustWeights(const Source& source, double rate);


/**
 * Accumulate weights, in the case of batch processing.
 */

void accumulateWeights(const Source& source, double rate);

void accumulateGradient(const Source& source);

void clearAccumulation();

void installAccumulation();

void adjustByRprop(double etaPlus, double etaMinus);


/**
 * Get the ith weight of this neuron.
 */

double getWeight(int j) const;


/**
 * Get the product of this Neuron's sensitivity times the jth input weight.
 */

double getWeightedSensitivity(int j) const;


/**
 * Set the jth weight of this neuron.
 */

void setWeight(int j, double _weight);


/**
 * Set the sensitivity of this neuron, first multiplying the argument by the derivative
 * from the most recent activation.
 */

void setSensitivity(double factor);

/**
 * Set sensitivity of the neuron to the specified argument - i.e. without first multiplying
 * by the derivative from the most recent activation.
 * Used for setting up a network without training first.
 **/
void setFixedSensitivity(double sensitivity);

/**
 * Add an increment to a weight.
 */

void addWeight(int weightIndex, double factor, double input);


/**
 * Show the sensitivity and weights of this neuron.
 */

void showWeights(const char* title);

/**
 * Save the sensitivity and weights to a file.
 * File is saved in following format:
 *
 * Input dimension
 * Number of layers
 * Number of neurons in layer
 * Layer type
 * ... (^ for each layer)
 * Layer index
 * Neuron index
 * Neuron input dimension (number of weights)
 * Weight 1
 * Weight 2
 * ... (^ for each neuron)
 *
 */

void saveWeights(std::ofstream& weightStream);

/**
 * Destructor
 */

virtual ~Neuron();


}; // class Neuron

#endif
