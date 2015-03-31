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
        bool train(int stepsize);
        bool morph();

        bool training;

        GLANN*      mWorker;
        ANNData*    mLayer1;
        ANNData*    mLayer2;

        vector<float> input;
        vector<float> inputHiddenLayer;
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
        ofxMaxiSample signalTarget;
        vector<float> audioInputF;
        vector<float> audioTargetF;

        vector<float> outputF;

};
