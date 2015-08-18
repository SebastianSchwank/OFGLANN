#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofDisableArbTex();
    ofEnableAlphaBlending();
    ofSetFrameRate(0);
    ofSetVerticalSync(false);
    glEnable(GL_DEPTH_TEST);

    //Input = Values Bias,X,Y
    InputSize  = sizeV;
    //Output == Values + Internal Calculations
    OutputSize = sizeV;
    mWorker = new GLANN();
    mWorker->initGLANN();

    mInputL = new ANNData( InputSize+1, OutputSize, 0.1, 1.0, 0.01);
    mL0 = new ANNData(     OutputSize+1, OutputSize, 0.1, 1.0, 0.01);
    //for(int i = 0; i < numHLayers)
    //    mLayers.push_back(new ANNData( OutputSize+1, OutputSize, 0.1, 5.0, 0.01));
    //mLayer0 = new ANNData( InputSize+1, OutputSize, 0.1, 5.0, 0.01);
    //mHLayer = new ANNData( OutputSize+1, OutputSize, 0.1, 0.1, 0.01);
    //mLayer2 = new ANNData( netSize, netSize, 0.5, 5.0, 0.01);
    //mLayer3 = new ANNData( netSize, netSize, 0.5, 5.0, 0.01);

    //mLayer0->mWeights.loadImage("L0FrequFSythesisFeedbackAdded.png");
    //mLayer1->mWeights.loadImage("L1Phase&FrequFSythesis.png");

    frameCounter = 0;
    train = true;

    periodicalError.push_back(999999.9);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    float freq = 1.0f * (frameCounter % sizeV) / sizeV;

    vector<float> input;

    //Bias
    input.push_back(0.9999);

    for(int i = 0; i < sizeV; i++){
        input.push_back(freq*1.0f*i/sizeV);
    }

    //input[Frequency] = 0.9999;
    //input[Amp*125+126] = 0.9999;

            //Layers = 2

            //vector<float> outputL1;
            //outputL1 = mWorker->propergateFW(input,mLayer1);

            //outputL1.push_back(0.9999);

        vector<float> outputLI;
        outputLI = mWorker->propergateFW(input,mInputL);

        vector<float> outputL0;
        outputL0 = mWorker->propergateFW(outputLI,mL0);


        vector<float> target;

        for(int i = 0; i < sizeV; i++){
            target.push_back( ( 1.0f+sin( freqMax * 2*PI * freq * 1.0f * i / sizeV) ) /2.1f );
        }

        //target[255+Phase*256] = 1.0;

        float sumQuadError = 0.0;
        vector<float> error;
        for(int i = 0; i < target.size(); i++){
            error.push_back(4.0*(target[i]-outputL0[i])*outputL0[i]*(1.0-outputL0[i]));
            sumQuadError += (target[i]-outputL0[i])*(target[i]-outputL0[i]);
        }
        globError.push_back(sumQuadError);

        if(train){
            vector<float> bpErr = mWorker->propergateBW(outputLI,error,mL0);
            mWorker->propergateBW(input,bpErr,mInputL);
        }

    mWorker->draw(mInputL,0,0);
    mWorker->draw(mL0,0,sizeV+25);

   ofSetColor(255,255,0);
    for(int i = 1; i < outputL0.size(); i++)
        ofLine(InputSize+i,outputL0[i-1]*50+25,InputSize+i+1,outputL0[i]*50+25);

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
        ofLine((InputSize*2+i)%ofGetWidth(),periodicalError[i-1]*10.0,
               (InputSize*2+i)%ofGetWidth()+1,periodicalError[i]*10.0);

    if(frameCounter % sizeV == 0){
            float SumPerError = 0.0;
            for(int i = 1; i < globError.size(); i++)
                SumPerError += globError[i];

            cout << SumPerError/globError.size() << "\n";
            periodicalError.push_back(SumPerError/globError.size());
            globError.clear();

            if(train & (periodicalError[periodicalError.size()-1] <  periodicalError[periodicalError.size()-2]) ){
                mInputL->mWeights.saveImage("L0FrequFSythesisFeedbackAdded.png");
                //mLayer1->mWeights.saveImage("L1FrequFSythesisFeedbackAdded.png");
                cout << "Saved";
            }else{
                //mLayer0->mWeights.loadImage("L0FrequFSythesisFeedbackAdded.png");
                //mLayer0->randMomentum(1.0);
                //periodicalError.pop_back();
                //cout << "Loaded";
            }
    }

    frameCounter++;
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
