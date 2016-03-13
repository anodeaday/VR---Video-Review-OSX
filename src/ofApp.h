#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void initvideo(string videopath);
        void initGui();
        void resetCamera();
    
    void onSliderEvent(ofxDatGuiSliderEvent e);
    
    void onButtonEvent(ofxDatGuiButtonEvent e);

    ofxDatGuiButton* pauseBtn;
    bool paused;
    
    ofxDatGuiButton* latlongBtn;
    bool latlong;
    ofxDatGuiButton* fullScreenBtn;
    bool fullScreen;
    
    string videopath;
    bool videoloaded;
    ofVideoPlayer video;
    
    //GUI
    ofxDatGui* gui;
    ofxDatGuiLabel* label;
    ofxDatGuiSlider* slider;
    
    //Video Sphere
    ofFbo fbo;
    
    ofCamera camera;
    ofSpherePrimitive monoSphere;
    float cameraRotationX, cameraRotationY, oldPitch;
    
    float oldTouchX,oldTouchY,newTouchX,newTouchY;
    float centerX,centerY;
    ofQuaternion cameraHeading,cameraPitch;

    bool inwindow;
};
