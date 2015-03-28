// vertex shader

#version 140

varying vec2  TexCoord;

void main(){
    TexCoord = gl_MultiTexCoord0.st;
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}
