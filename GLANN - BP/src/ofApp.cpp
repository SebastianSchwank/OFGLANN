#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofDisableArbTex();
    ofEnableAlphaBlending();
    ofSetFrameRate(60);
    glEnable(GL_DEPTH_TEST);
    ofSetFrameRate(0);
    ofSetVerticalSync (false);

    netSize = 32;
    mWorker = new GLANN();
    mWorker->initGLANN(netSize);

    mNetwork = new ANNData( netSize, 0.05,0.2, 0.0);
    //mNetwork->mWeights.loadImage("LayersOutput.png");

    frameCounter = 0;
    numIterations = 1;
    train = true;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    frameCounter++;

    int pos = frameCounter % netSize;

    vector<float> input;
    input.clear();
    for(int i = 0; i < netSize; i++)
        //input.push_back((1.0+sin(((pos % netSize)/5.0)*i/netSize))/2.1);
        input.push_back(0.5);

    //Don't forget the bias !
    input[0] = 0.9999;

    input[pos/2] = 0.9999;
    //input[pos/2+netSize/2] = 0.999;
    //input[pos-1 % netSize] = 0.999;


    vector<vector<float>> outputLX;
    outputLX.clear();

    outputLX.push_back(mWorker->propergateFW(input,mNetwork));
    //Don't forget the bias !
    if(numIterations != 0)
        outputLX[0][0] = 0.999;

    for(int i = 0; i < numIterations; i++){
        outputLX.push_back(mWorker->propergateFW(outputLX[i],mNetwork));
        if(i != numIterations-1)
            //Don't forget the bias !
            outputLX[i+1][0] = 0.999;
    }

    vector<float> target;
    target.clear();
    for(int i = 0; i < netSize; i++)
        target.push_back(0.5);

    for(int i = netSize/2; i < netSize; i++)
        target[i] = ((1.0+sin(0.2*2.0*PI*(pos+1.0)*(i-netSize/2)/netSize))/2.1);

    float sumQuadError = 0.0;
    vector<float> error;
    error.clear();
    for(int i = 0; i < netSize; i++){
        error.push_back(4.0*(target[i]-outputLX[numIterations][i])
                        *outputLX[numIterations][i]*(1.0-outputLX[numIterations][i]));
        sumQuadError += (target[i]-outputLX[numIterations][i])
                        *(target[i]-outputLX[numIterations][i]);
    }
    globError.push_back(sumQuadError);


    if(train){
        if(numIterations > 0){
            vector<float> backpropErr = error;
            for(int i = numIterations-1; i >= 0 ; i--)
                backpropErr = mWorker->propergateBW(outputLX[i],backpropErr,mNetwork);
            mWorker->propergateBW(input,backpropErr,mNetwork);
        }

            mWorker->propergateBW(input,error,mNetwork);
    }


    mWorker->draw(mNetwork);

   ofSetColor(255);
    for(int i = 1; i < netSize; i++)
        ofLine(netSize+(i*1),outputLX[numIterations][i-1]*50+25,netSize+(i*1)+1
               ,outputLX[numIterations][i]*50+25);

    ofSetColor(255,0,0);
    for(int i = 1; i < netSize; i++)
        ofLine(netSize+(i),error[i-1]*50+25,netSize+(i)+1,error[i]*50+25);

    ofSetColor(255,255,0);
    for(int i = 1; i < netSize; i++)
        ofLine(netSize+(i*1),input[i-1]*50+75,netSize+(i*1)+1,input[i]*50+75);

    ofSetColor(255,0,0);
    for(int i = 1; i < netSize; i++)
        ofLine(netSize+(i*1),target[i-1]*50+125,netSize+(i*1)+1,target[i]*50+125);

    ofSetColor(255,255,0);
    for(int i = 1; i < periodicalError.size(); i++)
        ofLine(netSize+(i),periodicalError[i-1]*1.0,
               netSize+(i+1),periodicalError[i]*1.0);

    if(frameCounter % netSize == 0){
            frameCounter = 0;
            float SumPerError = 0.0;
            for(int i = 1; i < globError.size(); i++)
                SumPerError += globError[i];

            cout << SumPerError << "\n";
            globError.clear();
            periodicalError.push_back(SumPerError);

            mNetwork->mWeights.saveImage("LayersOutput.png");
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    train = !train;
    if(!train) ofSetFrameRate(17);
    else ofSetFrameRate(0);
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
