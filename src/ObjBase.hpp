#pragma once
#include "ofCamera.h"

class ObjBase{
public:
    virtual void setup(){};
    virtual void update(){};
    virtual void draw(ofCamera& cam, bool isShadow){};
    virtual void randomize(){};
    virtual void setParam(int ch, float val){};
};
