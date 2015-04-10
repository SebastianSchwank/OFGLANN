#ifndef ANNDATA_H
#define ANNDATA_H

#include "ofImage.h"

#include "../glanntools.h"

class ANNData
{
    public:
        ANNData(int size, float learningRate, float steepness, float momentum);
        ANNData(ofImage preCalcWeightsImage);
        virtual ~ANNData();
        ANNData(const ANNData& other);

        float getSteepness();
        float getLearningRate();
        float getMomentum();

        int getSize();

        ofImage mWeights;
        ofImage mMomentum;

    private:

        float learningRate;
        float steepness;
        float momentum;

        int size;
};

#endif // ANNDATA_H
