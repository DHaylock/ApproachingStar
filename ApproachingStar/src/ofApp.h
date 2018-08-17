#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
    
        ofSerial arduino;
    
        void readSensorData();
        unsigned char bytesReturned[1];
        string messageReceived;
        string cpmessage;

    
        int sensorDistance[2];
        deque <int> smoothedDistance[2];
        int smoothedDistance1;
        int smoothedDistance2;
    
        ofSoundPlayer starOne;
        ofSoundPlayer starTwo;
    
        void drawPlinth();
};
