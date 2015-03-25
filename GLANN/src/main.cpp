#include "ofMain.h"
#include "ofApp.h"

// import programmable shaders
#ifdef TARGET_OPENGLES
    #include "ofGLProgrammableRenderer.h"
#endif

//========================================================================
int main(int argc,char* argv[]){

    ofSetLogLevel(OF_LOG_VERBOSE);
	#ifdef TARGET_OPENGLES
        ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	#endif
	ofSetupOpenGL(256,256,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
