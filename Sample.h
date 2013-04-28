// file:    Sample.h
// author:  Robert Keller
// purpose: Header file for Sample class

#ifndef __Sample__
#define __Sample__

#include "Source.h"

/**
 * A Sample holds one input vector and the corresponding output.
 * It is one kind of Source for a Neuron
 */

class Sample : public Source
{
private:

int inputDim, outputDim;

/**
 * array of sample output values
 */

double *output;


/**
 * array of sample input values
 */

double *input;


public:

/**
 * Create a sample, with the indicated dimensions.
 */

Sample(int _outputDim, int _inputDim);


/**
 * Set the value of the ith output for this sample.
 */

void setOutput(int i, double _output);


/**
 * Set the value of the ith input for this sample.
 */

void setInput(int i, double _input);


/**
 * Get the desired value of the output.
 */

double getOutput(int i) const;


/**
 * Get the ith input value of this sample.
 */

double getInput(int i) const;


/**
 * Get the ith input value of this sample (as a Source).
 */

double get(int i) const;


/**
 * Get the output dimension.
 */

int getOutputDimension() const;


/**
 * Get the input dimension.
 */

int getInputDimension() const;


/**
 * Show information about this sample on the output stream.
 */

void show();


/**
 * destructor
 */

virtual ~Sample();

}; // class Sample

#endif
