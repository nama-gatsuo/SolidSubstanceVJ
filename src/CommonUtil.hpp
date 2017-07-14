#pragma once
#include "ofMain.h"

class SmoothPoint : public ofPoint{
public:
    SmoothPoint():mSpeed(0.05){};
    void update(float dt){
        this->x += (mTarget.x - this->x) * mSpeed * dt;
        this->y += (mTarget.y - this->y) * mSpeed * dt;
        this->z += (mTarget.z - this->z) * mSpeed * dt;
    }
    void to(ofPoint target){
        mTarget = target;
    }
    void setSpeed(float speed){
        mSpeed = speed;
    }
    
private:
    ofPoint mTarget;
    float mSpeed;
};

class SmoothValue {
public:
    SmoothValue():mSpeed(0.05),mValue(0),mTarget(0){}
    
    void update(float dt){
        mValue += (mTarget - mValue) * mSpeed * dt;
    }
    void to(float target){
        mTarget = target;
    }
    float getValue(){
        return mValue;
    };
    void setSpeed(float speed){
        mSpeed = speed;
    }
    
private:
    float mValue;
    float mTarget;
    float mSpeed;
};
