#include "anndata.h"

ANNData::ANNData(int size)
{
    this->size = size;

    //Init the Weights to small random Values
    mWeights.allocate(size, size, OF_IMAGE_COLOR_ALPHA);

    int i = 0;
    while ( i < mWeights.getPixelsRef().size() ) {
        ofColor pixelColor = GLANNTools::pack(ofRandom(0.45,0.55));
        mWeights.setColor(i,pixelColor);
        i++;
    }
    mWeights.reloadTexture();

    //Init the Momentum to "zero" = 0.5
    mMomentum.allocate(size,size,OF_IMAGE_COLOR_ALPHA);
    int j = 0;
    while ( i < mMomentum.getPixelsRef().size() ) {
        ofColor pixelColor = GLANNTools::pack(0.5);
        mMomentum.setColor(i,pixelColor);
        i++;
    }
    mMomentum.reloadTexture();

}

ANNData::~ANNData()
{
    //dtor
}

ANNData::ANNData(const ANNData& other)
{
    //copy ctor
}
