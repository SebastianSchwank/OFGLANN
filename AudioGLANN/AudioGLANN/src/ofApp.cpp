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

    mLayer1 = new ANNData( netSize, 0.3, 0.04, 0.01);
    mLayer2 = new ANNData( netSize, 0.3, 0.04, 0.01);

    signalInput.load("Input_Sonnentanz.wav");
    //signalInput.load("AllesGute.wav");
    signalTarget.load("Target_Sonnentanz.wav");

    signalInput.normalise(1.0);
    signalTarget.normalise(1.0);

    for(int i = 0; i < signalInput.length; i++){
        audioInputF.push_back((signalInput.play()+1.0)/2.0);
        audioTargetF.push_back((signalTarget.play()+1.0)/2.0);
    }

    frameCounter = 0;
    training = true;
}

bool ofApp::train(int stepsize){

        input.clear();
        for(int i = 0; i < netSize; i++)
            input.push_back(audioInputF[i+frameCounter]);

        //Don't forget the bias !
        input[0] = 0.9999;
        //input[pos-1 % netSize] = 0.999;

        inputHiddenLayer.clear();
        inputHiddenLayer = mWorker->propergateFW(input,mLayer1);

        output.clear();
        output = mWorker->propergateFW(inputHiddenLayer,mLayer2);

        target.clear();
        for(int i = 0; i < netSize; i++)
            target.push_back(audioTargetF[i+frameCounter]);

        float sumQuadError = 0.0;
        error.clear();
        for(int i = 0; i < netSize; i++){
            error.push_back(4.0*(target[i]-output[i])*output[i]*(1.0-output[i]));
            sumQuadError += (target[i]-output[i])*(target[i]-output[i]);
            //outputF.push_back((output[i]-1.0)*2.0);
        }
        globError.push_back(sumQuadError);

        vector<float> hiddenError = mWorker->propergateBW(inputHiddenLayer,error,mLayer2);
        mWorker->propergateBW(input,hiddenError,mLayer1);

        if(frameCounter >= signalInput.length - 2 * netSize){
                frameCounter = 0;
                float SumPerError = 0.0;
                for(int i = 1; i < globError.size(); i++)
                    SumPerError += globError[i];

                cout << "\n Global Error over all samples: " << SumPerError << "\n";
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

        inputHiddenLayer.clear();
        inputHiddenLayer = mWorker->propergateFW(input,mLayer1);

        output.clear();
        output = mWorker->propergateFW(inputHiddenLayer,mLayer2);

        for(int i = 0; i < netSize; i++)
            outputF.push_back((output[i]-1.0)*2.0);

        frameCounter += netSize;
        cout << "\r" << 100.0 * (float)frameCounter/signalInput.length;

        if(frameCounter >= signalInput.length - 2 * netSize){
            //Have to add supersampling here
            std::ofstream ofile("signalOut.bin", std::ios::binary);
            ofile.write((char*) &outputF[0], sizeof(float)*outputF.size());
            ofile.close();
            cout << "signalOut.bin written \n";
            return false;
        }
}

//--------------------------------------------------------------
void ofApp::update(){

    if(training){
        if(train(netSize/3)){
            cout << "\r Training progress: "
            << 100.0 * (float)frameCounter/signalInput.length;
        }else{
            cout << "\n Training cycle passed \n";
            mLayer1->mWeights.saveImage("currentProjectsOutfile1.png");
            mLayer2->mWeights.saveImage("currentProjectsOutfile2.png");
            cout << "Output Weights-Image written.\n" ;
            mWorker->draw(mLayer1)->saveImage("currentProjectPreview1.png");
            mWorker->draw(mLayer2)->saveImage("currentProjectPreview2.png");
            cout << "Preview Weights written.\n";
            signalTarget.setPosition(0.0);
            signalInput.setPosition(0.0);
            frameCounter = 0;
            cout << "Training sucessfull ! \n";
            training = false;
        }

    }else{

        if(morph()){}
        else{

            cout << "Morph sucessfull ! \n";

            signalTarget.setPosition(0.0);
            signalInput.setPosition(0.0);
            frameCounter = 0;
            training = true;
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){

        //mWorker->draw(mNetwork);
        mWorker->draw(mLayer1)->draw(0,256);
        mWorker->draw(mLayer2)->draw(netSize,256);

        ofSetColor(255);
        ofDrawBitmapString("Input",0,50);
        for(int i = 1; i < input.size(); i++)
            ofLine(i,input[i-1]*50+25,i+1,input[i]*50+25);

       ofSetColor(255,0,0);
       ofDrawBitmapString("Output",0,75);
        for(int i = 1; i < output.size(); i++)
            ofLine(i,output[i-1]*50+50,i+1,output[i]*50+50);

        ofSetColor(0,255,0);
        ofDrawBitmapString("Target",0,100);
        for(int i = 1; i < target.size(); i++)
            ofLine(i,target[i-1]*50+75,i+1,target[i]*50+75);

        ofSetColor(255,0,255);
        ofDrawBitmapString("Error",0,150);
        for(int i = 1; i < error.size(); i++)
            ofLine(i,error[i-1]*50+125,i+1,error[i]*50+125);
/*
        ofSetColor(255,255,0);
        for(int i = 1; i < periodicalError.size(); i++)
            ofLine(netSize*2+(i%netSize),periodicalError[i-1]/2,
                   netSize*2+(i%netSize+1),periodicalError[i]/2);
*/
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
