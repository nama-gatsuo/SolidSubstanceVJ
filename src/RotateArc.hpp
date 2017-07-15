#include "ofMain.h"
#include "ObjBase.hpp"
#include "CommonUtil.hpp"

class RotateArc : public ObjBase {
public:
    void setup(){
        
        shader.load("shader/scene/RotateArc.vert", "shader/scene/shader.frag");
        
        mesh = ofMesh::box(1.,1.,1., 1,1,1);
        
        for (int i = 0; i < num; i++) {
            
            float coin = ofRandom(1.0);
            ofVec3f col;
            if (coin < 0.9) col.set(0.5);
            else col.set(0.9, 8.0, 1.9);
            
            int n = floor(ofRandom(1.0)*20.) + 20;
            
            coin = ofRandom(1.0);
            
            SmoothPoint s;
            cols.push_back(col);
            nums.push_back(n);
            boxScale.push_back(s);
            
            SmoothValue sv;
            radius.push_back(sv);
            rotation.push_back(ofVec3f(ofRandom(PI), ofRandom(PI), ofRandom(PI)));
        }
        randomize();
    };
    void update(float dt){
        for (int i = 0; i < num; i++) {
            boxScale[i].update(dt);
            radius[i].update(dt);
        }
        t += dt * 0.01;
    };
    void draw(ofCamera& cam, bool isShadow){
        
        ofMatrix4x4 normalMatrix = ofMatrix4x4::getTransposedOf(cam.getModelViewMatrix().getInverse());
        
        for (int i = 0; i < num; i++) {
            shader.begin();
            shader.setUniform1i("isShadow", isShadow?1:0);
            shader.setUniform1i("isWire", drawMode==OF_MESH_WIREFRAME?1:0);
            shader.setUniformMatrix4f("normalMatrix", normalMatrix);
            shader.setUniform1f("farClip", cam.getFarClip());
            shader.setUniform1f("nearClip", cam.getNearClip());
            
            shader.setUniform3f("col", cols[i]);
            shader.setUniform1f("time", t);
            shader.setUniform1i("num", nums[i]);
            shader.setUniform1f("radius", radius[i].getValue() * (1.5 + cos(ofGetElapsedTimef()*0.2+nums[i])));
            shader.setUniform3f("boxScale", boxScale[i]);
            shader.setUniform3f("rot", rotation[i]);
            
            mesh.drawInstanced(drawMode, nums[i]);
            shader.end();
        }
        
    };
    void randomize(){
        for (int i = 0; i < num; i++) {
            float coin = ofRandom(1.0);
            if (coin < 0.33) boxScale[i].to(ofPoint(10*ofRandom(4., 6.), 20, 200));
            else if (coin < 0.66) boxScale[i].to(ofPoint(10, 10*ofRandom(4., 4.), 200));
            else boxScale[i].to(ofPoint(20) * ofRandom(1., 2.));
            
            radius[i].to(400. * ofRandom(1.0, 2.0));
        }
    };
    void setParam(int ch, float val){};
private:
    int num = 16;
    ofVboMesh mesh;
    ofShader shader;
    
    vector<int> nums;
    vector<ofVec3f> cols;
    vector<SmoothPoint> boxScale;
    vector<ofVec3f> rotation;
    vector<SmoothValue> radius;
    
    float t = 0.0;
    
};
