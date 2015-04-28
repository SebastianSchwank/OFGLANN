#pragma once

#include <fstream>

#include "ofMain.h"

#include "ofxMaxim.h"

#include "dataModel/anndata.h"
#include "worker/glann.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

    private:
        bool train();
        bool morph();

        bool training;

        GLANN*      mWorker;
        ANNData*    mLayer1;
        ANNData*    mLayer2;
        ANNData*    mLayer3;

        vector<float> input;
        vector<float> inputHiddenLayer1;
        vector<float> inputHiddenLayer2;
        vector<float> output;
        vector<float> target;
        vector<float> errorHiddenLayer;
        vector<float> error;

        int supersampling;
        long frameCounter;
        int netSize;
        bool trainNet;
        vector<float> globError;
        vector<float> periodicalError;

        ofxMaxiSample signalInput;
        ofxMaxiSample signalT1;
        ofxMaxiSample signalT2;
        ofxMaxiSample signalT3;
        vector<float> audioInputTimeParam;
        vector<float> audioInputF;
        vector<float> audioTargetF;

};
