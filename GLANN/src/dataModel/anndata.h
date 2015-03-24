#ifndef ANNDATA_H
#define ANNDATA_H

#include "ofImage.h"

#include "../glanntools.h"

class ANNData
{
    public:
        ANNData(int size);
        ANNData(ofImage preCalcWeightsImage);
        virtual ~ANNData();
        ANNData(const ANNData& other);

    //private:
        ofImage mWeights;
        ofImage mMomentum;

        float learningRate;
        float steepness;
        float momentum;

        int size;
};

#endif // ANNDATA_H
