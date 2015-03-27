#ifndef GLANNTOOLS_H
#define GLANNTOOLS_H

#include "ofColor.h"

class GLANNTools
{
    public:
        GLANNTools();
        virtual ~GLANNTools();

        static ofFloatColor pack(float value);
        static float        unpack(ofFloatColor pixel);

    protected:
    private:
        static float fract(float f);
};

#endif // GLANNTOOLS_H
