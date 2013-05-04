// file:    helper.cc
// author:  Kim Merrill
// purpose: C++ code for helper functions for scripts

#include "helper.h"

ActivationFunction* hardlim  = new Hardlim();
ActivationFunction* hardlims = new Hardlims();
ActivationFunction* onehot   = new Onehot();
ActivationFunction* logsig   = new Logsig();
ActivationFunction* purelin  = new Purelin();
ActivationFunction* satlin   = new Satlin();
ActivationFunction* satlins  = new Satlins();
ActivationFunction* tansig   = new Tansig();

/**
 * Get layer type by matching name to string.
 */

ActivationFunction* getLayerType(std::string name)
  {
  if( name == "hardlim" )  return hardlim;
  if( name == "hardlims" ) return hardlims;
  if( name == "logsig" )   return logsig;
  if( name == "onehot" )   return onehot;
  if( name == "purelin" )  return purelin;
  if( name == "satlin" )   return satlin;
  if( name == "satlins" )  return satlins;
  if( name == "tansig" )   return tansig;

  std::cout << "error, unrecognized function: " << name << std::endl;
  exit(1);  
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
 * Run samples through net and save output values.
 */

double runSamples(double& mse, std::list<Sample*>& testSamples, Network& network,
  std::ofstream& outputStream)
{
  double usageError = 0;
  mse = 0;

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
  network.saveOutput(outputStream);

  printf(", sample test sse: % 6.3f\n", sampleSSE);
  printf("\n");
  }
  return usageError;
}
