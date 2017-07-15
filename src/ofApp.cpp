#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(0);
    pe.setup();
    setupDeferred();
    updateDeferredParam();
//    shadowLightPass->setEnabled(false);
    
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
    
    receiver.setup(7401);
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

    while (receiver.hasWaitingMessages()){
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        string address = m.getAddress();
        vector<string> dirs = ofSplitString(address, "/");
        
        if (dirs[1] == "p") {
            int i = ofToInt(dirs[2]);
            float val = m.getArgAsInt32(0) / 128.;
            
            if (i == 0) {
                pl1_rad.set(10.+val*800.);
                lightingPass->getLightRef(0).radius = pl1_rad.get();
            } else if (i == 1) {
                pl1_rad.set(10.+val*800.);
                lightingPass->getLightRef(1).radius = pl2_rad.get();
            } else if (i == 2) {
            } else if (i == 3) {
                dof_focal.set(0.6 * val);
                dofPass->setFocus(dof_focal.get());
            } else if (i == 4) {
                dt.set(0.001 + 2.0 * val);
            }
            
        } else if (dirs[1] == "bang") {
            int i = ofToInt(dirs[2]);
            
            if (i == 0) {
                // randomize scene
                for (int i = 0; i < objs.size(); i++) {
                    if (objs[i]->isEnable()) objs[i]->randomize();
                }
                lp1.to(ofPoint(ofRandom(-800, 800), ofRandom(30, 800), ofRandom(-800, 800)));
                lp2.to(ofPoint(ofRandom(-800, 800), ofRandom(30, 800), ofRandom(-800, 800)));
                
                camPos.to(ofPoint(ofRandom(-1600, 1600), ofRandom(0, 1000), ofRandom(-1600, 1600)));
                camLook.to(ofPoint(ofRandom(-400, 400), ofRandom(-400, 400), ofRandom(-400, 400)));
                
            } else if (i == 1) {
                // randomize effects
                float coin = ofRandom(1.);
                
                if (coin < 0.1) {
                    enableWireFrame();
                } else if (coin < 0.5) {
                    disableWireFrame();
                }
                
                coin = ofRandom(1.);
                if (coin < 0.3) {
                    int index = floor(ofRandom(1.) * objs.size());
                    if (objs[index]->isEnable()) {
                        disableObj(index);
                    } else {
                        if (activeNum < MAX_NUM) {
                            enableObj(index);
                        }
                    }
                }
                
                coin = ofRandom(1.);
                if (coin < 0.15) pe.setMode(0);
                else if (coin < 0.2) pe.setMode(1);
                else if (coin < 0.3) pe.setMode(2);
                else if (coin < 0.4) pe.setMode(3);
                else if (coin < 0.5) pe.disableGrey();
                else if (coin < 0.6) pe.enableGrey(false);
                else if (coin < 0.65) pe.enableGrey(true);
                else if (coin < 0.75) {
                    if (hdrBloomPass->getEnabled()) hdrBloomPass->setEnabled(false);
                    else hdrBloomPass->setEnabled(true);
                }
            }
        }
    }
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
            lp1.to(ofPoint(ofRandom(-800, 800), ofRandom(30, 800), ofRandom(-800, 800)));
            lp2.to(ofPoint(ofRandom(-800, 800), ofRandom(30, 800), ofRandom(-800, 800)));
            
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
        case '0': toggleObj(0); break;
        case '1': toggleObj(1); break;
        case '2': toggleObj(2); break;
        case '3': toggleObj(3); break;
        case '4': toggleObj(4); break;
        case '5': toggleObj(5); break;
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
            pe.disableGrey();
            break;
        case 'n':
            pe.enableGrey(false);
            break;
        case 'm':
            pe.enableGrey(true);
            break;
        case 'l':
            if (hdrBloomPass->getEnabled()) hdrBloomPass->setEnabled(false);
            else hdrBloomPass->setEnabled(true);
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
    ssaoPass->setDarkness(ao_dark.get());
    
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

void ofApp::enableObj(int index){
    objs[index]->enable(); activeNum++;
}
void ofApp::disableObj(int index){
    objs[index]->diswwwable(); activeNum--;
}

void ofApp::toggleObj(int index){
    if (objs[index]->isEnable()) {
        disableObj(index);
    } else {
        enableObj(index);
    }
}
