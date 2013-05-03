#
# Makefile for 2-layer 1-output backpropagation network
#

# the exeuctable being made

EXE = bp


# the C++ compiler

CXX = g++		


# compiler flags

CXXFLAGS = -Wall -g


# libraries (math)

LIBS = -lm


# documentation generator

DOC = doxygen


all : $(EXE) 

doc :  Doxyfile	$(OBJS)		# Doxygen documentation
	$(DOC) Doxyfile

test : 				# If diff produces output, test failed
	$(EXE) < test2.in | diff - test2.out

clean : 
	rm -rf $(EXE) $(OBJS)

# object files

OBJS =  test.o \
        Hardlim.o \
        Hardlims.o \
        Network.o \
        Layer.o \
        Logsig.o \
        Neuron.o \
        Onehot.o \
        OnehotLayer.o \
        Purelin.o \
        Sample.o \
        Satlin.o \
        Satlins.o \
        Source.o \
        Tansig.o \
        Trace.o

$(EXE) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJS) $(LIBS)

test.o : test.cc
	$(CXX) -c $(CXXFLAGS) test.cc

Hardlim.o : Hardlim.h Hardlim.cc ActivationFunction.h
	$(CXX) -c $(CXXFLAGS) Hardlim.cc

Hardlims.o : Hardlims.h Hardlims.cc ActivationFunction.h
	$(CXX) -c $(CXXFLAGS) Hardlims.cc

Layer.o : Layer.h Layer.cc Neuron.o
	$(CXX) -c $(CXXFLAGS) Layer.cc

Logsig.o : Logsig.h Logsig.cc ActivationFunction.h
	$(CXX) -c $(CXXFLAGS) Logsig.cc

Network.o : Network.h Network.cc
	$(CXX) -c $(CXXFLAGS) Network.cc

Neuron.o : Neuron.h Neuron.cc
	$(CXX) -c $(CXXFLAGS) Neuron.cc

Onehot.o : Onehot.h Onehot.cc ActivationFunction.h
	$(CXX) -c $(CXXFLAGS) Onehot.cc

OnehotLayer.o : OnehotLayer.h OnehotLayer.cc Layer.o
	$(CXX) -c $(CXXFLAGS) OnehotLayer.cc

Purelin.o : Purelin.h Purelin.cc ActivationFunction.h
	$(CXX) -c $(CXXFLAGS) Purelin.cc

Sample.o : Sample.h Sample.cc
	$(CXX) -c $(CXXFLAGS) Sample.cc

Satlin.o : Satlin.h Satlin.cc ActivationFunction.h
	$(CXX) -c $(CXXFLAGS) Satlin.cc

Satlins.o : Satlins.h Satlins.cc ActivationFunction.h
	$(CXX) -c $(CXXFLAGS) Satlins.cc

Source.o : Source.h Source.cc
	$(CXX) -c $(CXXFLAGS) Source.cc

Tansig.o : Tansig.h Tansig.cc ActivationFunction.h
	$(CXX) -c $(CXXFLAGS) Tansig.cc

Trace.o : Trace.h Trace.cc
	$(CXX) -c $(CXXFLAGS) Trace.cc 
