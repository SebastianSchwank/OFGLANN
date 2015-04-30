#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofDisableArbTex();
    ofEnableAlphaBlending();
    ofSetFrameRate(0);
    ofSetVerticalSync(false);
    glEnable(GL_DEPTH_TEST);

    //Input = Values 256 + bias = 256
    netSize = 256;
    mWorker = new GLANN();
    mWorker->initGLANN();

    mLayer0 = new ANNData( netSize, netSize, 0.1, 0.25, 0.01);
    mLayer1 = new ANNData( netSize, netSize, 0.1, 0.25, 0.01);

    //mLayer0->mWeights.loadImage("L0Phase&FrequFSythesis.png");
    //mLayer1->mWeights.loadImage("L1Phase&FrequFSythesis.png");

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

    float Frequency = ofRandom(0.9999);
    float Amp     = ofRandom(0.9999);

    for(int i = 0; i < netSize; i++)
        input.push_back(0.0);

    //Don't forget the bias !
    input[0] = 0.9999;

    input[Frequency*256] = Amp;
    //input[Amp*125+126] = 0.9999;

            vector<float> outputL0;
            outputL0 = mWorker->propergateFW(input,mLayer0);

            //Don't forget the bias !
            outputL0[0] = 0.9999;

            vector<float> outputL1;
            outputL1 = mWorker->propergateFW(outputL0,mLayer1);

            //cout << outputL1.size();

            //ofSetColor(outputL1[2] * 255);
            //ofCircle(x,y,1);

        //Target = Histogramm of noise
        vector<float> target;
        for(int i = 0; i < netSize; i++)
            target.push_back((1.0+Amp*sin(10.0*Frequency*2.0*PI*(1.0*i/netSize)))/2.1);

        //target[255+Phase*256] = 1.0;

        float sumQuadError = 0.0;
        vector<float> error;
        for(int i = 0; i < target.size(); i++){
            error.push_back(4.0*(target[i]-outputL1[i])*outputL1[i]*(1.0-outputL1[i]));
            sumQuadError += (target[i]-outputL1[i])*(target[i]-outputL1[i]);
        }
        globError.push_back(sumQuadError);

        if(train){
            vector<float> backpropError = mWorker->propergateBW(outputL0,error,mLayer1);
            mWorker->propergateBW(input,backpropError,mLayer0);
        }

    mWorker->draw(mLayer0,0,0);
    mWorker->draw(mLayer1,0,netSize+5);

   ofSetColor(255,255,0);
    for(int i = 1; i < outputL1.size(); i++)
        ofLine(netSize+i,outputL1[i-1]*50+25,netSize+i+1,outputL1[i]*50+25);

    ofSetColor(255,0,0);
    for(int i = 1; i < error.size(); i++)
        ofLine(netSize+i,error[i-1]*50+25,netSize+i+1,error[i]*50+25);

    ofSetColor(0,255,255);
    for(int i = 1; i < input.size(); i++)
        ofLine(netSize+i,input[i-1]*50+75,netSize+i+1,input[i]*50+75);

    ofSetColor(255,0,255);
    for(int i = 1; i < target.size(); i++)
        ofLine(netSize+i,target[i-1]*50+75,netSize+i+1,target[i]*50+75);

    ofSetColor(255,255,0);
    for(int i = 1; i < periodicalError.size(); i++)
        ofLine(netSize*2+(i%ofGetWidth()),periodicalError[i-1]*10.0,
               netSize*2+(i%ofGetWidth()+1),periodicalError[i]*10.0);

    if(frameCounter % 128 == 0){
            float SumPerError = 0.0;
            for(int i = 1; i < globError.size(); i++)
                SumPerError += globError[i];

            cout << SumPerError/globError.size() << "\n";
            globError.clear();
            periodicalError.push_back(SumPerError/globError.size());

            if(train){
                mLayer0->mWeights.saveImage("L0Phase&FrequFSythesis.png");
                mLayer1->mWeights.saveImage("L1Phase&FrequFSythesis.png");
            }
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
