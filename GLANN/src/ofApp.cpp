#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    mWorker = new GLANN();
    mWorker->initGLANN(512);

    mNetwork = new ANNData( 512, 0.4, 1.0, 0.05);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    vector<float> input;
    for(int i = 0; i < 512; i++) input.push_back(sinf(i/512.0));
    vector<float> output = mWorker->propergateFW(input,mNetwork);

    vector<float> target = input;
    vector<float> error;
    for(int i = 0; i < 512; i++)
        error.push_back((target[i]-output[i])*output[i]*(1.0-output[i]));

    mWorker->propergateBW(input,error,mNetwork);

    mWorker->draw(mNetwork);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
