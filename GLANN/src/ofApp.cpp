#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetVerticalSync (false);
    ofSetFrameRate(150);
    ofEnableAlphaBlending();

    mWorker = new GLANN();
    mWorker->initGLANN(128);

    mNetwork = new ANNData( 128, 0.2, 0.1, 0.05);

    frameCounter = 0;
    train = true;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    frameCounter++;

    vector<float> input;
    for(int i = 0; i < 128; i++)
        input.push_back((1.0+sinf(((frameCounter % 128)/10.0)*i/128.0))/2.0);

    vector<float> output = mWorker->propergateFW(input,mNetwork);

    vector<float> target;
    for(int i = 0; i < 128; i++)
        target.push_back(0.5);
    target[frameCounter % 128] = 0.9;

    vector<float> error;
    for(int i = 0; i < 128; i++)
        error.push_back((target[i]-output[i])*output[i]*(1.0-output[i]));

    if(train) mWorker->propergateBW(input,error,mNetwork);

    //mWorker->draw(mNetwork);

   ofSetColor(255);
    for(int i = 1; i < 128; i++)
        ofLine(128+i,output[i-1]*50+25,128+i+1,output[i]*50+25);

    ofSetColor(255,0,0);
    for(int i = 1; i < 128; i++)
        ofLine(128+i,error[i-1]*50+25,128+i+1,error[i]*50+25);

    ofSetColor(255);
    for(int i = 1; i < 128; i++)
        ofLine(128+i,input[i-1]*50+75,128+i+1,input[i]*50+75);

    ofSetColor(255,0,0);
    for(int i = 1; i < 128; i++)
        ofLine(128+i,target[i-1]*50+75,128+i+1,target[i]*50+75);

    if(frameCounter % 128 == 0) frameCounter = 0;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    train = !train;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
