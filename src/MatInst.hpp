#pragma once
#include "ofMain.h"
#include "ObjBase.hpp"
#include "CommonUtil.hpp"

class MatInst : public ObjBase {
public:
    void setup(){
        createMesh();
        shader.load("shader/scene/MatInst.vert", "shader/scene/shader.frag");
        shader.begin();
        shader.setUniform2f("size", xSize, ySize);
        shader.end();
        
        rot1.setSpeed(0.005);
        scale1.setSpeed(0.005);
        trans1.setSpeed(0.05);
        rot2.setSpeed(0.005);
        scale2.setSpeed(0.005);
        trans2;
    };
    void update(float dt){
        rot1.update(dt);
        scale1.update(dt);
        trans1.update(dt);
        rot2.update(dt);
        scale2.update(dt);
        trans2.update(dt);
    };
    void draw(ofCamera &cam, bool isShadow){
        ofMatrix4x4 normalMatrix = ofMatrix4x4::getTransposedOf(cam.getModelViewMatrix().getInverse());
        
        shader.begin();
        shader.setUniform1i("isShadow",isShadow?1:0);
        shader.setUniform1i("isWire", drawMode==OF_MESH_WIREFRAME?1:0);
        shader.setUniformMatrix4f("normalMatrix", normalMatrix);
        shader.setUniform1f("farClip", cam.getFarClip());
        shader.setUniform1f("nearClip", cam.getNearClip());
        
        shader.setUniform3f("rot1", rot1);
        shader.setUniform3f("scale1", scale1);
        shader.setUniform3f("trans1", trans1);
        shader.setUniform3f("rot2", rot2);
        shader.setUniform3f("scale2", scale2);
        shader.setUniform3f("trans2", trans2);
        mesh.drawInstanced(drawMode, xSize * ySize);
        shader.end();
    };
    void randomize(){
        float tfactor = 300;
        float sMin = 1.0;
        float sMax = 2.0;
        float rMin = TWO_PI*1.0;
        float rMax = TWO_PI*3.0;
        
        rot1.to(ofPoint(ofRandom(rMin, rMax), ofRandom(rMin, rMax), ofRandom(rMin, rMax)));
        scale1.to(ofPoint(ofRandom(sMin, sMax), ofRandom(sMin, sMax), ofRandom(sMin, sMax)));
        trans1.to(ofPoint(ofRandom(1.0)*tfactor, ofRandom(1.0)*tfactor, ofRandom(1.0)*tfactor));
        rot2.to(ofPoint(ofRandom(rMin, rMax), ofRandom(rMin, rMax), ofRandom(rMin, rMax)));
        scale2.to(ofPoint(ofRandom(sMin, sMax), ofRandom(sMin, sMax), ofRandom(sMin, sMax)));
        trans2.to(ofPoint(ofRandom(1.0)*tfactor, ofRandom(1.0)*tfactor, ofRandom(1.0)*tfactor));
    };
    void setParams(int ch, float vol){};
private:
    void createMesh(){
        
        ofFloatColor c1;
        c1.set(1.0);
        ofFloatColor c2;
        c2.setHsb(0.5, 0.6, 1.);
        
        float w = 10.0;
        float h = 10.0;
        float d = 10.0;
        
        float t = 1.;
        w += t;
        h += t;
        d += t;
        
        ofMatrix4x4 m;
        m.makeIdentityMatrix();
        m.glTranslate(w/2, h/2, 0);
        m.glScale(t, t, d);
        createBox(m, c1);
        
        m.makeIdentityMatrix();
        m.glTranslate(-w/2, h/2, 0);
        m.glScale(t, t, d);
        createBox(m, c1);
        
        m.makeIdentityMatrix();
        m.glTranslate(0, h/2, d/2);
        m.glScale(w, t, t);
        createBox(m, c1);
        
        m.makeIdentityMatrix();
        m.glTranslate(0, h/2, -d/2);
        m.glScale(w, t, t);
        createBox(m, c1);
        
        //
        m.makeIdentityMatrix();
        m.glTranslate(w/2, 0, d/2);
        m.glScale(t, h, t);
        createBox(m, c1);
        
        m.makeIdentityMatrix();
        m.glTranslate(w/2, 0, -d/2);
        m.glScale(t, h, t);
        createBox(m, c1);
        
        m.makeIdentityMatrix();
        m.glTranslate(-w/2, 0, -d/2);
        m.glScale(t, h, t);
        createBox(m, c1);
        
        m.makeIdentityMatrix();
        m.glTranslate(-w/2, 0, d/2);
        m.glScale(t, h, t);
        createBox(m, c1);
        
        //
        m.makeIdentityMatrix();
        m.glTranslate(w/2, -h/2, 0);
        m.glScale(t, t, d);
        createBox(m, c1);
        
        m.makeIdentityMatrix();
        m.glTranslate(-w/2, -h/2, 0);
        m.glScale(t, t, d);
        createBox(m, c1);
        
        m.makeIdentityMatrix();
        m.glTranslate(0, -h/2, d/2);
        m.glScale(w, t, t);
        createBox(m, c1);
        
        m.makeIdentityMatrix();
        m.glTranslate(0, -h/2, -d/2);
        m.glScale(w, t, t);
        createBox(m, c1);
        
        //
//        m.makeIdentityMatrix();
//        m.glRotate(45, 1,0,0);
//        m.glRotate(45, 0,1,0);
//        m.glRotate(45, 0,0,1);
//        m.glScale(w * 0.9, h * 0.9, d * 0.9);
//        createBox(m, c2);
//
        
    };
    void createBox(const ofMatrix4x4& mat, const ofFloatColor& c){
        ofMesh box = ofMesh::box(1.0, 1.0, 1.0, 1, 1, 1);
        
        for (int i = 0; i < box.getNumVertices(); i++) {
            ofVec3f v = box.getVertex(i) * mat;
            ofVec3f n = ((box.getNormal(i) + v) * mat - (v * mat)).normalize();
            
            box.setVertex(i, v);
            box.setNormal(i, n);
            box.addColor(ofFloatColor(c.r, c.g, c.b));
        }
        mesh.append(box);
    };
    
    const int xSize = 64;
    const int ySize = 64;
    
    ofVboMesh mesh;
    ofShader shader;
    
    SmoothPoint rot1;
    SmoothPoint rot2;
    SmoothPoint scale1;
    SmoothPoint scale2;
    SmoothPoint trans1;
    SmoothPoint trans2;
};
