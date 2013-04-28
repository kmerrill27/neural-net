// file:    bp.cc
// author:  Robert Keller
// purpose: multi-layer neural network for backpropagation
// RCS: $Id: bp.cc,v 1.7 2010/09/21 17:40:09 keller Exp keller $

/* This is a self-contained program that trains a two-layer neural
 * network based on an input training set.  
 *
 * It reads the training set data, then attempts to train the network
 * within the given limit on the number of epochs.
 *<pre>
 * Command-line parameters are optional.  If given, they will be interpreted as:
 *
 *    Number of neurons in the hidden layer
 *
 *    Limit on the number of training epochs (a non-negative integer)
 *
 *    Learning rate  (a floating-point numeral)
 *
 *    MSE (mean-squared error) goal (a floating-point numeral)
 *
 *    Trace level (0 = no trace, 1 = some trace, etc. up to about 5)
 *
 * The input is from standard input, in free form, as follows:
 *
 *    A single number, numberInputs, indicating the input dimension
 *
 *    A list of numbers which should be a multiple of numberInputs+1 which
 *    are the training samples:
 *
 *        The single output for the sample (floating point), followed by
 *
 *        The vector of inputs for the sample (floating point)
 *</pre>
 *
 * The output of the program consists of the final weights, as well
 * as intermediate training information.
 */

#include <assert.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "ActivationFunction.h"
#include "Hardlim.h"
#include "Hardlims.h"
#include "Logsig.h"
#include "Network.h"
#include "Onehot.h"
#include "Purelin.h"
#include "Sample.h"
#include "Satlin.h"
#include "Satlins.h"
#include "Tansig.h"
#include "Trace.h"

enum  MODE {ONLINE = 0, BATCH = 1, RPROP = 2};

std::string modeName[] = {"on-line", "batch", "rprop"};

enum TERMINATION_REASON {NONE = 0, 
                         GOAL_REACHED = 1, 
                         LIMIT_EXCEEDED = 2, 
                         LACK_OF_PROGRESS = 3};

std::string reasonName[] = {"", "goal reached", "limit exceeded", "lack of progress"};

const MODE     defaultMode               = ONLINE;

const double  defaultGoal                = 0.01;

const double  defaultLearningRate        = 0.01;

const int     defaultTrace               = 1;

const int     minimumParameters          = 9;

ActivationFunction* hardlim  = new Hardlim();
ActivationFunction* hardlims = new Hardlims();
ActivationFunction* onehot   = new Onehot();
ActivationFunction* logsig   = new Logsig();
ActivationFunction* purelin  = new Purelin();
ActivationFunction* satlin   = new Satlin();
ActivationFunction* satlins  = new Satlins();
ActivationFunction* tansig   = new Tansig();

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


double getFloat(const char* str)
  {
  char* endptr;
  double result = strtod(str, &endptr);
  if( *endptr != '\0' )
    {
    printf("error in command: expected floating point, but found %s\n", str);
    exit(1);
    }
  return result;
  }

int getInteger(const char* str)
  {
  char* endptr;
  int result = strtol(str, &endptr, 0);
  if( *endptr != '\0' )
    {
    printf("error in command: expected integer, but found %s\n", str);
    exit(1);
    }
  return result;
  }


void showAndCountSamples(const char* title, std::list<Sample*>& samples, int& nSamples)
  {
  // Show and count the samples.

  std::list<Sample*>::iterator sample = samples.begin();

  if( Trace::atLevel(4) ) 
    {
    std::cout << "\n" << title << " samples are:\n" << std::endl;
    }

  while( sample != samples.end() )
    {
    ++nSamples;
    if( Trace::atLevel(4) ) 
      {
      std::cout << nSamples << ": ";
      (*sample)->show();
      std::cout << std::endl;
      }
    sample++;
    }

  std::cout << "\n" << nSamples << " " << title << " samples" << std::endl;
  }

/**
 * main program reads samples and runs training.
 */

