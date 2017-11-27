#include "CalatravaStruct.hpp"

void CalatravaStruct::setup(){
    shader.load("shader/scene/CalatravaStruct.vert", "shader/scene/shader.frag");
    
    shader.begin();
    shader.setUniform1i("total", num);
    shader.end();
    
    mesh = ofMesh::box(1.,1.,1., 1,1,1);
    
    ofFloatColor col;
    col.set(1);
    
    for (int i = 0; i < mesh.getNumIndices(); i++){
        mesh.addColor(col);
    }
    
    randomize();
    
    fromTrans.setSpeed(0.005);
    fromRotate.setSpeed(0.005);
    fromScale.setSpeed(0.005);
    toTrans.setSpeed(0.005);
    toRotate.setSpeed(0.005);
    toScale.setSpeed(0.005);
}
void CalatravaStruct::update(float dt){
    fromTrans.update(dt);
    fromRotate.update(dt);
    fromScale.update(dt);
    toTrans.update(dt);
    toRotate.update(dt);
    toScale.update(dt);
}

void CalatravaStruct::draw(ofCamera& cam, bool isShadow){
    ofMatrix4x4 normalMatrix = ofMatrix4x4::getTransposedOf(cam.getModelViewMatrix().getInverse());
    
    shader.begin();
    shader.setUniform1i("isShadow", isShadow?1:0);
    shader.setUniform1i("isWire", drawMode==OF_MESH_WIREFRAME?1:0);
    shader.setUniformMatrix4f("normalMatrix", normalMatrix);
    shader.setUniform1f("farClip", cam.getFarClip());
    shader.setUniform1f("nearClip", cam.getNearClip());
    
    shader.setUniform3f("fromTrans", fromTrans);
    shader.setUniform3f("fromRotate", fromRotate);
    shader.setUniform3f("fromScale", fromScale);
    shader.setUniform3f("toTrans", toTrans);
    shader.setUniform3f("toRotate", toRotate);
    shader.setUniform3f("toScale", toScale);
    
    mesh.drawInstanced(drawMode, num);
    
    shader.end();
}

void CalatravaStruct::randomize(){
    
    fromTrans.to(ofPoint(ofRandom(1000),ofRandom(1000),ofRandom(1000)));
    fromRotate.to(ofPoint(ofRandom(TWO_PI*2.),ofRandom(TWO_PI*2.),ofRandom(TWO_PI*2.)));
    fromScale.to(ofPoint(5.,5.,800));
    toTrans.to(ofPoint(ofRandom(2000),ofRandom(2000),ofRandom(2000)));
    toRotate.to(ofPoint(ofRandom(TWO_PI*2.),ofRandom(TWO_PI*2.),ofRandom(TWO_PI*2.)));
    toScale.to(ofPoint(5.,5.,800));;
    
}
void CalatravaStruct::setParam(int ch, float val){

}
