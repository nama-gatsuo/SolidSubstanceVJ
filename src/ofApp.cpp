#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(0);
    pe.setup();
    setupDeferred();
//    shadowLightPass->setEnabled(false);
//    hdrBloomPass->setEnabled(false);
    panel.add(dt.set("dt", 1.0, 0.001, 2.0));
    
    shared_ptr<ObjBase> o0(new CalatravaStruct());
    o0->setup();
    objs.push_back(o0);
    
    shared_ptr<ObjBase> o1(new MatInst());
    o1->setup();
    objs.push_back(o1);
    
    shared_ptr<ObjBase> o2(new TriWall());
    o2->setup();
    objs.push_back(o2);
    
    shared_ptr<ObjBase> o3(new TriPat());
    o3->setup();
    objs.push_back(o3);
    
    shared_ptr<ObjBase> o4(new FractInst());
    o4->setup();
    objs.push_back(o4);
    
    shared_ptr<ObjBase> o5(new RandomJoint());
    o5->setup();
    objs.push_back(o5);
    
    camPos.setSpeed(0.005);
    
    disableWireFrame();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    for (int i = 0; i < objs.size(); i++) {
        if (objs[i]->isEnable()) objs[i]->update(dt.get());
    }
    lp1.update(dt.get());
    lp2.update(dt.get());
    lightingPass->getLightRef(0).position = lp1;
    lightingPass->getLightRef(1).position = lp2;

    camPos.update(dt.get());
    cam.setPosition(camPos);
    cam.lookAt(camLook);
    if (isShow) updateDeferredParam();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (isWire) {
        pe.begin();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        cam.begin();
    } else {
        shadowLightPass->beginShadowMap(true);
        ofCamera sc = shadowLightPass->getCam();
        for (int i = 0; i < objs.size(); i++) {
            if (objs[i]->isEnable()) objs[i]->draw(sc, true);
        }
        lightingPass->drawLights(sc, true);
        shadowLightPass->endShadowMap();
        
        deferred.begin(cam, true);
    }
    
    for (int i = 0; i < objs.size(); i++) {
        if (objs[i]->isEnable()) objs[i]->draw(cam, false);
    }
    
    if (isWire) {
        lightingPass->drawLights(cam, false, OF_MESH_WIREFRAME);
        cam.end();
        ofDisableBlendMode();
        pe.end();
        pe.draw();
    } else {
        lightingPass->drawLights(cam, false);
        deferred.end(false);
        
        pe.begin();
        deferred.draw();
        pe.end();
        pe.draw();
    }
    
    if (isShow) panel.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key) {
        case ' ':
            for (int i = 0; i < objs.size(); i++) {
                if (objs[i]->isEnable()) objs[i]->randomize();
            }
            lp1.to(ofPoint(ofRandom(-800, 800), ofRandom(0, 800), ofRandom(-800, 800)));
            lp2.to(ofPoint(ofRandom(-800, 800), ofRandom(0, 800), ofRandom(-800, 800)));
            
            camPos.to(ofPoint(ofRandom(-1600, 1600), ofRandom(0, 1000), ofRandom(-1600, 1600)));
            camLook.to(ofPoint(ofRandom(-400, 400), ofRandom(-400, 400), ofRandom(-400, 400)));
            break;
        case 'h':
            isShow = !isShow;
            break;
        case 'w':
            if (isWire) disableWireFrame();
            else enableWireFrame();
            break;
        case '0':
            if (objs[0]->isEnable()) objs[0]->disable();
            else objs[0]->enable();
            break;
        case '1':
            if (objs[1]->isEnable()) objs[1]->disable();
            else objs[1]->enable();
            break;
        case '2':
            if (objs[2]->isEnable()) objs[2]->disable();
            else objs[2]->enable();
            break;
        case '3':
            if (objs[3]->isEnable()) objs[3]->disable();
            else objs[3]->enable();
            break;
        case '4':
            if (objs[4]->isEnable()) objs[4]->disable();
            else objs[4]->enable();
            break;
        case '5':
            if (objs[5]->isEnable()) objs[5]->disable();
            else objs[5]->enable();
            break;
        case 'a':
            pe.setMode(0);
            break;
        case 's':
            pe.setMode(1);
            break;
        case 'd':
            pe.setMode(2);
            break;
        case 'f':
            pe.setMode(3);
            break;
        case 'b':
            pe.toggleNega();
            break;
        default:
            break;
    }
}


//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