int main(int argc, char** argv)
{
int epochLimit;

double rate = defaultLearningRate;

double goal = defaultGoal;

MODE mode = defaultMode;

int numberLayers;

int* layerSize;	// array of layer sizes

ActivationFunction** layerType; 

Trace::setLevel(defaultTrace);

if( argc <= minimumParameters )
  {
  std::cout << "parameters: <training file> <max epochs> "
               "<learning rate> <mse goal> <mode: 0 = on-line, 1 = batch, 2 = rprop> <trace> " 
               "<saved weight file> <number of layers> <layer type> <number in layer> ... "
               "<test file> ..."
            << std::endl;
  exit(0);
  }

char* trainingFile = argv[1];

std::cout << "training file: " << trainingFile << std::endl;

// Set epochLimit and learning rate from command-line, if specified.

epochLimit = getInteger(argv[2]);

rate = getFloat(argv[3]);

goal = getFloat(argv[4]);

int modeInt = getInteger(argv[5]);

if( modeInt < 0 || modeInt > 2 )
  {
  printf("mode must be 0, 1, or 2\n");
  exit(1);
  }

mode = (MODE)modeInt;

Trace::setLevel(getInteger(argv[6]));

char* weightFile = argv[7];

std::ofstream weightStream(weightFile);

if( weightStream )
  {
  printf("Weights will be saved in: %s\n", weightFile);
  }
else
  {
  printf("Could not create weight file: %s\n", weightFile);
  }

numberLayers = getInteger(argv[8]);

if( numberLayers < 2 )
  {
  printf("currently, at least two layers must be specified\n");
  exit(1);
  }

int parametersNeeded =  minimumParameters + 2*numberLayers;

if( argc < parametersNeeded )
  {
  printf("need at least %d parameters on the command line for %d layers\n", 
         parametersNeeded, numberLayers);
  exit(1);
  }


layerSize = new int[numberLayers];

layerType = new ActivationFunction*[numberLayers];

int j = 0;

for( int i = minimumParameters; i < parametersNeeded; i+=2, j++ )
  {
  layerType[j] = getLayerType(argv[i]);
  layerSize[j] = getInteger(argv[i+1]);
  }

if( Trace::atLevel(1) )
  {
  std::cout << numberLayers 
            << " layers structured (from input to output) as: " << std::endl;

  for( int i = 0; i < numberLayers; i++ )
    {
    std::cout << "    " << layerType[i]->getName() 
              << " (" << layerSize[i] << " "
              << (layerType[i] == onehot ? "categories" : "neurons")
              << ")" << std::endl;
    }

  std::cout << std::endl;

  std::cout << "epoch limit = "      << epochLimit << std::endl;

  std::cout << "specified rate = "   << rate << std::endl;

  std::cout << "goal = " << goal     << std::endl;

  std::cout << "mode = " << modeName[mode]   << std::endl;

  std::cout << "trace = "            << Trace::getLevel() << std::endl;
  }


if( argc < parametersNeeded )
  {
  std::cout << "no test file specified" << std::endl;
  exit(1);
  }

char* testFile = argv[parametersNeeded];

std::cout << "test file: " << testFile << std::endl;

int inputDimension;		     // dimension of input
int outputDimension;		     // dimension of output

std::list<Sample*> trainingSamples;  // list of pointers to training samples

getSamples(trainingFile, outputDimension, inputDimension, trainingSamples);

int nTrainingSamples = 0;

showAndCountSamples("training", trainingSamples, nTrainingSamples);


int inputDimension2;		     // dimension of input
int outputDimension2;		     // dimension of output

std::list<Sample*> testSamples;      // list of pointers to test samples

getSamples(testFile, outputDimension2, inputDimension2, testSamples);

if( inputDimension2 != inputDimension || outputDimension2 != outputDimension )
  {
  printf("error, the dimensions of the test vs. training file don't match");
  exit(1);
  }

int nTestSamples = 0;

showAndCountSamples("test", testSamples, nTestSamples);


// If the output is one-hot, then the output dimension should be 1 and
// and the number of neurons parameter is interpreted as the number of categories.

bool onehotOutput = (layerType[numberLayers-1]->getName()) == "onehot";

int lastLayerSize = layerSize[numberLayers-1];

if(  (!onehotOutput && (outputDimension != lastLayerSize))
  || (onehotOutput && (outputDimension != 1) ))
  {
  printf("error, the number of neurons in the last layer %d must agree with "
         "the output dimension %d.\n",
         layerSize[numberLayers-1],
         outputDimension);
  exit(1);
  }

std::cout << "\nOutput dimension is " << outputDimension 
          << (onehotOutput ? " (one-hot)" : "")
          << "." << std::endl;

std::cout << "\nInput dimension is " << inputDimension << "." << std::endl;

// Show and count the samples.

std::list<Sample*>::iterator sample = trainingSamples.begin();

if( Trace::atLevel(4) ) 
  {
  std::cout << "\nTraining samples are:\n" << std::endl;
  }

int nsamples = 0;
while( sample != trainingSamples.end() )
  {
  if( Trace::atLevel(4) ) 
    {
    (*sample)->show();
    std::cout << std::endl;
    }
  sample++;
  nsamples++;
  }


Network network(numberLayers, layerSize, layerType, inputDimension);

if( Trace::atLevel(4) ) 
  {
  std::cout << "\nInitial Weights:" << std::endl;

  network.showWeights("initial");
  }

if( Trace::atLevel(1) ) std::cout << "\nTraining begins with epoch 1." << std::endl;

int epoch = 1;

int signAgreement;

double sse;

double mse = 1+goal;
double oldmse = 2+goal;

double etaPlus = 1.2;	// for rprop
double etaMinus = 0.5;	

TERMINATION_REASON reason = NONE;

// The total number of output values across all samples and network outputs

int interval = 1;

// Training loop

while( reason == NONE )
  {
  sse = 0;
  signAgreement = 0;

  switch( mode )
    {
    case RPROP:
    case BATCH:
      network.clearAccumulation();
    case ONLINE:
      ;
    }

  for( std::list<Sample*>::iterator sample = trainingSamples.begin();
       sample != trainingSamples.end();
       sample++ 
     )
    {
    // forward propagation

    network.fire(**sample);

    double sampleSSE = network.computeError(**sample);

    sse += sampleSSE;

    if( Trace::atLevel(4) )
      {
      printf("\nforward output: ");
      network.showOutput();

      (*sample)->show();

      printf(", sample sse: % 6.3f\n", sampleSSE);
      }

    // backpropagation

    network.setSensitivity(**sample);

      switch( mode )
	{
	case RPROP:
          network.accumulateGradient(**sample);
	case BATCH:
          network.accumulateWeights(**sample, rate);
	case ONLINE:
          network.adjustWeights(**sample, rate);
	}

    if( Trace::atLevel(4) )
      {
      network.showWeights("current");
      }
    }

  switch( mode )
    {
    case RPROP:
      network.adjustByRprop(etaPlus, etaMinus);

    case BATCH:
      network.installAccumulation();

    case ONLINE:
      ;
    }

  mse = sse/nsamples;

  double usageError = 0;

  for( std::list<Sample*>::iterator sample = trainingSamples.begin();
       sample != trainingSamples.end();
       sample++)
    {
    // evaluation with "use"

    network.use(**sample);

    usageError += (network.computeUsageError(**sample) != 0);
    }

  if( Trace::atLevel(3) || (Trace::atLevel(2) && epoch%interval == 0) )
    {
    printf("\nend epoch %d, mse: %10.8f %s, usage error: %d/%d (%5.2f%%)\n",
          epoch, 
          mse, 
          mse < oldmse ? "decreasing" : "increasing",
          (int)usageError,
          nsamples,
          100*usageError/nsamples);
    }

  epoch++;

  if( mse <= goal )
    {
    reason = GOAL_REACHED;
    }
  else if( epoch > epochLimit )
    {
    reason = LIMIT_EXCEEDED;
    }

/* There is a problem using this with a one-hot output layer.
  else if( oldmse == mse )
    {
    reason = LACK_OF_PROGRESS;
    }
*/

  oldmse = mse;
  }

if( Trace::atLevel(1) ) 
  {
  std::cout << "\nTraining ends at epoch " << epoch << ", "
            << reasonName[reason] << "." << std::endl;
  }

std::cout << "\nFinal Weights:" << std::endl;

network.showWeights("final");

// Show performance on all samples

std::cout << "\nFinal performance on all test samples:" << std::endl;

signAgreement = 0;

mse = 0;

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

std::cout << "\nAfter " << epoch-1 << " epochs using "
          << modeName[mode];

if( mode != RPROP )
  {
  std::cout << " with learning rate " << rate;
  }

std::cout << ", " << reasonName[reason]
          << ", test mse = " << mse/nsamples
          << ", total usage error = " << usageError << "/" << nsamples
          << " (" << 100*usageError/nsamples << "%)"
          << std::endl;
}

