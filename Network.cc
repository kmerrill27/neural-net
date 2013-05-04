// file:    Network.cc
// author:  Robert Keller
// purpose: Implementation of the Network class
// $Id: Network.cc,v 1.8 2010/09/21 17:40:09 keller Exp keller $

#include "Network.h"
#include "OnehotLayer.h"
#include "assert.h"

#include <iostream>

/**
 * constructor
 */

Network::Network()
  {
  }


/**
 * constructor
 */

Network::Network(int numberLayers, int* layerSize, ActivationFunction** type, int inputDimension)
  {
  init(numberLayers, layerSize, type, inputDimension);
  }


/**
 * Initialize the Network by specifying the number of Neurons and
 * the number of inputs in the hidden layer and the number of inputs.
 *
 * Randomize the weights of each Neuron.
 *
 * @param numberHidden number of Neurons in the hidden layer
 * @param inputDimension number of inputs to the network
 */

void Network::init(int _numberLayers, int* layerSize, ActivationFunction** type, int _inputDimension)
  {
  numberLayers = _numberLayers;
  
  layer = new Layer*[numberLayers];

  inputDimension = _inputDimension;

  // FIX: This assumes at least one hidden layer, probably not a good idea.

  assert(layerSize);
  assert(numberLayers > 1);

  lastLayer = numberLayers-1;

  layer[0] = new Layer(0, layerSize[0], type[0], _inputDimension);

  for( int i = lastLayer-1; i > 0; i-- )
    {
    layer[i] = new Layer(i, layerSize[i], type[i], layerSize[i-1]);
    }

  if( type[lastLayer]->getName() == "onehot" )
    {
    layer[lastLayer] = new OnehotLayer(lastLayer, 
                                       layerSize[lastLayer], 
                                       type[lastLayer], 
                                       layerSize[lastLayer-1]);
    }
  else
    {
    layer[lastLayer] = new Layer(lastLayer, 
                                 layerSize[lastLayer], 
                                 type[lastLayer], 
                                 layerSize[lastLayer-1]);
    }
  }



/**
 * Fire all the Neurons in the Network based on the input Sample.
 * starting with the hidden layer and working forward.
 *
 * @param sample the Sample from which the inputs to the network are taken
 */

void Network::fire(const Sample& sample)
  {
  layer[0]->fire(sample);

  for( int i = 1; i < numberLayers; i++ )
    {
    layer[i]->fire(*(layer[i-1]));
    }
  }


void Network::use(const Sample& sample)
  {
  layer[0]->use(sample);

  for( int i = 1; i < numberLayers; i++ )
    {
    layer[i]->use(*(layer[i-1]));
    }
  }


/**
 * Show the output of the network on the standard output stream.
 */

void Network::showOutput()
  {
  layer[lastLayer]->showOutput();
  }

/**
 * Save the output of the network to a file.
 */

void Network::saveOutput(std::ofstream& outputStream)
  {
  layer[lastLayer]->saveOutput(outputStream);
  }

/**
 * Compute the error as compared with the output of a given Sample.
 *
 * @param sample the Sample from which the inputs to the network are taken
 */

double Network::computeError(const Sample& sample)
  {
  double sse = 0;
  int n = sample.getOutputDimension();
  assert(n > 0);
  for( int i = 0; i < n; i++ )
    {
    double error = sample.getOutput(i) - layer[lastLayer]->get(i);
    sse += error*error;
    }
  return sse/n;
  }


/**
 * Compute the sign agreement as compared with the output of a given Sample.
 * This is for use with discrete outputs only.
 * @param sample the Sample from which the inputs to the network are taken
 */

int Network::computeUsageError(const Sample& sample)
  {
  int n = sample.getOutputDimension();
  for( int i = 0; i < n; i++ )
    {
    if( (sample.getOutput(i) > 0.5) != (layer[lastLayer]->get(i) > 0.5) )
      {
      return 1;		// disagreement
      }
    }
  return 0;		// no disagreement
  }


/**
 * Set the sensitivities in the Network based on the values in a given Sample,
 * in preparation for adjusting the weights.
 *
 * @param sample the Sample from which the sensitivity of the output Neuron is set.
 */

void Network::setSensitivity(const Sample& sample)
  {
  layer[lastLayer]->setSensitivity(sample);

  for( int i = lastLayer-1; i >= 0; i-- )
    {
    layer[i]->setSensitivity(*(layer[i+1]));
    }
  }

/**
 * Set sensitivity of specified neuron.
 *
 * @param i layer index of layer to be set
 * @param j neuron index of neuron to be set
 * @param sensitivity fixed sensitivity value 
 */

void Network::setFixedSensitivity(int i, int j, double sensitivity)
  {
    layer[i]->setFixedSensitivity(j, sensitivity);
  }

/**
 * Set weight to a specific value
 */

void Network::setWeight(int layerNum, int neuronNum, int weightNum, double weightVal)
  {
  layer[layerNum]->setWeight(neuronNum, weightNum, weightVal);
  }

/**
 * Adjust the weights of a network based on error values of a given Sample and
 * previously set sensitivities.
 *
 * @param sample the Sample from which the sensitivity of the output Neuron is set.
 * @param rate the learning rate with which weights are adjusted.
 */

void Network::adjustWeights(const Sample& sample, double rate)
  {
  for( int i = lastLayer; i > 0; i-- )
    {
    layer[i]->adjustWeights(*(layer[i-1]), rate);
    }
  layer[0]->adjustWeights(sample, rate);
  }


void Network::accumulateWeights(const Sample& sample, double rate)
  {
  for( int i = lastLayer; i > 0; i-- )
    {
    layer[i]->accumulateWeights(*(layer[i-1]), rate);
    }
  layer[0]->accumulateWeights(sample, rate);
  }


void Network::accumulateGradient(const Sample& sample)
  {
  for( int i = lastLayer; i > 0; i-- )
    {
    layer[i]->accumulateGradient(*(layer[i-1]));
    }
  layer[0]->accumulateGradient(sample);
  }


void Network::clearAccumulation()
  {
  for( int i = lastLayer; i >= 0; i-- )
    {
    layer[i]->clearAccumulation();
    }
  }

void Network::installAccumulation()
  {
  for( int i = lastLayer; i >= 0; i-- )
    {
    layer[i]->installAccumulation();
    }
  }

void Network::adjustByRprop(double etaPlus, double etaMinus)
  {
  for( int i = lastLayer; i >= 0; i-- )
    {
    layer[i]->adjustByRprop(etaPlus, etaMinus);
    }
  }

/**
 * Show the weights and sensitivities of all Neurons in the network.
 */

void Network::showWeights(const char* msg) const
  {
  for( int i = 0; i < numberLayers; i++ )
    {
    layer[i]->showWeights(msg);
    }
  }

/**
 * Save the sensitivity and weights to a file.
 */

void Network::saveWeights(std::ofstream& weightStream)
  {
  for( int i = 0; i < numberLayers; i++ )
    {
    layer[i]->saveWeights(weightStream);
    }
  }

/**
* Save input dimension and layer attributes to a file.
*/

void Network::saveStats(std::ofstream& weightStream)
{
  weightStream << inputDimension << std::endl;
  weightStream << numberLayers << std::endl;
  for (int i = 0; i < numberLayers; i++)
  {
    weightStream << layer[i]->getSize() << std::endl;
    weightStream << layer[i]->getType() << std::endl;
  }
}

/**
 * destructor
 */

Network::~Network()
  {
  }
