// fragment shader

#version 140

varying vec2 TexCoord;

uniform sampler2D inputV;
uniform sampler2D errorV;

uniform sampler2D weightsM;
uniform sampler2D momentumM;

uniform float steepness;
uniform float learningrate;
uniform float momentum;

uniform int size;

uniform int shaderMode;


vec4 pack( float v ) {
  vec4 enc = vec4(1.0, 255.0, 65025.0, 160581375.0) * v;
  enc = fract(enc);
  enc -= enc.yzww * vec4(1.0/255.0,1.0/255.0,1.0/255.0,0.0);
  enc.a = 1.0;
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
        vec4 weightsColor = texture(weightsM,TexCoord);
        float weightsValue = map(unpack(weightsColor));

        vec4 inputColor = texture(inputV,vec2(TexCoord.x,0.0));
        float inputValue = unpack(inputColor);

        gl_FragColor = pack(unmap(weightsValue * inputValue));
    }

    //Sum and Sig the rows
    if(shaderMode == 2){
        if(gl_FragCoord.x <= 1.0){
            float sumPixels = 0.0;
            for(float i = 1.0/float(size*2); i <= 1.0; i+= 1.0/float(size)){
                sumPixels += map(unpack(texture(weightsM,vec2(i,TexCoord.y))));
            }
            gl_FragColor = pack(clip(sigmoid(sumPixels)));
        }
    }

    //Backpropagate Error
    if(shaderMode == 3){

        if(gl_FragCoord.y < 1.0){

        }

    }

    //calculate Momentum
    if(shaderMode == 4){
        vec4 weightsColor = texture(weightsM,TexCoord.xy);
        float weightsValue = map(unpack(weightsColor));
        //weightsValue = weightsValue * (1.0- abs(weightsValue*weightsValue*weightsValue*weightsValue*weightsValue*weightsValue));

        vec4 errorColor = texture(errorV,vec2(TexCoord.y,0.0));
        float errorValue = map(unpack(errorColor));

        vec4 inputColor = texture(inputV,vec2(TexCoord.x,0.0));
        float inputValue = unpack(inputColor);

        vec4 momentumColor = texture(momentumM,TexCoord.xy);
        float momentumValue = map(unpack(momentumColor));

        gl_FragColor = pack(clip(unmap(inputValue * learningrate * errorValue + momentum * momentumValue)));
    }

    //Correct Weights
    if(shaderMode == 5){
        vec4 weightsColor = texture(weightsM,TexCoord.xy);
        float weightsValue = map(unpack(weightsColor));

        vec4 momentumColor = texture(momentumM,TexCoord.xy);
        float momentumValue = map(unpack(momentumColor));

        gl_FragColor = pack(clip(unmap(weightsValue + momentumValue)));
    }

    if(shaderMode == -1){
        vec4 pixelColor = texture(weightsM,TexCoord);
        float pixelValue = map(unpack(pixelColor));
        if(pixelValue >= 0.0) gl_FragColor = vec4(pixelValue,0.0,0.0,1.0);
        else gl_FragColor = vec4(0.0,0.0,abs(pixelValue),1.0);
    }

}
