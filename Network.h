// file:    Network.h
// author:  Robert Keller
// purpose: Header file for Network class

#include "ActivationFunction.h"
#include "Layer.h"

#include <iostream>
#include <fstream>

/**
 * Network defines a network with an arbitrary number of hidden layers
 * and one output layer.
 */

class Network
{
protected:

int inputDimension;

int numberLayers;

int lastLayer;

Layer** layer;

public:

/**
 * constructor
 */

Network();


/**
 * constructor
 */

Network(int _numberLayers, int* sizes, ActivationFunction** types, int inputDimension);


/**
 * Initialize the Network by specifying the number of Neurons and
 * the number of inputs in the hidden layer and the number of inputs.
 *
 * Randomize the weights of each Neuron.
 *
 * @param numberLayers total number of Layers, including the Output
 * @param inputDimension number of inputs to the network
 */

void init(int _numberLayers, int* sizes, ActivationFunction** types, int inputDimension);


/**
 * Fire all the Neurons in the Network based on the input Sample.
 * starting with the hidden layer and working forward.
 *
 * @param sample the Sample from which the inputs to the network are taken
 */

void fire(const Sample& sample);

void use(const Sample& sample);


/**
 * Get the output value of the network, based on the most recent firing.
 */

double getOutput();


/**
 * Show the output of the network on the standard output stream.
 */

void showOutput();


/**
 * Compute the error as compared with the output of a given Sample.
 *
 * @param sample the Sample from which the inputs to the network are taken
 */

double computeError(const Sample& sample);


/**
 * Compute error as if used, rather than trained (these are different
 * in the discrete output case).
 *
 * @param sample the Sample from which the inputs to the network are taken
 */

int computeUsageError(const Sample& sample);


/**
 * Set the sensitivities in the Network based on the values in a given Sample,
 * in preparation for adjusting the weights.
 *
 * @param sample the Sample from which the sensitivity of the output Neuron is set.
 */

void setSensitivity(const Sample& sample);

void setFixedSensitivity(int i, int j, double sensitivity);

/**
 * Set weight to a specific value
 */

void setWeight(int layerNum, int neuronNum, int weightNum, double weightVal);

/**
 * Adjust the weights of a network based on error values of a given Sample and
 * previously set sensitivities.
 *
 * @param sample the Sample from which the sensitivity of the output Neuron is set.
 * @param rate the learning rate with which weights are adjusted.
 */

void adjustWeights(const Sample& sample, double rate);

void accumulateWeights(const Sample& sample, double rate);

void accumulateGradient(const Sample& sample);

void clearAccumulation();

void installAccumulation();

void adjustByRprop(double etaPlus, double etaMinus);


/**
 * Show the weights and sensitivities of all Neurons in the network.
 */

void showWeights(const char* msg) const;

/**
 * Save the sensitivity and weights to a file.
 */

void saveWeights(std::ofstream& weightStream);

void saveStats(std::ofstream& weightStream);

/**
 * destructor
 */

~Network();

};
