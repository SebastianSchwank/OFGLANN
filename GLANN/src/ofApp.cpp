#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofDisableArbTex();
    ofEnableAlphaBlending();
    ofSetFrameRate(60);
    glEnable(GL_DEPTH_TEST);

    netSize = 128;
    mWorker = new GLANN();
    mWorker->initGLANN(netSize);

    mNetwork = new ANNData( netSize, 0.2, 1.0, 0.00);

    frameCounter = 0;
    train = true;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    frameCounter++;

    int pos = frameCounter;

    vector<float> input;
    input.clear();
    for(int i = 0; i < netSize; i++)
        //input.push_back((1.0+sin(((pos % netSize)/5.0)*i/netSize))/2.1);
        input.push_back(0.0);

    //Don't forget the bias !
    input[0] = 0.9999;

    input[pos % netSize] = 0.999;
    //input[pos-1 % netSize] = 0.999;


    vector<float> output;
    output.clear();
    output = mWorker->propergateFW(input,mNetwork);

    vector<float> target;
    target.clear();
    for(int i = 0; i < netSize; i++)
        target.push_back((1.0+sin(((pos % netSize)/5.0)*i/netSize))/2.1);

    float sumQuadError = 0.0;
    vector<float> error;
    error.clear();
    for(int i = 0; i < netSize; i++){
        error.push_back(4.0*(target[i]-output[i])*output[i]*(1.0-output[i]));
        sumQuadError += (target[i]-output[i])*(target[i]-output[i]);
    }
    globError.push_back(sumQuadError);

    if(train) mWorker->propergateBW(input,error,mNetwork);


    mWorker->draw(mNetwork);

   ofSetColor(255);
    for(int i = 1; i < netSize; i++)
        ofLine(netSize+i,output[i-1]*50+25,netSize+i+1,output[i]*50+25);

    ofSetColor(255,0,0);
    for(int i = 1; i < netSize; i++)
        ofLine(netSize+i,error[i-1]*50+25,netSize+i+1,error[i]*50+25);

    ofSetColor(255);
    for(int i = 1; i < netSize; i++)
        ofLine(netSize+i,input[i-1]*50+75,netSize+i+1,input[i]*50+75);

    ofSetColor(255,0,0);
    for(int i = 1; i < netSize; i++)
        ofLine(netSize+i,target[i-1]*50+75,netSize+i+1,target[i]*50+75);

    ofSetColor(255,255,0);
    for(int i = 1; i < periodicalError.size(); i++)
        ofLine(netSize*2+(i%netSize),periodicalError[i-1],
               netSize*2+(i%netSize+1),periodicalError[i]);

    if(frameCounter % netSize == 0){
            frameCounter = 0;
            float SumPerError = 0.0;
            for(int i = 1; i < globError.size(); i++)
                SumPerError += globError[i];

            cout << SumPerError << "\n";
            globError.clear();
            periodicalError.push_back(SumPerError);
    }

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
