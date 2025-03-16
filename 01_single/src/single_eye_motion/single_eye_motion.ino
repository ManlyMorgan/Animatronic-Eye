#include "Arduino.h" 
#include "single_eye.h"

//Servo Connection Pins, LeftRightServo - 3, UpDownServo - 5, OpenCloseServo - 6
Eye eye(3,5,6);

void setup() {
  Serial.begin(9600);

  // ENTER YOUR VALUES FOR UPPER/LOWER/CENTRE HERE, USE CALIBRATION.INO To Find the Values
  // My values below, for reference only
  // eye.setLeftRightLowerUpperCentre(60,115,90);
  // eye.setUpDownLowerUpperCentre(45,120,85);
  // eye.setEyeLidOpenClose(75,145);
  
  eye.setLeftRightLowerUpperCentre(x,y,z);
  eye.setUpDownLowerUpperCentre(a,b,c);
  eye.setEyeLidOpenClose(i,j);

  eye.init(); //setup eye functions

  eye.home(); //Centre eye and blink

  Serial.println("Setup Complete");
}

void loop() {
  
  eye.eyeMotion(); //keep this running repeatedly to move the
  someFunction();

}


void someFunction() {
  //Put whatever you want here (non blocking)
}