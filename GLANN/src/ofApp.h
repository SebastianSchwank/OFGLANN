#pragma once

#include "ofMain.h"
#include "ofMath.h"

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
        GLANN*              mWorker;

        ANNData*            mInputL;
        ANNData*            mL0;
        vector<ANNData*>    mLayers;
        int numHLayers = 8;

        int sizeV = 64;
        int freqMax = 10;

        int frameCounter;
        int InputSize,OutputSize;
        bool train;
        vector<float> globError;
        vector<float> periodicalError;

};
