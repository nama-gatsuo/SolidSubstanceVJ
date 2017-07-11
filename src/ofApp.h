#pragma once

#include "ofMain.h"
#include "ObjBase.hpp"
#include "CommonUtil.hpp"

#include "ofxOsc.h"
#include "ofxDeferredShading.h"


class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void windowResized(int w, int h);
	
private:
    vector<shared_ptr<ObjBase>> objs;
    
};
