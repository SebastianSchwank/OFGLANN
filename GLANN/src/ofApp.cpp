#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofDisableArbTex();
    ofEnableAlphaBlending();
    ofSetFrameRate(0);
    ofSetVerticalSync(false);
    glEnable(GL_DEPTH_TEST);

    //Input = Values 2x128 + 1 bias
     InputSize = 257;
    //Output == Values + Internal Calculations
    OutputSize = 128;
    mWorker = new GLANN();
    mWorker->initGLANN();

    mLayer = new ANNData( InputSize, OutputSize, 0.1, 0.1, 0.01);
    mGateLayer = new ANNData( OutputSize, OutputSize, 0.1, 0.1, 0.01);
    //mLayer2 = new ANNData( netSize, netSize, 0.5, 5.0, 0.01);
    //mLayer3 = new ANNData( netSize, netSize, 0.5, 5.0, 0.01);

    //mLayer0->mWeights.loadImage("L0FrequFSythesisFeedbackAdded.png");
    //mLayer1->mWeights.loadImage("L1Phase&FrequFSythesis.png");

    for(int i = 0; i < InputSize; i++)
        feedback.push_back(0.5);

    frameCounter = 0;
    train = true;

    periodicalError.push_back(999999.9);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    frameCounter++;
    int Frequency = frameCounter % OutputSize;

    vector<float> input;

    for(int i = 0; i < OutputSize+1; i++)
        input.push_back((1.0+sin(0.2*Frequency*2.0*PI*(1.0*i/OutputSize)))/2.1);

    //Don't forget the bias !
    input[0] = 0.9999;

    //input[Frequency] = 0.9999;
    //input[Amp*125+126] = 0.9999;

            //ITERATION == 2 !!!

            for(int i = 0; i < OutputSize; i++)
                input.push_back(feedback[i]);

            vector<float> outputL0;
            outputL0 = mWorker->propergateFW(input,mLayer);

            feedback.clear();
            vector<float> outputL1;
            outputL1 = mWorker->propergateFW(outputL0,mGateLayer);
            feedback = outputL1;

        vector<float> target;
        for(int i = 0; i < OutputSize; i++)
            target.push_back(0.5);

        target[Frequency] = 0.999;

        //target[255+Phase*256] = 1.0;

        float sumQuadError = 0.0;
        vector<float> error;
        for(int i = 0; i < target.size(); i++){
            error.push_back(4.0*(target[i]-outputL1[i])*outputL1[i]*(1.0-outputL1[i]));
            sumQuadError += (target[i]-outputL1[i])*(target[i]-outputL1[i]);
        }
        globError.push_back(sumQuadError);

        for(int i = 0; i < OutputSize; i++)
            error.push_back(0.0);

        if(train){
            vector<float> bpErr = mWorker->propergateBW(outputL0,error,mGateLayer);
            mWorker->propergateBW(input,bpErr,mLayer);
        }

    mWorker->draw(mLayer,0,0);
    mWorker->draw(mGateLayer,0,OutputSize);

   ofSetColor(255,255,0);
    for(int i = 1; i < outputL1.size(); i++)
        ofLine(InputSize+i,outputL1[i-1]*50+25,InputSize+i+1,outputL1[i]*50+25);

    ofSetColor(255,0,0);
    for(int i = 1; i < error.size(); i++)
        ofLine(InputSize+i,error[i-1]*50+25,InputSize+i+1,error[i]*50+25);

    ofSetColor(0,255,255);
    for(int i = 1; i < input.size(); i++)
        ofLine(InputSize+i,input[i-1]*50+75,InputSize+i+1,input[i]*50+75);

    ofSetColor(255,0,255);
    for(int i = 1; i < target.size(); i++)
        ofLine(InputSize+i,target[i-1]*50+75,InputSize+i+1,target[i]*50+75);

    ofSetColor(255,255,0);
    for(int i = 1; i < periodicalError.size(); i++)
        ofLine(InputSize*2+(i%ofGetWidth()),periodicalError[i-1]*100.0,
               InputSize*2+(i%ofGetWidth()+1),periodicalError[i]*100.0);

    if(frameCounter % OutputSize == 0){
            float SumPerError = 0.0;
            for(int i = 1; i < globError.size(); i++)
                SumPerError += globError[i];

            cout << SumPerError/globError.size() << "\n";
            periodicalError.push_back(SumPerError/globError.size());
            globError.clear();

            if(train & (periodicalError[periodicalError.size()-1] <  periodicalError[periodicalError.size()-2]) ){
                mLayer->mWeights.saveImage("LFrequFSythesisFeedbackAdded.png");
                mGateLayer->mWeights.saveImage("GFrequFSythesisFeedbackAdded.png");
                cout << "Saved";
            }else{
                //mLayer0->mWeights.loadImage("L0FrequFSythesisFeedbackAdded.png");
                //mLayer0->randMomentum(1.0);
                //periodicalError.pop_back();
                //cout << "Loaded";
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
