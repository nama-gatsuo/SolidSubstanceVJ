#pragma once
#include "ofMain.h"

class ObjBase{
public:
    virtual void setup(){};
    virtual void update(){};
    virtual void draw(ofCamera& cam, bool isShadow){};
    virtual void randomize(){};
    virtual void setParam(int ch, float val){};
    
    void enableWireFrame(){ drawMode = OF_MESH_WIREFRAME; };
    void disableWireFrame(){ drawMode = OF_MESH_FILL; };
    
    ofPolyRenderMode drawMode = OF_MESH_FILL;
};
