#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetDataPathRoot("../Resources/data/");
    
    inwindow = false;
    paused=false;
    latlong=false;
    fullScreen=false;
    
    ofEnableNormalizedTexCoords();
    
    ofEnableDepthTest();
    ofDisableAlphaBlending();
    ofDisableArbTex();
    
    
    videoloaded=false;
    videopath="";
    
    fbo.allocate(1920,1080,GL_RGB);
    fbo.begin();
    ofClear(0,0,0);
    fbo.end();
    
    //______________________________________________________________GUI
    
    gui = new ofxDatGui(ofGetWidth()*0.05,(ofGetWidth()/1.777778)+10);

    gui->setWidth(ofGetWidth()*0.9);
    
    label = gui->addLabel("360 Video Review");
    
    gui->addBreak()->setHeight(5.0f);
    slider = gui->addSlider("Video Progress", 0, 100);
    
    slider->onSliderEvent(this, &ofApp::onSliderEvent);
    slider->setPrecision(0);

    pauseBtn = gui->addButton("PAUSE");
//    pauseBtn->setWidth(gui->getWidth()*0.5);
    pauseBtn->onButtonEvent(this, &ofApp::onButtonEvent);
    pauseBtn->setWidth(gui->getWidth()*0.33);
    pauseBtn->setIndex(1);

    latlongBtn = gui->addButton("LATLONG");
    latlongBtn->onButtonEvent(this, &ofApp::onButtonEvent);
    latlongBtn->setPosition(pauseBtn->getWidth()+pauseBtn->getX(),pauseBtn->getY());
    latlongBtn->setWidth(gui->getWidth()*0.33);
    latlongBtn->setIndex(2);
    
    fullScreenBtn = gui->addButton("FULLSCREEN");
    fullScreenBtn->onButtonEvent(this, &ofApp::onButtonEvent);
    fullScreenBtn->setPosition(latlongBtn->getWidth()+pauseBtn->getX(),pauseBtn->getY());
    fullScreenBtn->setWidth(gui->getWidth()*0.33);
    fullScreenBtn->setIndex(3);
    
    //______________________________________________________________Video
    // Setup sphere video
    
    camera.setupPerspective();
    camera.setFov(75);
    camera.setVFlip(false);
    camera.setPosition(0, 0, 0);
    camera.setOrientation(ofVec3f(0,0,0)); // FIX SPHERE ORIENTATION

    monoSphere.setRadius(500);
    monoSphere.setPosition(0,0,0);
    monoSphere.setOrientation(ofVec3f(0,0,0)); // FIX SPHERE ORIENTATION
    monoSphere.setResolution(100);
    monoSphere.mapTexCoords(1,1, 0,0);

    cameraRotationX=0.0f;
    cameraRotationY=0.0f;
    
}
//--------------------------------------------------------------
void ofApp::initGui(){

}

//--------------------------------------------------------------
void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)
{
    if(e.target == slider){
        video.setFrame(e.value);

        video.update();
    }
}

//--------------------------------------------------------------
void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
    if (e.target->getLabel() == "PAUSE")
    {
        paused=!paused;
        video.setPaused(paused);
    }
    else if (e.target->getLabel() == "LATLONG")
    {
        latlong=!latlong;
    }
    else if (e.target->getLabel() == "FULLSCREEN")
    {
        fullScreen =!fullScreen;
        ofSetFullscreen(fullScreen);
        if (!fullScreen)
        {
            ofSetWindowShape(ofGetScreenWidth()*0.7, ofGetScreenWidth()*0.7);
            ofSetWindowPosition(ofGetScreenWidth()*0.5-ofGetScreenWidth()*0.35, ofGetScreenHeight()*0.5-ofGetScreenHeight()*0.35);
        }
        else{
            gui->setWidth(900);
            gui->update();
            pauseBtn->setWidth(200);
            
            latlongBtn->setPosition(400,pauseBtn->getY());
            latlongBtn->setWidth(200);
            
            fullScreenBtn->setPosition(700,pauseBtn->getY());
            fullScreenBtn->setWidth(200);
        }
    }
   
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //______________________________________________________________Video
    if(videoloaded)
    {
            pauseBtn->update();
            slider->setValue(video.getCurrentFrame());
            slider->setMax(video.getTotalNumFrames());
            slider->update();
            
            ofEnableNormalizedTexCoords();
            ofDisableDepthTest();
            ofEnableAlphaBlending();
            ofEnableArbTex();

            
            ofVec3f rotation (cameraRotationY,cameraRotationX,0);
            camera.setOrientation(rotation);
            video.update();

             video.getTexture().bind();
            fbo.begin();
           
            camera.begin();


            monoSphere.draw();
            camera.end();

            fbo.end();
            video.getTexture().unbind();

     
    }
    else{
        fbo.begin();
        ofClear(0,0,0);
        fbo.end();

    }
    
    //______________________________________________________________GUI
    
    gui->update();
    latlongBtn->update();
    fullScreenBtn->update();

}

