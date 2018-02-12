#include "ofApp.h"
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup(){
    glEnable(GL_DEPTH_TEST);
    ofSetVerticalSync(true);
    ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
    
    cam.initGrabber(640, 480);
    tracker.setup();
    
    newVideo.allocate(640, 480, OF_PIXELS_RGB);
    newTexture.allocate(newVideo);
    
    videoTexture.allocate(newVideo);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    cam.update();
    if (cam.isFrameNew()) {
        tracker.update(toCv(cam));
        position = tracker.getPosition();
        scale = tracker.getScale();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    int width = 640;
    int height = 480;
    
    ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
    ofMesh faceMesh = tracker.getImageMesh();
    
    float time = ofGetElapsedTimef();
    
    for(int j=0; j<faceMesh.getNumVertices(); j++){
        auto extraZ = 200 * ( 0.8 - ofRandomuf() );
        extraZ = (cos(time) * j) * -3;
        if (extraZ < 0) {
            extraZ = extraZ * -1;
        }
        
        faceMesh.setVertex(j, faceMesh.getVertex(j) + ofVec3f( 0, 0, extraZ ));
    }
    
    // Create corner vertex
//    faceMesh.addVertex(ofVec3f(0,0,0));
//    faceMesh.addVertex(ofVec3f(ofGetWidth(),0,0));
//    faceMesh.addVertex(ofVec3f(0,ofGetHeight(),0));
//    faceMesh.addVertex(ofVec3f(ofGetWidth(),ofGetHeight(),0));
    
    if (cam.isFrameNew()) {
        ofSetPolyMode(OF_POLY_WINDING_NONZERO);
        
        newVideo = cam.getPixels();
//        int k = 0;
//
//        for (int i = 0; i < newVideo.size(); i++) {
//
//            int newPx = 0;
//            if (k == 0) {
//                // red
//                newPx = newVideo[i] * 0.9;
//            } else if ( k == 1 ) {
//                // green
//                newPx = newVideo[i] * 2;
//            } else if ( k == 2 ) {
//                // blue
//                newPx = 255;
//            }
//
//            if (newPx < 0 ) {
//                newPx = 0;
//            } else if (newPx > 255) {
//                newPx = 255;
//            }
//
//            newVideo[i] = newPx;
//
//            k++;
//            if (k == 3) {
//                k = 0;
//            }
//        }
        
        newTexture.loadData(newVideo);
    }
    
    videoTexture.loadData(newVideo);
    
//    ofPushMatrix();
//    ofTranslate(cos(time) * 0.10, cos(time) * 0.10, cos(time) * 0.10);
    
    videoTexture.bind();
    faceMesh.draw();
    videoTexture.unbind();
    
//    ofPopMatrix();
    
    ofTranslate(0,0,0);
    cam.draw(0,0);
    
    if (tracker.getFound()) {
//        ofPushMatrix();
//        ofSetLineWidth(1);
//        tracker.draw();
//
//        // DRAW OVER MESH
//        ofMesh mesh;
//        mesh = tracker.getObjectMesh();
//
//        ofPushMatrix();
//        texture.begin();
//        ofSetColor(255);
//        ofSetupScreenOrtho(640, 480, -1000, 1000);
//        ofTranslate(position.x, position.y);
//
//        ofScale(scale, scale, scale);
//        for (int i = 0; i < 100; i++) {
//            ofSetColor(0, 0, 255);
//            mesh.addVertex(ofVec3f(position.x, position.y + (float)i, 1.0));
//        }
//        mesh.draw(OF_MESH_WIREFRAME);
//        texture.end();
//        ofPopMatrix();
//
//        // FaceTracker plugin MESH draw
//        ofPushMatrix();
//        applyMatrix(rotationMatrix);
//        ofScale(scale, scale, scale);
//        mesh.draw(OF_MESH_FILL);
//        ofPopMatrix();
//        // FaceTracker plugin MESH draw
//
//        ofPopMatrix();
        
    } else {
        ofDrawBitmapString("searching for face...", 10, 20);
    }
}

void ofApp::bindMyTexture() {
    ofTexture &tex = texture.getTextureReference();
    tex.bind();
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glLoadIdentity();
    ofTextureData texData = tex.getTextureData();
    if(texData.textureTarget == GL_TEXTURE_RECTANGLE_ARB) {
        glScalef(tex.getWidth(), tex.getHeight(), 1.0f);
    } else {
        glScalef(tex.getWidth() / texData.tex_w, tex.getHeight() / texData.tex_h, 1.0f);
    }
    glMatrixMode(GL_MODELVIEW);
}
void ofApp::unbindMyTexture() {
    texture.getTextureReference().unbind();
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'r') {
        tracker.reset();
    }
}

