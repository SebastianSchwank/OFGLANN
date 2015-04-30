#ifndef ANNDATA_H
#define ANNDATA_H

#include "ofImage.h"
#include "ofFbo.h"

#include "../glanntools.h"

class ANNData
{
    public:
        ANNData(int inputs,int outputs, float learningRate, float steepness, float momentum);
        ANNData(ofImage preCalcWeightsImage);
        virtual ~ANNData();
        ANNData(const ANNData& other);

        float getSteepness();
        float getLearningRate();
        float getMomentum();

        int getnumInputs();
        int getnumOutputs();

        ofImage mWeights;
        ofImage mMomentum;

        ofImage mInput;
        ofImage mOutput;
        ofImage mError;

        ofFbo *mFbo;

    private:

        float learningRate;
        float steepness;
        float momentum;

        int inputs,outputs;
};

#endif // ANNDATA_H
