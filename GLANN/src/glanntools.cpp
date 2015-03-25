#include "glanntools.h"

GLANNTools::GLANNTools()
{
    //ctor
}

GLANNTools::~GLANNTools()
{
    //dtor
}

ofFloatColor GLANNTools::pack (float v) {
  float enc[4];
  enc [0] = fract (1.0f         * v);
  enc [1] = fract (255.0f       * v);
  enc [2] = fract (65025.0f     * v);
  enc [3] = fract (160581375.0f * v);

  enc [0] -= enc [1] * 1.0f/255.0f;
  enc [1] -= enc [2] * 1.0f/255.0f;
  enc [2] -= enc [3] * 1.0f/255.0f;

  if(enc[0] < 0.0){
      enc[0] = 0.0;
  }

  if(enc[1] < 0.0){
      enc[1] = 0.0;
  }

  if(enc[2] < 0.0){
      enc[2] = 0.0;
  }


  if(enc[3] < 0.0){
      enc[3] = 0.0;
  }

  ofFloatColor resCol;
  resCol.set(enc[0],enc[1],enc[2],enc[3]);

  return resCol;
}

float GLANNTools::unpack(ofFloatColor pixelColor){
    float r = pixelColor.r;
    float g = pixelColor.g;
    float b = pixelColor.b;
    float a = pixelColor.a;
    float scaled = (float)(    r
                             + g*(1.0f / 255.0f)
                             + b*(1.0f / 65025.0f)
                             + a*(1.0f / 160581375.0f));
    return scaled;
}

// C++ offers `modf (...)`, which does the same thing, but this is simpler.
float GLANNTools::fract (float f) {
  return f-(long)f;
}
