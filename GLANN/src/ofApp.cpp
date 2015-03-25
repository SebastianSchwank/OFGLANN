#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    mWorker = new GLANN();
    mWorker->initGLANN(512);

    mNetwork = new ANNData( 512, 0.1, 1.0, 0.05);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    vector<float> input;
    for(int i = 0; i < 512; i++) input.push_back(ofRandom(0.0,0.9999));
    vector<float> output = mWorker->propergateFW(input,mNetwork);

    for(int i = 0; i < 5; i++) cout << output[i] << "|";
    cout << "\n";
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
