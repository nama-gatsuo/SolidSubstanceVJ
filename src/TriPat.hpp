#pragma once
#include "ofMain.h"
#include "ObjBase.hpp"

class TriPat : public ObjBase {
public:
    void setup(){
        
        shader.load("shader/gbuffer.vert", "shader/customShader.frag");
        
        plane = ofMesh::box(3000, 3000, 10, 1,1,1);
        for (int i = 0; i < plane.getNumVertices(); i++) {
            plane.addColor(ofFloatColor(0.5, 0.5, 0.7));
        }
        
        mesh.setMode(OF_PRIMITIVE_TRIANGLES);
        createRandomSeed();
        
        unsigned yn = 10;
        unsigned xn = 10;
        
        float r = 200;
        float dh = r * 1.5;
        float dw = r * sqrt(3.);
        for (int i = 0; i < yn; i++) {
            ofVec3f v[3];
            
            for (int j = 0; j < xn; j++) {
                
                ofVec3f shift(dw * (j-xn*0.5), 0, dh * (i-yn*0.5));
                if (i%2==0) shift.x -= dw * 0.5;
                
                v[0] = ofVec3f(0,0,r) + shift;
                v[1] = ofVec3f(-r*sqrt(3.)*0.5,0,-r*0.5) + shift;
                v[2] = ofVec3f(r*sqrt(3.)*0.5,0,-r*0.5) + shift;
                createTri(v[0], v[1], v[2]);
                
                v[0] = ofVec3f(r*sqrt(3.)*0.5,0,-r*0.5) + shift;
                v[1] = ofVec3f(0,0,r) + shift;
                v[2] = ofVec3f(r*sqrt(3.),0,r) + shift;
                createTri(v[0], v[1], v[2]);
            }
            
            
        }
        
    };
    void update(){};
    void draw(ofCamera& cam, bool isShadow){
        ofMatrix4x4 normalMatrix = ofMatrix4x4::getTransposedOf(cam.getModelViewMatrix().getInverse());
        shader.begin();
        shader.setUniform1i("isShadow", isShadow?1:0);
        shader.setUniformMatrix4f("normalMatrix", normalMatrix);
        shader.setUniform1f("farClip", cam.getFarClip());
        shader.setUniform1f("nearClip", cam.getNearClip());
        mesh.draw();
//        plane.draw();
        shader.end();
    };
    
    void randomize(){};
    void setParams(int ch, float vol){};
    
private:
    void createRandomSeed(){
        randomSeeds.clear();
        for (int i = 0; i < num; i++) {
            vector<float> trs;
            for (int j = 0; j < num-i; j++) {
                trs.push_back(ofRandom(1.));
            }
            randomSeeds.push_back(trs);
        }
    };
    void createTri(ofVec3f v1, ofVec3f v2, ofVec3f v3){
        ofVec3f v[3];
        v[0] = v1;
        v[1] = v2;
        v[2] = v3;
        
        ofVec3f center = (v[0] + v[1] + v[2]) / 3.0;
        
        for (int i = 0; i < 3; i++) {
            int i1 = i, i2 = i+1;
            if (i == 2) i2 = 0;
            
            createPat(v[i1], center, (v[i2] + v[i1]) * 0.5);
            createPat(v[i2], center, (v[i2] + v[i1]) * 0.5);
            
        }
        
    };
    void createPat(ofVec3f v1, ofVec3f v2, ofVec3f v3){
        
        for (int i = 0; i < num; i++) {
            
            ofVec3f start = v1.getInterpolated(v3, (float)i/num);
            ofVec3f next;
            for (int j = 0; j < num-i; j++){
                
                ofVec3f target;
                if (randomSeeds[i][j] < 0.5) {
                    target = (v2 - v1) * (1.0 / num);
                } else if (randomSeeds[i][j] < 0.85) {
                    target = (v3 - v1) * (1.0 / num);
                } else {
                    ofVec3f t1 = (v2 - v1) * (1.0 / num);
                    ofVec3f t2 = (v3 - v1) * (1.0 / num);
                    
                    target = t1 * 2.0 - t2;
                }
                
                next = start + target;
                
                addBox(start, next, ofNoise(i*randomSeeds[i][j], j*randomSeeds[i][j]));
                
                start = next;
            }
        }
        
    };
    void addBox(ofVec3f start, ofVec3f end, float width){
        float w = 40. * width;
        ofVec3f side = w * (start-end).getCrossed(ofVec3f(0,1.,0)).normalize();
        ofVec3f exc(0,60 * width ,0);
        ofFloatColor c;
        if (width > 0.4 && width < 0.6) {
            c.set(0.8, 0.8, 1.6);
        } else {
            c.set(0.5, 0.5, 0.7);
        }
        
        //
        mesh.addVertex(end + exc);
        mesh.addVertex(start + exc);
        mesh.addVertex(start - side);
        
        mesh.addVertex(start - side);
        mesh.addVertex(end - side);
        mesh.addVertex(end + exc);
        
        for (int i = 0; i < 6; i++) {
            ofVec3f v1 = end + exc,
            v2 = start + exc,
            v3 = start - side;
            
            mesh.addNormal((v2-v1).getCrossed(v3-v1));
            mesh.addColor(c);
        }
        
        //
        mesh.addVertex(end + exc);
        mesh.addVertex(start + exc);
        mesh.addVertex(start + side);
        
        mesh.addVertex(start +side);
        mesh.addVertex(end + side);
        mesh.addVertex(end + exc);

        for (int i = 0; i < 6; i++) {
            ofVec3f v1 = end + exc,
            v2 = start + side,
            v3 = start + exc;
            
            mesh.addNormal((v2-v1).getCrossed(v3-v1));
            mesh.addColor(c);
        }
        
        //
    };
    
    ofVboMesh mesh;
    ofVboMesh plane;
    ofShader shader;
    
    const unsigned num = 6;
    vector<vector<float>> randomSeeds;
    
};
