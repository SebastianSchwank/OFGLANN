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

    //vector<float> input;
    //for(int i = 0; i < 50; i++) input.push_back(0.9999);
    //vector<float> output = mWorker->propergateFW(input,mNetwork);

    //for(int i = 0; i < 50; i++) cout << output[i] << "|";
    //cout << "\n";

    ofClear(0,0,0,0);

    //ofRect(0,0,512,512);

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
