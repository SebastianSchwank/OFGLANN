#include "glanntools.h"

GLANNTools::GLANNTools()
{
    //ctor
}

GLANNTools::~GLANNTools()
{
    //dtor
}

ofColor GLANNTools::pack (float v) {
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

  ofColor resCol;
  resCol.set(enc[0]*255,enc[1]*255,
             enc[2]*255,enc[3]*255);

  return resCol;
}

float GLANNTools::unpack(ofColor pixelColor){
    float r = pixelColor.r/255.0;
    float g = pixelColor.g/255.0;
    float b = pixelColor.b/255.0;
    float a = pixelColor.a/255.0;
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
