#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetDataPathRoot("../Resources/data/");
    arduino.listDevices();
    
    // Auto Connect the Arduino
    vector <ofSerialDeviceInfo> deviceList = arduino.getDeviceList();
    
    for (int i = 0; i < deviceList.size(); i++) {
        if (ofIsStringInString(deviceList[i].getDeviceName(), "tty.usbmodem")) {
            string _arduinoName = "/dev/"+deviceList[i].getDeviceName();
            arduino.setup(_arduinoName,9600);
        }
    }
    
    // Load the Audio
    starOne.load("star_one.wav");
    starTwo.load("star_two.wav");
    starOne.setLoop(true);
    starTwo.setLoop(true);
    starOne.play();
    starTwo.play();
}

//--------------------------------------------------------------
void ofApp::update()
{
    readSensorData();
    
    // Update the Audio Volume
    starOne.setVolume(ofMap(ofClamp(smoothedDistance1,0,200),0,200,1.0,0.0));
    starTwo.setVolume(ofMap(ofClamp(smoothedDistance2,0,200),0,200,1.0,0.0));
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(0, 0, 0);
    
    // Reset the Distance
    smoothedDistance1 = 0;
    smoothedDistance2 = 0;
    
    // Calculate the Average Distances
    for (int i = 0; i < smoothedDistance[0].size(); i++) {
        smoothedDistance1 += smoothedDistance[0][i];
    }
    
    for (int i = 0; i < smoothedDistance[1].size(); i++) {
        smoothedDistance2 += smoothedDistance[1][i];
    }
    
    smoothedDistance1 /= 20;
    smoothedDistance2 /= 20;
    
    // Draw out the data
    stringstream ss;
    ss << "---------------------------" << endl;
    ss << "Raw Distance from Sensor 1 " << sensorDistance[0] << endl;
    ss << "Raw Distance from Sensor 2 " << sensorDistance[1] << endl;
    ss << endl;
    ss << "---------------------------" << endl;
    ss << "Raw Audio Swell Level 1 " << ofMap(ofClamp(sensorDistance[0],0,200),0,200,1.0,0.0) << endl;
    ss << "Raw Audio Swell Level 2 " << ofMap(ofClamp(sensorDistance[1],0,200),0,200,1.0,0.0) << endl;
    ss << endl;
    ss << "---------------------------" << endl;
    ss << "Smoothed Distance from Sensor 1 " << smoothedDistance1 << endl;
    ss << "Smoothed Distance from Sensor 2 " << smoothedDistance2 << endl;
    ss << endl;
    ss << "---------------------------" << endl;
    ss << "Smoothed Audio Swell Level 1 " << ofMap(ofClamp(smoothedDistance1,0,200),0,200,1.0,0.0) << endl;
    ss << "Smoothed Audio Swell Level 2 " << ofMap(ofClamp(smoothedDistance2,0,200),0,200,1.0,0.0) << endl;
    
    ofDrawBitmapString(ss.str(), 15, 15);
    drawPlinth();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if(key == OF_KEY_TAB) {
        ofSystem("open ./data/");
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void ofApp::drawPlinth()
{
    int size = 100;
    ofPushStyle();
    ofSetColor(ofColor::white);
    ofNoFill();
    ofDrawRectangle(ofGetWidth()/2 - (size / 2) , ofGetHeight()/2  - (size / 2), size, size);
    
    ofSetColor(ofColor::red);
    ofDrawCircle(ofGetWidth()/2, ofGetHeight()/2, (size/2 + 25) + smoothedDistance1 / 2);
    
    ofSetColor(ofColor::orange);
    ofDrawCircle(ofGetWidth()/2, ofGetHeight()/2, (size/2 + 25) + smoothedDistance2 / 2);
    
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::readSensorData()
{
    // If we have more than 25 entries remove the oldest
    for (int i = 0; i < 2; i++) {
        if(smoothedDistance[i].size() > 25) {
            smoothedDistance[i].pop_back();
        }
    }
    
    // While the Arduino is sending data
    if (arduino.available() > 0)
    {
        while (arduino.available() > 0)
        {
            // Read One byte at a time
            arduino.readBytes(bytesReturned, 1);
            
            // If its a newline
            if (*bytesReturned == '\n')
            {
                cpmessage = messageReceived;
                // Split the string via the comma
                vector <string> s = ofSplitString(messageReceived,",");
                
                if(s.size() > 1) {
                    sensorDistance[0] = ofToInt(s[0]);
                    sensorDistance[1] = ofToInt(s[1]);
                    smoothedDistance[0].push_front(ofToInt(s[0]));
                    smoothedDistance[1].push_front(ofToInt(s[1]));
                }
                messageReceived = "";
                break;
            }
            else
            {
                if(*bytesReturned != '\n')
                {
                    messageReceived += *bytesReturned;
                }
            }
        }
        memset(bytesReturned, 0, 1);
    }
}
