#include "anndata.h"

ANNData::ANNData(int size, float learningRate, float steepness, float momentum)
{
    this->size = size;
    this->learningRate = learningRate;
    this->steepness = steepness;
    this->momentum = momentum;

    //Init the Weights to small random Values
    mWeights.allocate(size, size, OF_IMAGE_COLOR_ALPHA);
    for(int x = 0; x < size; x++){
        for(int y= 0; y < size; y++){
            ofColor pixelColor = GLANNTools::pack(ofRandom(0.45,0.55));
            mWeights.setColor(x,y,pixelColor);
        }
    }
    mWeights.update();
    mWeights.reloadTexture();

    //Init the Momentum to "zero" = 0.5
    mMomentum.allocate(size,size,OF_IMAGE_COLOR_ALPHA);
    for(int x = 0; x < size; x++){
        for(int y= 0; y < size; y++){
            ofColor pixelColor = GLANNTools::pack(0.5);
            mMomentum.setColor(x,y,pixelColor);
        }
    }
    mMomentum.update();
    mMomentum.reloadTexture();

}

float ANNData::getSteepness(){
    return steepness;
}

float ANNData::getLearningRate(){
    return learningRate;
}

float ANNData::getMomentum(){
    return momentum;
}

int ANNData::getSize(){
    return size;
}

ANNData::~ANNData()
{
    //dtor
}

ANNData::ANNData(const ANNData& other)
{
    //copy ctor
}
