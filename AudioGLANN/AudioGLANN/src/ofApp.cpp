#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofDisableArbTex();
    ofEnableAlphaBlending();
    ofSetFrameRate(60);
    glEnable(GL_DEPTH_TEST);

    netSize = 512;
    mWorker = new GLANN();
    mWorker->initGLANN(netSize);

    mNetwork = new ANNData( netSize, 0.1, 0.2, 0.01);
    mNetwork->mWeights.loadImage("currentProjectsOutfile.png");

    frameCounter = 0;

    signalInput.load("Input_BecauseIBelive.wav");
    //signalInput.load("AllesGute.wav");
    signalTarget.load("Target_BecauseIBelive.wav");

    signalInput.normalise();
    signalTarget.normalise();

    for(int i = 0; i < signalInput.length; i++){
        audioInputF.push_back((signalInput.play()+1.0)/2.0);
        audioTargetF.push_back((signalTarget.play()+1.0)/2.0);
    }

    training = true;
}

bool ofApp::train(int stepsize){

        input.clear();
        for(int i = 0; i < netSize; i++)
            input.push_back(audioInputF[i+frameCounter]);

        //Don't forget the bias !
        input[0] = 0.9999;
        //input[pos-1 % netSize] = 0.999;

        output.clear();
        output = mWorker->propergateFW(input,mNetwork);

        target.clear();
        for(int i = 0; i < netSize; i++)
            target.push_back(audioTargetF[i+frameCounter]);

        float sumQuadError = 0.0;
        error.clear();
        for(int i = 0; i < netSize; i++){
            error.push_back(4.0*(target[i]-output[i])*output[i]*(1.0-output[i]));
            sumQuadError += (target[i]-output[i])*(target[i]-output[i]);
            outputF.push_back((output[i]-1.0)*2.0);
        }
        globError.push_back(sumQuadError);

        mWorker->propergateBW(input,error,mNetwork);

        if(frameCounter >= signalInput.length - 2 * netSize){
                frameCounter = 0;
                float SumPerError = 0.0;
                for(int i = 1; i < globError.size(); i++)
                    SumPerError += globError[i];

                cout << "\n" << SumPerError << "\n";
                globError.clear();
                periodicalError.push_back(SumPerError);
                frameCounter = 0;

                return false;
        }

        frameCounter += stepsize;
        return true;
}

bool ofApp::morph(){

        input.clear();
        for(int i = 0; i < netSize; i++)
            input.push_back(audioInputF[i+frameCounter]);

        //Don't forget the bias !
        input[0] = 0.9999;
        //input[pos-1 % netSize] = 0.999;

        output.clear();
        output = mWorker->propergateFW(input,mNetwork);

        for(int i = 0; i < netSize; i++) outputF.push_back(output[i]);

        frameCounter += netSize;

        if(frameCounter >= signalInput.length - 2 * netSize){
            std::ofstream ofile("signalOut.bin", std::ios::binary);
            ofile.write((char*) &outputF[0], sizeof(float)*audioInputF.size());
            return false;
        }
}

//--------------------------------------------------------------
void ofApp::update(){
    if(training){
        if(train(netSize)){
            cout << "\r" << 100.0 * (float)frameCounter/signalInput.length;
        }else{
            cout << "\n Training cycle passed \n";
            mNetwork->mWeights.saveImage("currentProjectsOutfile.png");
            cout << "Output Weights-Image written." ;
            signalTarget.setPosition(0.0);
            signalInput.setPosition(0.0);
            frameCounter = 0;
            cout << "Training sucessfull ! \n";
            training = false;
        }

    }else{

        while(morph());

        cout << "Morph sucessfull ! \n";
        training = true;

        signalTarget.setPosition(0.0);
        signalInput.setPosition(0.0);
        frameCounter = 0;
    }

}

//--------------------------------------------------------------
void ofApp::draw(){

        mWorker->draw(mNetwork);

        ofSetColor(255);
        ofDrawBitmapString("Input",netSize,50);
        for(int i = 1; i < input.size(); i++)
            ofLine(netSize+i,input[i-1]*50+25,netSize+i+1,input[i]*50+25);

       ofSetColor(255,0,0);
       ofDrawBitmapString("Output",netSize,75);
        for(int i = 1; i < output.size(); i++)
            ofLine(netSize+i,output[i-1]*50+50,netSize+i+1,output[i]*50+50);

        ofSetColor(0,255,0);
        ofDrawBitmapString("Target",netSize,100);
        for(int i = 1; i < target.size(); i++)
            ofLine(netSize+i,target[i-1]*50+75,netSize+i+1,target[i]*50+75);

        ofSetColor(255,0,255);
        ofDrawBitmapString("Error",netSize,150);
        for(int i = 1; i < error.size(); i++)
            ofLine(netSize+i,error[i-1]*50+125,netSize+i+1,error[i]*50+125);

        ofSetColor(255,255,0);
        for(int i = 1; i < periodicalError.size(); i++)
            ofLine(netSize*2+(i%netSize),periodicalError[i-1]/2,
                   netSize*2+(i%netSize+1),periodicalError[i]/2);

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