void ofApp::setupDeferred(){
    deferred.init(ofGetWidth(), ofGetHeight());
    ssaoPass = deferred.createPass<SsaoPass>().get();
    
    shadowLightPass = deferred.createPass<ShadowLightPass>().get();
    shadowLightPass->lookAt(ofVec3f(0.0));
    shadowLightPass->setCam(60, 0.1, 4000);
    shadowLightPass->setPosition(0, 1500.0, 500);
    shadowLightPass->lookAt(ofVec3f(0.0));

    
    lightingPass = deferred.createPass<PointLightPass>().get();
    ofxDeferredShading::PointLight dlight;
    dlight.ambientColor = ofFloatColor(0.005);
    lightingPass->addLight(dlight);
    
    dlight.ambientColor = ofFloatColor(0.0);
    lightingPass->addLight(dlight);
    
    hdrBloomPass = deferred.createPass<HdrBloomPass>().get();
    dofPass = deferred.createPass<DofPass>().get();
    
    // gui
    panel.setup();
    pl1.setName("Point light 1");
    pl1.add(pl1_diff.set("Diffuse Color", ofFloatColor(0.4)));
    pl1.add(pl1_spe.set("Specular Color", ofFloatColor(1.0)));
    pl1.add(pl1_rad.set("Radius", 500, 100, 2000));
    pl1.add(pl1_int.set("Intensity", 1.2, 0.1, 3.0));
    panel.add(pl1);
    
    pl2.setName("Point light 2");
    pl2.add(pl2_diff.set("Diffuse Color", ofFloatColor(0.4)));
    pl2.add(pl2_spe.set("Specular Color", ofFloatColor(1.0)));
    pl2.add(pl2_rad.set("Radius", 500, 100, 2000));
    pl2.add(pl2_int.set("Intensity", 1.2, 0.1, 3.0));
    panel.add(pl2);
    
    ao.setName("Ambient Occlusion");
    ao.add(ao_rad.set("Occlusion Radius", 5.0, 0.1, 100.0));
    ao.add(ao_dark.set("Darkness", 0.8, 0.1, 5.0));
    panel.add(ao);
    
    shadow.setName("Shadow Light");
    shadow.add(sha_amb.set("Ambient", 0.0, 0.0, 1.0));
    shadow.add(sha_dif.set("Diffuse", 0.0, 0.3, 1.0));
    shadow.add(sha_dark.set("Shadow Darkness", 0.4, 0.0, 1.0));
    shadow.add(sha_blend.set("Lighting Blend", 0.4, 0.0, 1.0));
    panel.add(shadow);
    
    dof.setName("Defocus Blur");
    dof.add(dof_blur.set("Max Blur", 0.5, 0.0, 1.0));
    dof.add(dof_ape.set("Aperture", 0.1, 0.0, 1.0));
    dof.add(dof_focal.set("Focus Distance", 0.2, 0.0, 1.0));
    panel.add(dof);
}

void ofApp::updateDeferredParam(){
    
    lightingPass->getLightRef(0).diffuseColor = pl1_diff.get();
    lightingPass->getLightRef(0).specularColor = pl1_spe.get();
    lightingPass->getLightRef(0).radius = pl1_rad.get();
    lightingPass->getLightRef(0).intensity = pl1_int.get();
    
    lightingPass->getLightRef(1).diffuseColor = pl2_diff.get();
    lightingPass->getLightRef(1).specularColor = pl2_spe.get();
    lightingPass->getLightRef(1).intensity = pl2_int.get();
    lightingPass->getLightRef(1).radius = pl2_rad.get();
    
    ssaoPass->setOcculusionRadius(ao_rad.get());
    ssaoPass->setDrakness(ao_dark.get());
    
    shadowLightPass->setAmbientColor(ofFloatColor(sha_amb.get()));
    shadowLightPass->setDiffuseColor(ofFloatColor(sha_dif.get()));
    shadowLightPass->setDarkness(sha_dark.get());
    shadowLightPass->setBlend(sha_blend.get());
    
    
    dofPass->setFocus(dof_focal.get());
    dofPass->setMaxBlur(dof_blur.get());
    dofPass->setAperture(dof_ape.get());
    
}

void ofApp::enableWireFrame(){
    isWire = true;
    for (int i = 0; i < objs.size(); i++) {
        objs[i]->enableWireFrame();
    }
}

void ofApp::disableWireFrame(){
    isWire = false;
    for (int i = 0; i < objs.size(); i++) {
        objs[i]->disableWireFrame();
    }
}

void ofApp::setSpeed(float s){
    for (int i = 0; i < objs.size(); i++) {
        
    }
}
