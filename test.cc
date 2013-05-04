// file:    test.cc
// author:  Kim Merrill
// purpose: runs test samples through pre-trained multi-layer neural network,
// loading in weights from a file

/**
 * Loads in network attributes (weights, etc.) from file and runs
 * samples through network, saving outputs to file.
 *
 * ./test <weight file> <input file> <outputs file>
 * e.x. ./test licks.weights.save test.sample.in outputs.save
 */

#include <vector>
#include <string>

#include "helper.h"

const int	minimumParameters = 3;

int inputDimension;
int numberLayers;
int* layerSize;
ActivationFunction** layerType;

/**
 * Read in neuron weights from file.
 */

void loadNextNeuronWeights(std::ifstream& weightStream, std::vector<double>& weights)
{
  int numberOfInputs;
  weightStream >> numberOfInputs;
  double weight;
  weights.clear();
  for( int i = 0; i <= numberOfInputs && weightStream >> weight; i++  )
    {
      weights.push_back(weight);
    }
}

/**
 * Read in network attributes (input dimension, number of layers, and layer types + sizes)
 * from file.
 */

void loadStats(std::ifstream& weightStream)
{
  int lSize;
  std::string lType;

  weightStream >> inputDimension;
  std::cout << inputDimension << std::endl;
  weightStream >> numberLayers;
  std::cout << numberLayers << std::endl;

  for (int i = 0; i < numberLayers && weightStream >> lSize; i++)
  {
    layerSize[i] = lSize;
    weightStream >> lType;
    std::cout << lType << std::endl;
    layerType[i] = getLayerType(lType);
  }
}

/**
 * main program reads in saved weights and runs samples through network.
 */

int main(int argc, char** argv)
{

if( argc <= minimumParameters )
  {
  std::cout << "parameters: <saved weight file> <test file> <output file>" << std::endl;
  exit(0);
  }

  char* weightFile = argv[1];
  std::cout << "weight file: " << weightFile << std::endl;
  std::ifstream weightStream(weightFile);

  if( weightStream )
  {
    printf("Weights will be loaded from: %s\n", weightFile);
  }
  else
  {
    printf("Could not find weight file: %s\n", weightFile);
  }

  char* testFile = argv[2];
  std::cout << "test file: " << testFile << std::endl;

  char* outputFile = argv[3];
  std::cout << "output file: " << outputFile << std::endl;
  std::ofstream outputStream(outputFile);

  if( outputStream )
  {
    printf("Outputs will be saved in: %s\n", outputFile);
  }
  else
  {
    printf("Could not create outputs file: %s\n", outputFile);
  }

  std::vector<double> weights;

  layerSize = new int[numberLayers];
  layerType = new ActivationFunction*[numberLayers];

  loadStats(weightStream);

  Network network(numberLayers, layerSize, layerType, inputDimension);

  int layer, neuron;
  double sensitivity;

  // Set up neuron weights and sensitivities one-by-one
  while (weightStream >> layer) {
  	weightStream >> neuron;
  	loadNextNeuronWeights(weightStream, weights);
    weightStream >> sensitivity;
    network.setFixedSensitivity(layer, neuron, sensitivity);
  	for (std::vector<double>::size_type i = 0; i < weights.size(); i++) {
  		network.setWeight(layer, neuron, i, weights.at(i));
  	}
  }
  network.showWeights("set");

int inputDimension2;         // dimension of input
int outputDimension2;        // dimension of output

std::list<Sample*> testSamples;      // list of pointers to test samples

getSamples(testFile, outputDimension2, inputDimension2, testSamples);

double mse;

// Run net on test samples
runSamples(mse, testSamples, network, outputStream);
}
