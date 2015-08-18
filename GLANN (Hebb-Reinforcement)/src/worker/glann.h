#ifndef GLANN_H
#define GLANN_H

#include "ofShader.h"
#include "ofImage.h"
#include "ofFbo.h"
#include "ofColor.h"
#include "ofGraphics.h"

#include "..\dataModel\anndata.h"

#include <Vector>

class GLANN
{
    public:
        GLANN();
        virtual ~GLANN();

        //Loades the shader and inits the fbos
        bool initGLANN();

        //This function propergates an input Vector through a given ANN returning
        //it's output (in case of rekurrent networks do this multible times on the
        //output Data)
        vector<float> propergateFW(vector<float> input, ANNData* netToProcess);

        //This function trains the network with the given Data
        //consisting of an input (already propergated by the net)
        //it's output and the desired/expected values named as target
        vector<float> propergateBW(vector<float> input,
                                   vector<float> output,
                                    ANNData* netToProcess);

        void draw(ANNData* netToProcess,int x,int y);

    protected:
    private:
		ofShader shader;

		int      mFBOSize;
};

#endif // GLANN_H
