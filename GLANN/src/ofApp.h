#pragma once

#include "ofMain.h"

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
        GLANN*      mWorker;
        ANNData*    mLayer0;
        ANNData*    mLayer1;


        int frameCounter;
        int netSize;
        bool train;
        vector<float> globError;
        vector<float> periodicalError;

};
