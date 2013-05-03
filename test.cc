#include <fstream>
#include <iostream>
#include <vector>

#include "Network.h"
#include "ActivationFunction.h"
#include "Logsig.h"
#include "Purelin.h"

const int	minimumParameters = 0;

ActivationFunction* logsig  = new Logsig();
ActivationFunction* purelin = new Purelin();

ActivationFunction* getLayerType(std::string name)
  {
  if( name == "logsig" )   return logsig;
  if( name == "purelin" )  return purelin;

  std::cout << "error, unrecognized function: " << name << std::endl;
  exit(1);  
  }

void getNextNeuronWeights(std::ifstream& weightStream, std::vector<double>& weights)
{
  int numberOfInputs;
  double weight;
  weightStream >> numberOfInputs;
  weights.clear();
  for( int i = 0; i <= numberOfInputs && weightStream >> weight; i++  )
    {
      std::cout << "WEIGHT " << i << ": " << weight << std::endl;
      weights.push_back(weight);
    }
}

/**
 * main program reads in saved weights and runs samples through network.
 */

int main(int argc, char** argv)
{

if( argc <= minimumParameters )
  {
  std::cout << "parameters: <saved weight file> <number of layers> <layer type> "
  			   "<number in layer> ... <test file>"
            << std::endl;
  exit(0);
  }

  char* weightFilename = "licks.weights.save";

  std::cout << "weight file: " << weightFilename << std::endl;

  std::ifstream weightStream(weightFilename);

  int numberLayers = 2;
  int* layerSize = new int[numberLayers];
  int inputDimension = 424;
  ActivationFunction** layerType;
  layerType = new ActivationFunction*[numberLayers];
  layerSize[0] = 16;
  layerSize[1] = 1;
  layerType[0] = getLayerType("logsig");
  layerType[1] = getLayerType("purelin");

  Network network(numberLayers, layerSize, layerType, inputDimension);

  char* testFile = "licks.in";

  std::cout << "test file: " << testFile << std::endl;

  std::vector<double> weights;

  int layer, neuron;
  double sensitivity;
  while (weightStream >> layer) {
    std::cout << layer << std::endl;
  	weightStream >> neuron;
  	std::cout << neuron << std::endl;
  	getNextNeuronWeights(weightStream, weights);
    weightStream >> sensitivity;
    std::cout << sensitivity << std::endl;
    std::cout << weights.size() << std::endl;
  	for (std::vector<double>::size_type i = 0; i < weights.size(); i++) {
      //std::cout << i << std::endl;
  		network.setWeight(layer, neuron, i, weights.at(i));
  	}
    network.showWeights("final");
  	// find way to setSensitivty not from Sample
  }
}
