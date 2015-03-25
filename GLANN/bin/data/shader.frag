// fragment shader

uniform sampler2DRect inputV;
uniform sampler2DRect errorV;

uniform sampler2DRect weightsM;
uniform sampler2DRect momentumM;

uniform float steepness;
uniform float learningrate;
uniform float momentum;

uniform int size;

uniform int shaderMode;

vec4 pack(const float value)
{
  const vec4 bitSh = vec4(256.0*256.0*256.0, 256.0*256.0, 256.0, 1.0);
  const vec4 bitMsk = vec4(0.0, 1.0/256.0, 1.0/256.0, 1.0/256.0);
  vec4 res = fract(value * bitSh);
  res -= res.xxyz * bitMsk;
  res.a = 1.0;
  res.r = value;
  return res;
}
float unpack(const vec4 value)
{
  const vec4 bitSh = vec4(1.0/(256.0*256.0*256.0), 1.0/(256.0*256.0), 1.0/256.0, 1.0);
  return value.r;//(dot(value, bitSh));
}
//Map from 0..1 to -1..1 and unmap back
float map(float val){
    return ((2.0*val)-1.0);
}
float unmap(float val){
    return (0.5*(val+1.0));
}

//sigmoide activation function
float sigmoid(float x){
    return (1.0 / (1.0+exp(-x*steepness)));
}
//Bcs. of 1.0 can't be packed (BUG) i've to clip before
float clip(float val){
    if (val >= 1.0) return 0.9999;
    if (val <= 0.0) return 0.0000;
    return val;
}

void main()
{
    // gl_FragCoord contains the window relative coordinate for the fragment.
    //gl_FragColor = vec4(xTex,yTex,1.0,1.0);

    //Multiply Matrix with InputVector
    if(shaderMode == 1){
        vec4 weightsColor = texture(weightsM,gl_FragCoord.xy);
        float weightsValue = map(unpack(weightsColor));

        vec4 inputColor = texture(inputV,gl_FragCoord.xy);
        float inputValue = unpack(inputColor);

        gl_FragColor = pack(unmap(weightsValue*inputValue));
    }

    //Sum and Sig the rows
    if(shaderMode == 2){
        if(gl_FragCoord.x < 1.0){
            float sumPixels = 0.0;
            for(int i = 0; i < size; i++){
                sumPixels += map(unpack(texture(weightsM,vec2(i,gl_FragCoord.y))));
            }
            gl_FragColor = pack(clip(sigmoid(sumPixels)));
        }
    }

    //Backpropagate Error
    if(shaderMode == 3){

        if(gl_FragCoord.y < 1.0){

        }

    }

    //Correct weights
    if(shaderMode == 4){
        vec4 weightsColor = texture(weightsM,gl_FragCoord.xy);
        float weightsValue = map(unpack(weightsColor));

        vec4 errorColor = texture(errorV,vec2(gl_FragCoord.y,1));
        float errorValue = map(unpack(errorColor));

        vec4 inputColor = texture(inputV,vec2(gl_FragCoord.x,1));
        float inputValue = unpack(inputColor);

        gl_FragColor = pack(clip(unmap(learningrate * errorValue * inputValue * 30.0 + weightsValue)));

    }

    if(shaderMode == -1){
        vec4 pixelColor = texture(weightsM,gl_FragCoord.xy);
        float pixelValue = map(unpack(pixelColor));
        if(pixelValue >= 0.0) gl_FragColor = vec4(pixelValue,0.0,0.0,1.0);
        else gl_FragColor = vec4(0.0,0.0,abs(pixelValue),1.0);
    }

}
