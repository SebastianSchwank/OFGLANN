#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofDisableArbTex();
    ofEnableAlphaBlending();
    ofSetFrameRate(0);
    ofSetVerticalSync(false);
    glEnable(GL_DEPTH_TEST);

    netSize = 256;
    mWorker = new GLANN();
    mWorker->initGLANN(netSize);

    mLayer0 = new ANNData( netSize, 0.1, 1.0, 0.01);
    mLayer1 = new ANNData( netSize, 0.1, 1.0, 0.01);

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
    for(int i = 0; i < netSize; i++)
        //input.push_back((1.0+sin(((pos % netSize)/5.0)*i/netSize))/2.1);
        input.push_back(0.0);

    //Don't forget the bias !
    input[0] = 0.9999;

    input[pos % netSize] = 0.999;
    //input[pos-1 % netSize] = 0.999;


    vector<float> outputL0;
    outputL0 = mWorker->propergateFW(input,mLayer0);

    //Don't forget the bias !
    outputL0[0] = 0.9999;
    vector<float> outputL1;
    outputL1 = mWorker->propergateFW(outputL0,mLayer1);

    vector<float> target;
    for(int i = 0; i < netSize; i++)
        target.push_back((1.0+sin(((pos % netSize)/5.0)*i/netSize))/2.1);

    float sumQuadError = 0.0;
    vector<float> error;
    for(int i = 0; i < netSize; i++){
        error.push_back(4.0*(target[i]-outputL1[i])*outputL1[i]*(1.0-outputL1[i]));
        sumQuadError += (target[i]-outputL1[i])*(target[i]-outputL1[i]);
    }
    globError.push_back(sumQuadError);

    if(train){
        vector<float> backpropError = mWorker->propergateBW(outputL0,error,mLayer1);
        //for(int i = 0; i < netSize; i++) cout << backpropError[i];
        mWorker->propergateBW(input,backpropError,mLayer0);
    }



    mWorker->draw(mLayer0,0,0);
    mWorker->draw(mLayer1,0,netSize+5);

   ofSetColor(255);
    for(int i = 1; i < netSize; i++)
        ofLine(netSize+i,outputL1[i-1]*50+25,netSize+i+1,outputL1[i]*50+25);

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
        ofLine(netSize*2+(i%ofGetWidth()),periodicalError[i-1],
               netSize*2+(i%ofGetWidth()+1),periodicalError[i]);

    if(frameCounter % netSize == 0){
            frameCounter = 0;
            float SumPerError = 0.0;
            for(int i = 1; i < globError.size(); i++)
                SumPerError += globError[i];

            cout << SumPerError << "\n";
            globError.clear();
            periodicalError.push_back(SumPerError);

            mLayer0->mWeights.saveImage("L0simpleDFT.png");
            mLayer1->mWeights.saveImage("L1simpleDFT.png");
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
