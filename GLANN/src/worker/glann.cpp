#include "glann.h"

GLANN::GLANN()
{
    //ctor
}

GLANN::~GLANN()
{
    //dtor
}

        //Loades the shader and inits the fbos
bool GLANN::initGLANN(){

}

//This function propergates an input Vector through a given ANN returning
//it's output (in case of rekurrent networks do this multible times on the
//output Data)
vector<float> GLANN::propergateFW(vector<float> input, ANNData* netToProcess){

}

//This function trains the network with the given Data
//consisting of an input (already propergated by the net)
//it's output and the desired/expected values named as target
vector<float> GLANN::propergateBW(vector<float> input,vector<float> output,
                                  vector<float> target,ANNData* netToProcess){

}