//--------------------------------------------------------------
void ofApp::resetCamera(){
    cameraRotationX=0;
    cameraRotationY=0;
}

//--------------------------------------------------------------
void ofApp::draw(){
     //______________________________________________________________GUI
    ofBackground(ofColor(0,0,0));

    
    
    gui->draw();
    
    
    if (videoloaded){
        if(!latlong)
        {
            ofEnableNormalizedTexCoords();
            fbo.draw(0,0,ofGetWidth(),ofGetWidth()/1.777778);
        }
        else
        {
            ofDisableNormalizedTexCoords();
            video.draw(0,0,ofGetWidth(),ofGetWidth()*0.5);
        }
    }
    else{
        string Message = "DRAG A 360 video into this window";
        ofDrawBitmapString(Message,ofGetWidth()*0.4,ofGetHeight()/2);
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key==32){
        paused=!paused;
        video.setPaused(paused);
    }
    if(key=='j'){
        video.previousFrame();
        video.update();
        
    }
    if(key=='k'){
        if(video.isPaused()) {
            paused=false;
            video.setPaused(paused);
        }
        else{
            paused=!paused;
            video.setPaused(paused);
        }
    }
    if(key=='l'){
        video.nextFrame();
        video.update();
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if (inwindow){

        newTouchX = (centerX-x)*0.02;
        newTouchY = (centerY-y)*0.02;
    //    cout<<newTouchY;
        cameraRotationX+=newTouchX;
        cameraRotationY+=newTouchY;
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if (y<ofGetWidth()/1.77778){
        inwindow=true;
    }
    else{
        inwindow=false;
    }

        centerX = x;
        centerY = y;

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    oldTouchX = x;
    oldTouchY = y;
    
    

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    if(!fullScreen){
            gui->setWidth(ofGetWidth()*0.9);

    }
        gui->setPosition(ofGetWidth()*0.05,(ofGetWidth()/1.777778));

    if (fullScreen)
    {
        
        gui->setWidth(900);

        pauseBtn->setWidth(100);

        
        latlongBtn->setPosition(300,pauseBtn->getY());
        latlongBtn->setWidth(300);
        
        fullScreenBtn->setPosition(600,pauseBtn->getY());
        fullScreenBtn->setWidth(300);
    }
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

void ofApp::initvideo(string videopath){
    resetCamera();
    video.load(videopath);
    video.play();
    videoloaded=true;
    string videofile="";
    for (int i=videopath.length(); i>0; i--)
    {
        videofile = videofile + videopath[i];
        if ((videopath[i] == '/') || (videopath[i] == '\\'))
        {
            int foundslash = i;
            string reversed = "";
            
            for (int x=videofile.length();x>0;x--)
            {
                reversed=reversed+videofile[x];
            }
            
            videofile="360 REVIEW - " + reversed;
//            cout<< videofile;

            label->setLabel(videofile);
            break;
        }
        
    }
    
}



//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

    videopath = ofToString(dragInfo.files[0]);
    cout<<ofToString(dragInfo.files[0]);
    
    if (videoloaded)
    {
        videoloaded=false;
//        video.closeMovie();

        initvideo(videopath);
        
        
    }
    else{
        initvideo(videopath);
    }
}

