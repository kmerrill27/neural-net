// file:    Source.h
// author:  Robert Keller
// purpose: Header file for Source class

/**
 * A Source is a source of values to a Layer or Neuron
 *
 * Specifically, a Source can either be 
 *
 * <pre>
 * <ul>
 *     <li>a Sample: the values are the input values of the Sample
 *
 *     <li>a HiddenLayer: the values are the outputs of the HiddenLayer
 * </ul>
 * </pre>
 */

#ifndef __Source__

class Source
{
protected:

/**
 * the number of values in the source
 */

int dim;


public:

/**
 * constructor
 */

Source();


/**
 * Get the the ith value of this Source.
 */

virtual double get(int i) const = 0;


/**
 * destructor
 */

virtual ~Source();


}; // class Source

#define __Source__
#endif
