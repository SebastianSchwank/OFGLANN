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
    delete &fbo;
}

//Loades the shader and inits the fbos
bool GLANN::initGLANN(int FboSize){
    shader.load("shader");

    mFBOSize = FboSize;

    fbo.allocate(FboSize,FboSize);
    fbo.begin();
        ofClear(0,0,0,255);
    fbo.end();

    mCurrError.allocate(FboSize,1,OF_IMAGE_COLOR_ALPHA);
    mCurrInput.allocate(FboSize,1,OF_IMAGE_COLOR_ALPHA);
}

//This function propergates an input Vector through a given ANN returning
//it's output (in case of rekurrent networks do this multible times on the
//output Data)
vector<float> GLANN::propergateFW(vector<float> input, ANNData* netToProcess){

    //Pack the Input data to Image
    for(int i = 0; i < mCurrInput.width; i++)
        mCurrInput.setColor(i,0,GLANNTools::pack(input[i]));

    mCurrInput.reloadTexture();
    mCurrInput.update();

    shader.begin();

        shader.setUniform1i("shaderMode",1);

        shader.setUniformTexture("weightsM",netToProcess->mWeights.getTextureReference(),1);
        shader.setUniformTexture("inputV",mCurrInput.getTextureReference(),2);

        shader.setUniform1i("size",mFBOSize);

        fbo.begin();
            ofRect(0, 0, mFBOSize, mFBOSize);
        fbo.end();

    shader.end();

    ofPixels MulMartix;
    fbo.readToPixels(MulMartix);

    ofImage MatrixImage;
    MatrixImage.setFromPixels(MulMartix);
    MatrixImage.update();

    shader.begin();

        shader.setUniform1i("shaderMode",2);

        //Feedback into the "Pipeline"
        shader.setUniformTexture("weightsM",MatrixImage.getTextureReference(),1);

        shader.setUniform1f("steepness",netToProcess->getSteepness());
        shader.setUniform1i("size",mFBOSize);

        fbo.begin();
            ofRect(0, 0, mFBOSize, mFBOSize);
        fbo.end();

    shader.end();

    ofPixels Output;
    fbo.readToPixels(Output);

    vector<float> output;
    for(int i = 0; i < mFBOSize; i++)
        output.push_back(GLANNTools::unpack(Output.getColor(0,i)));

    return output;
}

//This function trains the network with the given Data
//consisting of an input (already propergated by the net)
//it's output and the desired/expected values named as target
vector<float> GLANN::propergateBW(vector<float> input, vector<float> error
                                  ,ANNData* netToProcess){
    //Pack the Input data to Image
    for(int i = 0; i < mCurrInput.width; i++)
        mCurrInput.setColor(i,0,GLANNTools::pack(input[i]));

    mCurrInput.reloadTexture();
    mCurrInput.update();

    //Pack the Output data to Image
    for(int i = 0; i < mCurrError.width; i++)
        mCurrError.setColor(i,0,GLANNTools::pack((1.0+error[i])/2.0));

    mCurrError.reloadTexture();
    mCurrError.update();

/*
    shader.begin();
        //Set shader to propergate the Error Backwards through the net
        shader.setUniform1i("shaderMode",3);

        shader.setUniformTexture("weightsM",netToProcess->mWeights.getTextureReference(),1);
        shader.setUniformTexture("inputV",mCurrInput.getTextureReference(),2);
        shader.setUniformTexture("errorV",mCurrError.getTextureReference(),3);

        shader.setUniform1i("size",mFBOSize);

        fbo.begin();
            ofRect(0, 0, mFBOSize, mFBOSize);
        fbo.end();

    shader.end();
*/
    shader.begin();
        //Set shader to propergate the Error Backwards through the net
        shader.setUniform1i("shaderMode",4);

        shader.setUniformTexture("weightsM",netToProcess->mWeights.getTextureReference(),1);
        shader.setUniformTexture("inputV",mCurrInput.getTextureReference(),2);
        shader.setUniformTexture("errorV",mCurrError.getTextureReference(),3);

        shader.setUniform1i("size",mFBOSize);
        shader.setUniform1f("learningrate",netToProcess->getLearningRate());

        fbo.begin();
            ofRect(0, 0, mFBOSize, mFBOSize);
        fbo.end();

    shader.end();

    ofPixels corrWeights;
    fbo.readToPixels(corrWeights);

    netToProcess->mWeights.setFromPixels(corrWeights);

}

void GLANN::draw(ANNData* netToProcess){

    ofClear(0, 0, 0, 0);

    shader.begin();

        shader.setUniform1i("shaderMode",-1);
        shader.setUniform1i("size",mFBOSize);

        shader.setUniformTexture("weightsM",netToProcess->mWeights.getTextureReference(),1);

        ofRect(0, 0, mFBOSize, mFBOSize);

    shader.end();
}
