#include "Arduino.h" 
#include "double_eye.h"

//Servo Connection Pins, Left Eye, then Right Eye (Left Right, Up Down, Open Close) 
// The order of left and right is important, and it it taken from the robots perspective (use the linkage labels for reference)
Eyes Eyes(10,9,3,
          14,6,5);

void setup() {
  Serial.begin(9600);
  
  // ENTER YOUR VALUES FOR UPPER/LOWER/CENTRE HERE, USE CALIBRATION.INO To Find the Values
  Eyes.rightEye.setLeftRightLowerUpperCentre(60,115,90);
  Eyes.rightEye.setUpDownLowerUpperCentre(45,120,85);
  Eyes.rightEye.setEyeLidOpenClose(100,35);

  Eyes.leftEye.setLeftRightLowerUpperCentre(60,120,85);
  Eyes.leftEye.setUpDownLowerUpperCentre(35,110,75);
  Eyes.leftEye.setEyeLidOpenClose(75,145);

  //Setup Eyes
  Eyes.init(); 

  // Centre Eyes
  Eyes.home();
}

void loop() {
  Eyes.eyeMotion(random(200,500));
  someFunction();
}

void someFunction() {
  //Put whatever you want here (non blocking)
}

