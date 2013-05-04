README:


Run a multi-level perceptron on a given set of samples, loading in weights from a file.

Compile on a Linux system with 

make

Sample run script to run network with attributes specified in licks.weights.save on samples
in test.sample.in and save outputs to outputs.save:

./test licks.weights.save test.sample.in outputs.save


--------------------------------------------------------------------


Train a multi-level perceptron by backpropagation or resilient backpropagation (rprop).

NOTE: To retrain the network and save new weights to licks.weights.save, change makefile
to compile bp instead of test. Then:

A sample run script is

./licks.rprop.sh

which will train the network on the licks in all.in


For usage of the program, type

./bp

to get a display of command-line parameters, or examine licks.rprop.sh to see
an example of the parameters.


Currently weights are not saved in a file. However they can be dumped out at the end.
Someone needs to add code to save them in a file, and possibly to reload them.


The meaning of the lines in licks.in are as follows:

The first line is the number of outputs (1) and the number of inputs (424).
These are created from Impro-Visor.
Each of the remaining lines is a sample.

The first number is a quality rating 0 to 1.0 added by a human.
The next 24 bits are an encoding of two chords, such as Dm7 G7.
Each chord uses 12 bits and the 1 bits indicate which notes are in the chord,
starting with C and going up chromatically

That leaves 400 bits for melody. The melody is two bars worth of eighth note slots
i.e. 16 slots, giving 400/16 = 25 bits per slot. 

In one slot, the first 24 bits are a 1-hot encoding of chromatic pitches over 2 octaves,
starting with C. The remaining bit indicates whether a note is tied over.

Example: The following line represents a single graded melody.

0.9 1 0 1 0 0 1 0 0 0 1 0 0 0 0 1 0 0 1 0 1 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0

In Impro-Visor leadsheet notation, it would be represented

a+8 b+8 g+8 a+8 f+8 d+8 c+8 b8 g#8 b8 g8 f8 g8 g#8 e8 c8

This text string can be entered into Impro-Visor to see the melody on the staff.
