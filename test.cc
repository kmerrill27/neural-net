
const int	minimumParameters = 6;

int inputDimension;
int outputDimension;

void readWeights(FILE* weightFile) {

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

  char* weightFilename = argv[1];

  std::cout << "weight file: " << weightFilename << std::endl;

  FILE* weightFile;
  weightFile = fopen(weightFilename, "r");

  if ( weightFile )
  {
  printf("Weights will be loaded in: %s\n", weightFilename);
  }
  else
  {
  printf("Could not create weight file: %s\n", weightFilename);
  }

  int numberLayers = 2;
  int* layerSize = new int[numberLayers];
  ActivationFunction** layerType = new ActivationFunction*[numberLayers];
  layerSize[0] = 16;
  layerSize[1] = 1;
  layerType[0] = new Logsig();
  layerType[1] = new Purelin();

  char* testFile = "licks.in";

  std::cout << "test file: " << testFile << std::endl;

  // find way to setSensitivty not from Sample
  //network.setWeight();
}
