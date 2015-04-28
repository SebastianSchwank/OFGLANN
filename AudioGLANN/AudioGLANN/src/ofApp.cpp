#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(0);
    //glEnable(GL_DEPTH_TEST);

    netSize = 512;
    mWorker = new GLANN();
    mWorker->initGLANN(netSize);

    mLayer1 = new ANNData( netSize, 0.2, 0.2, 0.03);
    //mLayer2 = new ANNData( netSize, 0.1, 1.0, 0.03);
    //mLayer3 = new ANNData( netSize, 0.1, 0.8, 0.03);

    mLayer1->mWeights.loadImage("ProjectsOutfileL1.png");
    //mLayer2->mWeights.loadImage("ProjectsOutfileL2.png");

    signalInput.load("Impulses.wav");
    //signalInput.load("AllesGute.wav");
    signalT1.load("ImpulseResponses.wav");
    //signalT2.load("Frequenzweiche(Mid).wav");
    //signalT3.load("Frequenzweiche(Low).wav");

    //signalInput.normalise(1.0);
    //signalT1.normalise(1.0);
    //signalT2.normalise(1.0);
    //signalT3.normalise(1.0);

    for(int i = 0; i < signalInput.length; i++)
        audioInputF.push_back((signalInput.play()+1.0)/2.0);

    for(int i = 0; i < signalT1.length; i++)
        audioTargetF.push_back((signalT1.play()+1.0)/2.0);

    for(int j = 0; j < netSize/2; j++)
        audioInputTimeParam.push_back(0.5);
    audioInputTimeParam[audioInputTimeParam.size()-1] = 1.0;


    frameCounter = 0;
    training = true;
}

bool ofApp::train(){

        target.clear();
        for(int i = 0; i < netSize/2; i++)
            target.push_back(audioTargetF[i]);

        for(int i = 0; i < netSize/2; i++)
            target.push_back(audioInputTimeParam[i]);

        float sumQuadError = 0.0;
        error.clear();
        for(int i = 0; i < netSize; i++){
            error.push_back(4.0*(target[i]-output[i])*output[i]*(1.0-output[i]));
            sumQuadError += (target[i]-output[i])*(target[i]-output[i]);
            //outputF.push_back((output[i]-1.0)*2.0);
        }
        globError.push_back(sumQuadError/netSize);

        //vector<float> hiddenError = mWorker->propergateBW(inputHiddenLayer1,error,mLayer2);
        //hiddenError = mWorker->propergateBW(inputHiddenLayer1,hiddenError,mLayer2);
        mWorker->propergateBW(input,error,mLayer1);
        //mWorker->propergateBW(input,hidenError,mLayer1);

        return true;
}

bool ofApp::morph(){

        input.clear();
        for(int i = 0; i < netSize/2; i++)
            input.push_back(audioInputF[i]);

        for(int i = 0; i < netSize/2; i++)
            input.push_back(audioInputTimeParam[i]);

        //Don't forget the bias !
        input[0] = 0.999;

        //inputHiddenLayer.clear();
        //inputHiddenLayer = mWorker->propergateFW(input,mLayer1);

        //Don't forget the bias !
        //inputHiddenLayer[0] = 0.999;

        //inputHiddenLayer1.clear();
        //inputHiddenLayer1 = mWorker->propergateFW(input,mLayer1);

        //Bias.
        //inputHiddenLayer1[0] = 0.999;

        output.clear();
        output = mWorker->propergateFW(input,mLayer1);
/*
        //Bias.
        inputHiddenLayer2[0] = 0.999;

        output.clear();
        output = mWorker->propergateFW(inputHiddenLayer2,mLayer3);
*/
        return true;
}

//--------------------------------------------------------------
void ofApp::update(){

    if(training){

        frameCounter += 1;

        morph();
        train();
        //Wrap the vectors around
        rotate(audioInputF.begin(),audioInputF.begin()+7,audioInputF.end());
        rotate(audioTargetF.begin(),audioTargetF.begin()+7,audioTargetF.end());
        if((frameCounter % (netSize/2)) == 0){
            rotate(audioInputTimeParam.begin(),audioInputTimeParam.begin()+1,audioInputTimeParam.end());
            cout << "TimeParam++ \n";
        }

        cout << "\r Training progress: "
        << frameCounter << "->" << audioInputF.size()/7
        << "| Current mean-error per sample : " << globError[globError.size()-1];

        if(frameCounter >= audioInputF.size()){
            cout << "Training Lesson complete -> redo now !\n";
            mLayer1->mWeights.saveImage("ProjectsOutfileL1.png");
            //mLayer2->mWeights.saveImage("ProjectsOutfileL2.png");
            cout << "Output Weights-Image written.\n" ;
            mWorker->getPrev(mLayer1).saveImage("ProjectPreviewL1.png");
            //mWorker->getPrev(mLayer2).saveImage("ProjectPreviewL2.png");
            cout << "Preview Weights written.\n";
            frameCounter = 0;

            audioInputTimeParam.clear();
            for(int j = 0; j < netSize/2; j++)
                audioInputTimeParam.push_back(0.0);
            audioInputTimeParam[0] = 1.0;

            float GlobErrorPerTraining = 0.0;
            for(int i = 0; i < globError.size(); i++){
                GlobErrorPerTraining += globError[i];
            }
            GlobErrorPerTraining = GlobErrorPerTraining / globError.size();
            periodicalError.push_back(GlobErrorPerTraining);
            cout << "This training lesson caused an Error of : " << GlobErrorPerTraining << "\n";
            globError.clear();
        }
    }else{
        morph();
        //Wrap the vectors around
        rotate(audioInputF.begin(),audioInputF.begin()+1,audioInputF.end());
        rotate(audioTargetF.begin(),audioTargetF.begin()+1,audioTargetF.end());

        frameCounter += 1;

        if(frameCounter >= audioInputF.size()){
            frameCounter = 0;
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){

        ofClear(126);
        //mWorker->draw(mLayer1,netSize);
        //mWorker->draw(mLayer2,netSize*2);
        //mLayer1->mMomentum.draw(0,256);

        ofSetColor(255);
        ofDrawBitmapString("Input",0,50);
        for(int i = 1; i < input.size(); i++)
            ofLine(i,input[i-1]*50+25,i+1,input[i]*50+25);

       ofSetColor(255,0,0);
       ofDrawBitmapString("Output",0,75);
        for(int i = 1; i < output.size(); i++)
            ofLine(i,output[i-1]*50+75,i+1,output[i]*50+75);

        ofSetColor(0,255,0);
        ofDrawBitmapString("Target",0,100);
        for(int i = 1; i < target.size(); i++)
            ofLine(i,target[i-1]*50+125,i+1,target[i]*50+125);

        ofSetColor(255,0,255);
        ofDrawBitmapString("Error",0,150);
        for(int i = 1; i < error.size(); i++)
            ofLine(i,error[i-1]*50+175,i+1,error[i]*50+175);

        ofSetColor(255,255,0);
        for(int i = 1; i < periodicalError.size(); i++)
            ofLine(netSize*2+(i),periodicalError[i-1]*8192,
                   netSize*2+(i+1),periodicalError[i]*8192);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    training = !training;
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
