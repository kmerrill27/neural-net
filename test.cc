#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "Network.h"
#include "ActivationFunction.h"
#include "Logsig.h"
#include "Purelin.h"

const int	minimumParameters = 2;

int inputDimension;
int numberLayers;
int* layerSize;
ActivationFunction** layerType;

ActivationFunction* logsig  = new Logsig();
ActivationFunction* purelin = new Purelin();

ActivationFunction* getLayerType(std::string name)
  {
  if( name == "logsig" )   return logsig;
  if( name == "purelin" )  return purelin;

  std::cout << "error, unrecognized function: " << name << std::endl;
  exit(1);  
  }

void loadNextNeuronWeights(std::ifstream& weightStream, std::vector<double>& weights)
{
  int numberOfInputs;
  double weight;
  weights.clear();
  for( int i = 0; i <= numberOfInputs && weightStream >> weight; i++  )
    {
      weights.push_back(weight);
    }
}

void loadStats(std::ifstream& weightStream)
{
  int lSize;
  std::string lType;

  weightStream >> inputDimension;
  weightStream >> numberLayers;

  for (int i = 0; i < numberLayers && weightStream >> lSize; i++)
  {
    layerSize[i] = lSize;
    weightStream >> lType;
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
  std::cout << "parameters: <saved weight file> <test file>" << std::endl;
  exit(0);
  }

  char* weightFilename = argv[1];

  std::cout << "weight file: " << weightFilename << std::endl;

  std::ifstream weightStream(weightFilename);

  char* testFile = argv[2];

  std::cout << "test file: " << testFile << std::endl;

  std::vector<double> weights;

  layerSize = new int[numberLayers];
  layerType = new ActivationFunction*[numberLayers];

  loadStats(weightStream);

  Network network(numberLayers, layerSize, layerType, inputDimension);

  int layer, neuron;
  double sensitivity;
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
}
