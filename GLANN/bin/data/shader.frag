// fragment shader

#version 150

uniform sampler2DRect inputV;
uniform sampler2DRect outputV;
uniform sampler2DRect targetV;

uniform sampler2DRect weightsM;
uniform sampler2DRect momentumM;

uniform float steepness;
uniform float learningrate;
uniform float momentum;

uniform int size;

uniform int shaderMode;

vec4 pack( float v ) {
  vec4 enc = vec4(1.0, 255.0, 65025.0, 160581375.0) * v;
  enc = fract(enc);
  enc -= enc.yzww * vec4(1.0/255.0,1.0/255.0,1.0/255.0,0.0);
  return enc;
}

float unpack( vec4 rgba ) {
  return dot( rgba, vec4(1.0, 1.0/255.0, 1.0/65025.0, 1.0/160581375.0) );
}

//Map from 0..1 to -1..1 and unmap back
float map(float val){
    return ((2.0*val)-1.0);
}

float unmap(float val){
    return (0.5*(val+1.0));
}

void main()
{
    // gl_FragCoord contains the window relative coordinate for the fragment.
    //gl_FragColor = vec4(xTex,yTex,1.0,1.0);

    if(shaderMode == -1){
        vec4 pixelColor = texture(weightsM,gl_FragCoord.xy);
        float pixelValue = map(unpack(pixelColor));
        if(pixelValue >= 0) gl_FragColor = vec4(pixelValue,0.0,0.0,1.0);
        else gl_FragColor = vec4(0.0,0.0,abs(pixelValue),1.0);
    }

}
