#include "glann.h"

GLANN::GLANN()
{
    //ctor
    mFBOSize = -1;
}

GLANN::~GLANN()
{
    //dtor
    delete &shader;
}

//Loades the shader and inits the fbos
bool GLANN::initGLANN(){
    shader.load("shader");
}

//This function propergates an input Vector through a given ANN returning
//it's output (in case of rekurrent networks do this multible times on the
//output Data)
vector<float> GLANN::propergateFW(vector<float> input, ANNData* netToProcess){

    //Pack the Input data to Image
    for(int i = 0; i < netToProcess->mInput.width; i++)
        netToProcess->mInput.setColor(i,0,GLANNTools::pack(input[i]));

    netToProcess->mInput.reloadTexture();
    netToProcess->mInput.update();

    shader.begin();

        shader.setUniform1i("shaderMode",1);

        shader.setUniform1i("InputSize",netToProcess->getnumInputs());
        shader.setUniform1i("OutputSize",netToProcess->getnumOutputs());

        shader.setUniformTexture("weightsM",netToProcess->mWeights.getTextureReference(),1);
        shader.setUniformTexture("inputV",netToProcess->mInput.getTextureReference(),2);

        netToProcess->mFbo->begin();
            ofClear(0,0,0,0);
            netToProcess->mWeights.draw(0,0);
        netToProcess->mFbo->end();

    shader.end();

    //fbo.draw(0,0);

    ofPixels MulMartix;
    netToProcess->mFbo->readToPixels(MulMartix);

    ofImage MatrixImage;
    MatrixImage.setFromPixels(MulMartix);
    MatrixImage.update();

    shader.begin();

        shader.setUniform1i("shaderMode",2);

        //Feedback into the "Pipeline"
        shader.setUniformTexture("weightsM",MatrixImage.getTextureReference(),1);

        shader.setUniform1f("steepness",netToProcess->getSteepness());

        shader.setUniform1i("InputSize",netToProcess->getnumInputs());
        shader.setUniform1i("OutputSize",netToProcess->getnumOutputs());

        netToProcess->mFbo->begin();
            ofClear(0,0,0,0);
            netToProcess->mWeights.draw(0,0);
        netToProcess->mFbo->end();

    shader.end();

    ofPixels Output;
    netToProcess->mFbo->readToPixels(Output);

    vector<float> output;
    for(int i = 0; i < netToProcess->mOutput.height; i++)
        output.push_back(GLANNTools::unpack(Output.getColor(0,i)));

    return output;
}

//This function trains the network with the given Data
//consisting of an input (already propergated by the net)
//it's output and the desired/expected values named as target
vector<float> GLANN::propergateBW(vector<float> input, vector<float> error
                                  ,ANNData* netToProcess){
    //Pack the Input data to Image
    for(int i = 0; i < netToProcess->mInput.width; i++)
        netToProcess->mInput.setColor(i,0,GLANNTools::pack(input[i]));

    netToProcess->mInput.reloadTexture();
    netToProcess->mInput.update();

    //Pack the Output data to Image
    for(int i = 0; i < netToProcess->mError.height; i++)
        netToProcess->mError.setColor(0,i,GLANNTools::pack((1.0+error[i])/2.0));

    netToProcess->mError.reloadTexture();
    netToProcess->mError.update();


    shader.begin();
        //Set shader to propergate the Error Backwards through the net
        shader.setUniform1i("shaderMode",3);

        shader.setUniformTexture("weightsM",netToProcess->mWeights.getTextureReference(),1);
        shader.setUniformTexture("inputV",netToProcess->mInput.getTextureReference(),2);
        shader.setUniformTexture("errorV",netToProcess->mError.getTextureReference(),3);

        shader.setUniform1i("InputSize",netToProcess->getnumInputs());
        shader.setUniform1i("OutputSize",netToProcess->getnumOutputs());

        netToProcess->mFbo->begin();
            ofClear(0,0,0,0);
            netToProcess->mWeights.draw(0,0);
        netToProcess->mFbo->end();

    shader.end();

    ofPixels fboPixels;
    netToProcess->mFbo->readToPixels(fboPixels);
    error.clear();
    for(int i = 0; i < netToProcess->mInput.width; i++)
        error.push_back(2.0*GLANNTools::unpack(fboPixels.getColor(i,0))-1.0);

    shader.begin();
        //Set shader to calculate the current momentum
        shader.setUniform1i("shaderMode",4);

        shader.setUniformTexture("weightsM",netToProcess->mWeights.getTextureReference(),1);
        shader.setUniformTexture("momentumM",netToProcess->mMomentum.getTextureReference(),2);
        shader.setUniformTexture("inputV",netToProcess->mInput.getTextureReference(),3);
        shader.setUniformTexture("errorV",netToProcess->mError.getTextureReference(),4);

        shader.setUniform1i("InputSize",netToProcess->getnumInputs());
        shader.setUniform1i("OutputSize",netToProcess->getnumOutputs());

        shader.setUniform1f("learningrate",netToProcess->getLearningRate());
        shader.setUniform1f("momentum",netToProcess->getMomentum());

        netToProcess->mFbo->begin();
            ofClear(0,0,0,0);
            netToProcess->mWeights.draw(0,0);
        netToProcess->mFbo->end();

    shader.end();

    fboPixels.clear();
    netToProcess->mFbo->readToPixels(fboPixels);
    netToProcess->mMomentum.setFromPixels(fboPixels);

    shader.begin();
        //Set shader to correct the weights with the momentum
        shader.setUniform1i("shaderMode",5);

        shader.setUniformTexture("weightsM",netToProcess->mWeights.getTextureReference(),1);
        shader.setUniformTexture("momentumM",netToProcess->mMomentum.getTextureReference(),2);

        shader.setUniform1i("InputSize",netToProcess->getnumInputs());
        shader.setUniform1i("OutputSize",netToProcess->getnumOutputs());

        netToProcess->mFbo->begin();
            ofClear(0,0,0,0);
            netToProcess->mWeights.draw(0,0);
        netToProcess->mFbo->end();

    shader.end();

    fboPixels.clear();
    netToProcess->mFbo->readToPixels(fboPixels);
    netToProcess->mWeights.setFromPixels(fboPixels);

    return error;
}

void GLANN::draw(ANNData* netToProcess,int x, int y){

    shader.begin();

        shader.setUniform1i("shaderMode",-1);
        shader.setUniform1i("size",mFBOSize);

        shader.setUniformTexture("weightsM",netToProcess->mWeights.getTextureReference(),1);

        netToProcess->mWeights.draw(x,y);

    shader.end();
}
