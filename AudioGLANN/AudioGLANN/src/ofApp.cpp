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

    mLayer1 = new ANNData( netSize, 0.1, 0.2, 0.03);
    //mLayer2 = new ANNData( netSize, 0.1, 0.1, 0.00);

    //mLayer1->mWeights.loadImage("currentProjectsOutfile1.png");
    //mLayer2->mWeights.loadImage("currentProjectsOutfile2.png");
/*
    signalInput.load("Input_Sonnentanz.wav");
    //signalInput.load("AllesGute.wav");
    signalTarget.load("Target_Sonnentanz.wav");

    signalInput.normalise(1.0);
    signalTarget.normalise(1.0);

    for(int i = 0; i < signalInput.length; i++){
        audioInputF.push_back((signalInput.play()+1.0)/2.0);
        audioTargetF.push_back((signalTarget.play()+1.0)/2.0);
    }
*/
    for(int j = 0; j < netSize; j++){
        for(int i = 0; i < netSize; i++){
            audioInputF.push_back(1.0+sin( ((1.0*i)/netSize) * 2 * 3.14159265359 * (0.05*j+1.0))/2.0);
            audioTargetF.push_back(0.5);
        }
        audioTargetF[j+j*netSize] = 0.99;
    }

    frameCounter = 0;
    training = true;
}

bool ofApp::train(){

        input.clear();
        for(int i = 0; i < netSize; i++)
            input.push_back(audioInputF[i+frameCounter]);

        //Don't forget the bias !
        input[0] = 0.9999;

        target.clear();
        for(int i = 0; i < netSize; i++)
            target.push_back(audioTargetF[i+frameCounter]);
/*
        inputHiddenLayer.clear();
        for(int i = 0; i < netSize; i++)
            inputHiddenLayer.push_back(outputL1F[i+frameCounter]);
*/
        float sumQuadError = 0.0;
        error.clear();
        for(int i = 0; i < netSize; i++){
            error.push_back(4.0*(target[i]-output[i])*output[i]*(1.0-output[i]));
            sumQuadError += (target[i]-output[i])*(target[i]-output[i]);
            //outputF.push_back((output[i]-1.0)*2.0);
        }
        globError.push_back(sumQuadError/netSize);

        mWorker->propergateBW(input,error,mLayer1);

        return true;
}

bool ofApp::morph(){

        input.clear();
        for(int i = 0; i < netSize; i++)
            input.push_back(audioInputF[i+frameCounter]);

        //Don't forget the bias !
        input[0] = 0.9999;

        output.clear();
        output = mWorker->propergateFW(input,mLayer1);

/*
        if(frameCounter >= audioInputF.size() - 2 * netSize){
            //Have to add supersampling here
            std::ofstream ofile("signalOut.bin", std::ios::binary);
            ofile.write((char*) &outputF[0], sizeof(float)*outputF.size());
            ofile.close();
            cout << "signalOut.bin written \n";
            return false;
        }
*/
        return true;
}

//--------------------------------------------------------------
void ofApp::update(){

    if(training){
        morph();
        train();
        cout << "\r Training progress: "
        << 100.0 * (float)frameCounter/audioInputF.size()
        << "| Current mean-error per sample : " << globError[globError.size()-1];

        frameCounter += netSize;

        if(frameCounter >= audioInputF.size()){
            cout << "Training Lesson complete -> redo now !\n";
            mLayer1->mWeights.saveImage("currentProjectsOutfile1.png");
            cout << "Output Weights-Image written.\n" ;
            mWorker->draw(mLayer1)->saveImage("currentProjectPreview1.png");
            cout << "Preview Weights written.\n";
            signalTarget.setPosition(0.0);
            signalInput.setPosition(0.0);
            frameCounter = 0;

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
        frameCounter += netSize;

        if(frameCounter >= audioInputF.size()){
            frameCounter = 0;
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){

        //mWorker->draw(mNetwork);
        mWorker->draw(mLayer1)->draw(0,256);

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
            ofLine(netSize*2+(i%netSize),periodicalError[i-1]*256,
                   netSize*2+(i%netSize+1),periodicalError[i]*256);

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
