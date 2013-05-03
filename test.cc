#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <list>

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
  weightStream >> numberOfInputs;
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
 * Get samples from standard input.
 */

void getSamples(char* inputFile, int& outputDimension, int& inputDimension, std::list<Sample*>& listOfSamples)
  {
  std::ifstream in(inputFile);

  if( !in )
    {
    std::cout << "Input file " << inputFile << " not found." << std::endl;
    }

  // Get the dimensions from the input file

  if( !(in >> outputDimension && in >> inputDimension) )
    {
    std::cout << "Insufficient data file, aborting" << std::endl;
    exit(1);
    }

  std::cout << "\nOutput dimension is " << outputDimension << "." << std::endl;
  std::cout << "\nInput dimension is " << inputDimension << "." << std::endl;

  if( outputDimension < 0 || inputDimension < 0 )
    {
    std::cout << "Dimensions must be non-negative, aborting" << std::endl;
    exit(1);
    }

  // Get the training samples, saving them in a list.

  double output, input;

  while( in >> output )
    {
    // There is at least one more sample.

    Sample* thisSample = new Sample(outputDimension, inputDimension);
    
    // Set the first output value.

    thisSample->setOutput(0, output);        

    int i;

    // Set any additional output values.

    for( i = 1; i < outputDimension && in >> output; i++  )
      {
      thisSample->setOutput(i, output);
      }

    if( i+1 < outputDimension )
      {
      std::cout << "Last sample only had " << i+1
                << " out of " << outputDimension
    << " inputs, aborting." 
    << std::endl;
      exit(1);
      }


    // Set the input values.

    for( i = 0; i < inputDimension && in >> input; i++  )
      {
      thisSample->setInput(i, input);
      }

    // Check for incomplete input sample.  
    // i should be inputDimension-1 if complete.

    if( i+1 < inputDimension )
      {
      std::cout << "Last sample only had " << i+1
                << " out of " << inputDimension
    << " inputs, aborting." 
    << std::endl;
      exit(1);
      }

    listOfSamples.push_back(thisSample);
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

int inputDimension2;         // dimension of input
int outputDimension2;        // dimension of output

std::list<Sample*> testSamples;      // list of pointers to test samples

getSamples(testFile, outputDimension2, inputDimension2, testSamples);

double mse = 0;

double usageError = 0;

for( std::list<Sample*>::iterator sample = testSamples.begin();
     sample != testSamples.end();
     sample++
   )
  {

#if 0
// Show sample outpus commented out for brevity
  printf("----------------------------------------------------------------------\n");
  printf("sample ");    
  (*sample)->show();
  printf("\n");  
#endif

  // Final evaluation with "use"

  network.use(**sample);

  double sampleSSE = network.computeUsageError(**sample);

  printf("\nusage outputs:    ");
  network.showOutput();
  printf(", sample usage sse: % 6.3f%s   \n", 
         sampleSSE, sampleSSE > 0 ? " (non-zero)": "");

  usageError += sampleSSE;


  // Final evaluation with "fire"

  network.fire(**sample);

  sampleSSE = network.computeError(**sample);

  mse += sampleSSE;

  printf("test outputs: ");
  network.showOutput();
  printf(", sample test sse: % 6.3f\n", sampleSSE);
  printf("\n");

  }
}
