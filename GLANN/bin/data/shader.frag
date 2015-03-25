// fragment shader

#version 120

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

void main()
{
    // gl_FragCoord contains the window relative coordinate for the fragment.
    gl_FragColor = texture(weightsM,gl_FragCoord.xy);
    //gl_FragColor = vec4(xTex,yTex,1.0,1.0);

    //if(shaderMode == -1){
    //    gl_FragColor = texture2D(weightsM,vec2(xTex,yTex));
        //gl_FragColor = vec4(xTex,yTex,0.0,1.0);
    //}

}
