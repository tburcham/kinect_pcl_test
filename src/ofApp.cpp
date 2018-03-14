#include "ofApp.h"

//NOTE: if you are unable to connect to your device on OS X, try unplugging and replugging in the power, while leaving the USB connected.
//ofxKinectV2 will only work if the NUI Sensor shows up in the Superspeed category of the System Profiler in the USB section.

//On OS X if you are not using the example project. Make sure to add OpenCL.framework to the Link Binary With Library Build Phase
//and change the line in Project.xcconfig to OTHER_LDFLAGS = $(OF_CORE_LIBS) $(OF_CORE_FRAMEWORKS) -framework OpenCL

//--------------------------------------------------------------
void ofApp::setup(){
    
    //Uncomment for verbose info from libfreenect2
    //ofSetLogLevel(OF_LOG_VERBOSE);
    
    ofBackground(30, 30, 30);
    
    //see how many devices we have.
    ofxKinectV2 tmp;
    vector <ofxKinectV2::KinectDeviceInfo> deviceList = tmp.getDeviceList();
    
    //allocate for this many devices
    kinects.resize(deviceList.size());
    texDepth.resize(kinects.size());
    texRGB.resize(kinects.size());
    meshes.resize(kinects.size());
    pointCloud.resize(kinects.size());
    
    panel.setup("", "settings.xml", 10, 100);
    
    //Note you don't have to use ofxKinectV2 as a shared pointer, but if you want to have it in a vector ( ie: for multuple ) it needs to be.
    for(int d = 0; d < kinects.size(); d++){
        kinects[d] = shared_ptr <ofxKinectV2> (new ofxKinectV2());
        kinects[d]->open(deviceList[d].serial);
        panel.add(kinects[d]->params);
        
        meshes[d].setMode(OF_PRIMITIVE_LINES_ADJACENCY);
        meshes[d].enableColors();
        meshes[d].enableIndices();
    }
    
    panel.add(pointCloudSteps.setup("pointCloudSteps", 25, 1, 500));
    panel.add(pointSize.setup("pointSize", 1, 1, 10));
    panel.add(connectionDistance.setup("connectionDistance", 20, 1, 500));
    
    panel.loadFromFile("settings.xml");
    
    ofEnableDepthTest();
    glEnable(GL_POINT_SMOOTH); // use circular points instead of square points
    
    cam.setFov(60);
    cam.setNearClip(1);
    cam.setFarClip(20000);
    
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    glPointSize(pointSize);
    
    for(int d = 0; d < kinects.size(); d++){
        kinects[d]->update();
        if( kinects[d]->isFrameNew() ){
            texDepth[d].loadData( kinects[d]->getDepthPixels() );
            texRGB[d].loadData( kinects[d]->getRgbPixels() );
            
            //pointCloud.push_back(kinects[d]->getWorldCoordinates());
            
            pointCloud = kinects[d]->getWorldCoordinates();
            
            meshes[d].clear();
            
            vector<ofPoint> pc = reducePointCloud(pointCloud, pointCloudSteps);
            
            meshes[d].addVertices(pc);
            
            // Let's add some lines!
           // float connectionDistance = 50;
            int numVerts = meshes[d].getNumVertices();
            for (int a=0; a<numVerts; ++a) {
                ofVec3f verta = meshes[d].getVertex(a);
                for (int b=a+1; b<numVerts; ++b) {
                    ofVec3f vertb = meshes[d].getVertex(b);
                    float distance = verta.distance(vertb);
                    if (distance <= connectionDistance) {
                        // In OF_PRIMITIVE_LINES, every pair of vertices or indices will be
                        // connected to form a line
                        meshes[d].addIndex(a);
                        meshes[d].addIndex(b);
                    }
                }
            }
            
            cout << meshes[d].getNumVertices();
            cout << "\n";
        }
    }
    

}

//--------------------------------------------------------------
void ofApp::draw(){
    //ofDrawBitmapString("ofxKinectV2: Work in progress addon.\nBased on the excellent work by the OpenKinect libfreenect2 team\n\n-Requires USB 3.0 port ( superspeed )\n-Requires patched libusb. If you have the libusb from ofxKinect ( v1 ) linked to your project it will prevent superspeed on Kinect V2", 10, 14);
    
    cam.begin();
    
    for(int d = 0; d < kinects.size(); d++){
        float dwHD = 1920/4;
        float dhHD = 1080/4;
        
        float shiftY = 100 + ((10 + texDepth[d].getHeight()) * d);
        
        //texDepth[d].draw(200, shiftY);
        //texRGB[d].draw(210 + texDepth[d].getWidth(), shiftY, dwHD, dhHD);
        
        ofSetColor(255, 255, 255, 128);
        
        //cout << meshes[d].getNumVertices();
        //cout << "\n";
        
        
        ofPushMatrix();

    
        ofScale(1, -1, -1);
        ofTranslate(dwHD, dhHD, -1000); // center the points a bit
        ofEnableDepthTest();
        //mesh.drawVertices();
        meshes[d].draw();

        ofDisableDepthTest();
        ofPopMatrix();
        
        
        //kinect.getColorSource()->getTexture().bind()
        //texDepth[d].bind();
        //mesh.draw();
        //texDepth[d].unbind();
        //kinect.getColorSource()->getTexture().unbind()
        
        //pointCloud[d].
    }
    
    
    
    cam.end();
    
    panel.draw();
}




vector<ofPoint> ofApp::reducePointCloud(vector<ofPoint> pc, int steps){
    
    vector<ofPoint> reducedPc;
    
    for (int i = 0; i < pc.size(); i+=steps) {
        
        reducedPc.push_back(pc[i]);
        
    }
    
    return reducedPc;
    
    
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
