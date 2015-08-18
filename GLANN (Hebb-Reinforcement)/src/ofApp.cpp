#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofDisableArbTex();
    ofEnableAlphaBlending();
    ofSetFrameRate(0);
    ofSetVerticalSync(false);
    glEnable(GL_DEPTH_TEST);

    //Input = Values Bias,X,Y
    InputSize = 16*16;
    //Output == Values + Internal Calculations
    OutputSize = 16*16;
    mWorker = new GLANN();
    mWorker->initGLANN();

    mLayer = new ANNData( InputSize+1, OutputSize, 0.1, 0.1, 0.01);
    mHLayer = new ANNData( OutputSize+1, OutputSize, 0.1, 0.1, 0.01);
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

    frameCounter++;

    vector<float> input;
    //Bias
    input.push_back(0.9999);

    for(int i = 0; i < InputSize; i++){
        input.push_back(0.0);
    }

    float xF = (frameCounter % 16)/16.0;
    float yF = (int(frameCounter/16.0f) % 16)/16.0;
    input[int(xF*16)+int(yF*16)*16] = 0.9999;

    //Don't forget the bias !
    input[0] = 0.9999;

    //input[Frequency] = 0.9999;
    //input[Amp*125+126] = 0.9999;

            //Layers = 2

            vector<float> outputL0;
            outputL0 = mWorker->propergateFW(input,mLayer);

            //Bias
            outputL0.push_back(0.9999);

            vector<float> outputL1;
            outputL1 = mWorker->propergateFW(outputL0,mHLayer);


        ofBackground(0);
        ofSetColor(255);
        ofLine(0,0,xF*16,yF*16);
        ofImage picture;
        picture.grabScreen(0,0,16,16);

        vector<float> target;
        for(int x = 0; x < 16; x++){
            for(int y = 0; y < 16; y++){
                target.push_back(picture.getColor(x,y).getBrightness()/256.0);
            }
        }

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
            vector<float> bpErr = mWorker->propergateBW(outputL0,error,mHLayer);
            mWorker->propergateBW(input,bpErr,mLayer);
        }

    mWorker->draw(mLayer,0,0);
    mWorker->draw(mHLayer,0,OutputSize);

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
                mHLayer->mWeights.saveImage("GFrequFSythesisFeedbackAdded.png");
                cout << "Saved";
            }else{
                //mLayer0->mWeights.loadImage("L0FrequFSythesisFeedbackAdded.png");
                //mLayer0->randMomentum(1.0);
                //periodicalError.pop_back();
                //cout << "Loaded";
            }
    }

    for(int x = 0; x < 16; x++){
        for(int y = 0; y < 16; y++){
            float bright = picture.getColor(x,y).getBrightness();
            ofSetColor((int)bright);
            ofEllipse(256+x*8,y*8,8,8);
            bright = outputL1[x+y*16];
            ofSetColor(bright*255.0);
            ofEllipse(256+16*8+x*8,y*8,8,8);
            bright = input[x+y*16];
            ofSetColor(bright*255.0);
            ofEllipse(256+2*16*8+x*8,y*8,8,8);
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
