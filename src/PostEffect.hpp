#pragma once
#include "ofMain.h"

class PostEffect {
public:
    void setup();
    void begin();
    void end();
    void draw();
    
    void bang();
    void setMode(int mode);
    void setParam(int ch, float val);
    void enableNega();
    void disableNega();
    void toggleNega();
    
private:
    ofFbo base;
    ofFbo composite;
    
    int mode = 0;
    float params[3] = { 1.0, 0.1, 0.0 };
    ofShader complexConv;
    ofShader mirrorConv;
    
    bool isNega = false;
    ofShader negaConv;
    ofShader gammaConv;
    
};
