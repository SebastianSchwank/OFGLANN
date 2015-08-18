#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofDisableArbTex();
    ofEnableAlphaBlending();
    ofSetFrameRate(0);
    ofSetVerticalSync(false);
    glEnable(GL_DEPTH_TEST);

    //Input = Values 2 + 1 bias
     InputSize = 3;
    //Output == Values + Internal Calculations
    OutputSize = 2;
    mWorker = new GLANN();
    mWorker->initGLANN();

    mLayer = new ANNData( InputSize, OutputSize, 0.1, 1.0, 0.01);
    mHLayer = new ANNData( InputSize, OutputSize, 0.1, 1.0, 0.01);
    //mLayer2 = new ANNData( netSize, netSize, 0.5, 5.0, 0.01);
    //mLayer3 = new ANNData( netSize, netSize, 0.5, 5.0, 0.01);

    //mLayer0->mWeights.loadImage("L0FrequFSythesisFeedbackAdded.png");
    //mLayer1->mWeights.loadImage("L1Phase&FrequFSythesis.png");

    frameCounter = 0;
    train = true;


//Predefined Data Set
    vector<float> input;
    input.push_back(0.0);
    input.push_back(0.0);
    inputSet.push_back(input);

    vector<float> target;
    target.push_back(0.0);
    target.push_back(0.0);
    targetSet.push_back(target);

    input.clear();
    input.push_back(0.999);
    input.push_back(0.0);
    inputSet.push_back(input);

    target.clear();
    target.push_back(0.999);
    target.push_back(0.0);
    targetSet.push_back(target);

    input.clear();
    input.push_back(0.0);
    input.push_back(0.999);
    inputSet.push_back(input);

    target.clear();
    target.push_back(0.999);
    target.push_back(0.0);
    targetSet.push_back(target);

    input.clear();
    input.push_back(0.999);
    input.push_back(0.999);
    inputSet.push_back(input);

    target.clear();
    target.push_back(0.0);
    target.push_back(0.999);
    targetSet.push_back(target);

    periodicalError.push_back(999999.9);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    int index = frameCounter % inputSet.size();

    vector<float> input = inputSet[index];

    //Don't forget the bias !
    input.push_back(0.9999);

    //input[Frequency] = 0.9999;
    //input[Amp*125+126] = 0.9999;

            //ITERATION == 2 !!!

            vector<float> outputL0;
            outputL0 = mWorker->propergateFW(input,mLayer);

            //Don't forget the bias !
            outputL0.push_back(0.9999);

            vector<float> outputL1;
            outputL1 = mWorker->propergateFW(outputL0,mHLayer);

        vector<float> target = targetSet[index];

        //target[255+Phase*256] = 1.0;

        float sumQuadError = 0.0;
        vector<float> error;
        for(int i = 0; i < target.size(); i++){
            error.push_back(4.0*(target[i]-outputL1[i])*outputL1[i]*(1.0-outputL1[i]));
            sumQuadError += (target[i]-outputL1[i])*(target[i]-outputL1[i]);
        }
        globError.push_back(sumQuadError);

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

    if(frameCounter % 1024 == 0){
            float SumPerError = 0.0;
            for(int i = 1; i < globError.size(); i++)
                SumPerError += globError[i];

            cout << SumPerError/globError.size() << "\n";
            periodicalError.push_back(SumPerError/globError.size());
            globError.clear();

            for(int x = 0; x < 32; x++){
                for(int y = 0; y < 32; y++){
                    float netX,netY;
                    netX = 1.0f*x/32.0f;
                    netY = 1.0f*y/32.0f;

                    input.clear();
                    input.push_back(netX);
                    input.push_back(netY);

                    vector<float> outputL0;
                    outputL0 = mWorker->propergateFW(input,mLayer);

                    //Don't forget the bias !
                    outputL0.push_back(0.9999);

                    vector<float> outputL1;
                    outputL1 = mWorker->propergateFW(outputL0,mHLayer);

                    ofSetColor(outputL1[0]*256.0,0.0,outputL0[1]*256.0);
                    ofCircle(x+200,y,1);

                }

                cout << x << "\n";
            }

            ofImage renderedOutput;
            renderedOutput.grabScreen(200,0,236,36);
            string Label = ofToString(frameCounter);
            Label += ".png";
            renderedOutput.saveImage(Label);

            if(train & (periodicalError[periodicalError.size()-1] <  periodicalError[periodicalError.size()-2]) ){
                mLayer->mWeights.saveImage("L0BinaryInspiredFloatingpointAdding.png");
                mHLayer->mWeights.saveImage("L1BinaryInspiredFloatingpointAdding.png");
            }else{
                //mLayer0->mWeights.loadImage("L0FrequFSythesisFeedbackAdded.png");
                //mLayer0->randMomentum(1.0);
                //periodicalError.pop_back();
                //cout << "Loaded";
            }
            cout << "Saved";
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
