#include <fstream>
#include <iostream>
#include <vector>

const int	minimumParameters = 0;

void getNextNeuronWeights(std::ifstream& weightStream, std::vector<double>& weights)
{
  int numberOfInputs;
  double weight;
  weightStream >> numberOfInputs;
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
  //ActivationFunction** layerType = new ActivationFunction*[numberLayers];
  layerSize[0] = 16;
  layerSize[1] = 1;
  //layerType[0] = new Logsig();
  //layerType[1] = new Purelin();

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
  	for (int i = 0; i < weights.size(); i++) {
  		//network.setWeight(layer, neuron, i, weights.at(i));
  	}
  	// find way to setSensitivty not from Sample
  }
}
