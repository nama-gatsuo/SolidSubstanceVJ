#pragma once
#include "ofMain.h"

class ObjBase{
public:
    virtual void setup(){};
    virtual void update(float dt){};
    virtual void draw(ofCamera& cam, bool isShadow){};
    virtual void randomize(){};
    virtual void setParam(int ch, float val){};
    
    bool isEnable(){ return isEnabled; };
    void enable(){ isEnabled = true; };
    void disable(){ isEnabled = false; };
    
    void enableWireFrame(){ drawMode = OF_MESH_WIREFRAME; };
    void disableWireFrame(){ drawMode = OF_MESH_FILL; };
    
    bool isEnabled = false;
    ofPolyRenderMode drawMode = OF_MESH_FILL;
};
