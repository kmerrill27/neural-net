// file:    helper.h
// author:  Kim Merrill
// purpose: Header file for helper methods

#include <list>
#include <iostream>
#include <fstream>

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

/**
 * Get layer type by matching name to string.
 */

ActivationFunction* getLayerType(std::string name);

/**
 * Get samples from standard input.
 */

void getSamples(char* inputFile, int& outputDimension, int& inputDimension, std::list<Sample*>& listOfSamples);

/**
 * Run samples through net and save output values.
 */

double runSamples(double& mse, std::list<Sample*>& testSamples, Network& network, std::ofstream& outputStream);
