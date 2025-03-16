#include "double_eye.h"

Eyes::Eyes(int left_leftRightPin, int left_upDownPin, int left_eyeLidPin,int right_leftRightPin, int right_upDownPin, int right_eyeLidPin)
    : leftEye(left_leftRightPin, left_upDownPin, left_eyeLidPin), 
      rightEye(right_leftRightPin, right_upDownPin, right_eyeLidPin) {


}

void Eyes::init() {
  leftEye.init();
  rightEye.init();

  currTime = millis();
  eyeOpenDuration = random(eyeOpenMin, eyeOpenMax);  //random time to keep eye open between blinks
  lastBlinkTime = currTime + eyeOpenDuration; //do this first
  eyeStillDuration = random(eyeStillMin, eyeStillMax);  //random time to keep eye still before moving
  lastMoveTime = currTime;
}

void Eyes::home() {
  int speed = 60;

  leftEye.leftRightServo.setEaseTo(leftEye.leftRightCentre,speed);
  leftEye.upDownServo.setEaseTo(leftEye.upDownCentre,speed);
  leftEye.eyeLidServo.setEaseTo(leftEye.eyeLidClose,speed);
  
  rightEye.leftRightServo.setEaseTo(rightEye.leftRightCentre,speed);
  rightEye.upDownServo.setEaseTo(rightEye.upDownCentre,speed);
  rightEye.eyeLidServo.setEaseTo(rightEye.eyeLidClose,speed);

  synchronizeAllServosAndStartInterrupt(false);      // Do not start interrupt, because we use updateAllServos() every 20 ms below
    do {
        delay(20); 
    } while (!updateAllServos());

  leftEye.eyeLidServo.setEaseTo(leftEye.eyeLidOpen,speed);
  rightEye.eyeLidServo.setEaseTo(rightEye.eyeLidOpen,speed);

  synchronizeAllServosAndStartInterrupt(false);      // Do not start interrupt, because we use updateAllServos() every 20 ms below
    do {
        delay(20); 
    } while (!updateAllServos());
}


void Eyes::eyeMotion(float depth) {

  currTime = millis();
  if ((currTime - lastBlinkTime) > eyeOpenDuration) {  //blink event
    blink(random(800, 1000), random(900, 1000), 0);
    if (!random(0, 3)) {  //sometimes don't move eye when blinking
      // moveEyeBall(random(400, 600), getRndEyePos(leftRightLower, leftRightUpper, leftRightCentre, 0.4), getRndEyePos(upDownLower, upDownUpper, upDownCentre, 0.4));
      float* point = generateRandomPoint(depth,0.4);
      lookAt(point[0],point[1],point[2],random(400, 600));
      lastMoveTime = millis();
    }
    if (!random(0, 5)) {                                           //1 in x chance
      eyeOpenDuration = random(eyeOpenShort, eyeOpenShort + 100);  //short blink (double)
    } else {
      eyeOpenDuration = random(eyeOpenMin, eyeOpenMax);  //next random duration for blinking
    }
    lastBlinkTime = millis();
  }

  currTime = millis();
  if ((currTime - lastMoveTime) > eyeStillDuration) {  //move eye event
    // moveEyeBall(random(100, 200), getRndEyePos(leftRightLower, leftRightUpper, leftRightCentre, 1), getRndEyePos(upDownLower, upDownUpper, upDownCentre, 1));
    float* point = generateRandomPoint(depth,0.4);
    lookAt(point[0],point[1],point[2],random(100, 200));
    
    // sometimes keep eye still for a long time, but most times much shorter
    if (!random(0, 5)) {                                           //1 in x chance
      eyeStillDuration = random(eyeStillMin, eyeStillMin+200); 
    } else {
      eyeStillDuration = random(eyeStillMin, eyeStillMax); 
    }
    
    lastMoveTime = millis();
  }
}

void Eyes::blink(int closeSpeed, int openSpeed, int closeDelay) {
  leftEye.eyeLidServo.setEaseTo(leftEye.eyeLidClose, closeSpeed);
  rightEye.eyeLidServo.setEaseTo(rightEye.eyeLidClose, closeSpeed);

  synchronizeAllServosAndStartInterrupt(false);      // Do not start interrupt, because we use updateAllServos() every 20 ms below
  do {
    delay(20); // Optional 20ms delay. Can be less.
  } while (!updateAllServos());

  delay(25); //allow eyes to close fully

  leftEye.eyeLidServo.setEaseTo(leftEye.eyeLidOpen, openSpeed);
  rightEye.eyeLidServo.setEaseTo(rightEye.eyeLidOpen, openSpeed);

  synchronizeAllServosAndStartInterrupt(false);      // Do not start interrupt, because we use updateAllServos() every 20 ms below
  do {
    delay(20); // Optional 20ms delay. Can be less.
  } while (!updateAllServos());
}

void Eyes::lookAt(float posX,float posY, float posZ,int speed) {
  // Step 1, Calculate Angle For Each Eye in LR and UD

  //Left Eye

  float left_angleLR = 90 - (57.2958* atan2(posY, posX-(eyeDistance/2)));   //converted to degs    
  float right_angleLR = 90 - (57.2958* atan2(posY, posX+(eyeDistance/2)));   //converted to degs   

  float left_angleUD = 90 - (57.2958* atan2((cos(left_angleLR/57.2958) * posY),posZ));   //converted to degs
  float right_angleUD = 90 - (57.2958* atan2((cos(right_angleLR/57.2958) * posY),posZ));   //converted to degs

  // Serial.print("Pos X: ");
  // Serial.print(posX);
  // Serial.print(" Pos Y: ");
  // Serial.print(posY);
  // Serial.print(" Pos Z: ");
  // Serial.println(posZ);
  // Serial.print(" left_angleUD: ");
  // Serial.print(left_angleUD);

  //Step 2, Convert Angle to Servo Positions
  float LR_coeffs[4] = {0.0000011343,0.0039990389,0.9812176147,0.1148424022};
  float UP_coeffs[4] = {0.0000408673,0.0021149326,1.2047511527,0.6274157224};


  float left_LR = leftEye.leftRightCentre + getInverseKin(left_angleLR,LR_coeffs,3);
  float right_LR = rightEye.leftRightCentre + getInverseKin(right_angleLR,LR_coeffs,3);

  float left_UD = leftEye.upDownCentre + getInverseKin(left_angleUD,UP_coeffs,3);
  float right_UD = rightEye.upDownCentre - getInverseKin(right_angleUD,UP_coeffs,3); //reversed because motor moves other way

  // Serial.print(" left_UD unconstrained: ");
  // Serial.println(left_UD);

  left_LR = constrain(left_LR,leftEye.leftRightLower,leftEye.leftRightUpper);
  right_LR = constrain(right_LR,rightEye.leftRightLower,rightEye.leftRightUpper);
  
  left_UD = constrain(left_UD,leftEye.upDownLower,leftEye.upDownUpper);
  right_UD = constrain(right_UD,rightEye.upDownLower,rightEye.upDownUpper);

  // Serial.print(" right_LR constrained: ");
  // Serial.println(right_LR);

  leftEye.leftRightServo.setEaseTo(left_LR, speed);
  rightEye.leftRightServo.setEaseTo(right_LR, speed);

  leftEye.upDownServo.setEaseTo(left_UD, speed);
  rightEye.upDownServo.setEaseTo(right_UD, speed);

  synchronizeAllServosAndStartInterrupt(false);      // Do not start interrupt, because we use updateAllServos() every 20 ms below
  do {
    delay(20); // Optional 20ms delay. Can be less.
  } while (!updateAllServos());

}

float Eyes::getInverseKin(float target,float coeffs[],int degree) {
  float result = 0.0;

  // Compute the polynomial function
  for (int i = 0; i <= degree; i++) {
      result += coeffs[i] * pow(target, degree-i);
  }

  return result;
}

float* Eyes::generateRandomPoint(float depth,float centreFactor) {
  //generate random position in workspace given a depth (this value will be randomly generated most likely)
  static float point[3];

  //Define max and min for left and right, using max eye angles

  float LR_maxAngle = 30; 
  float UD_maxAngle = 35;

  float X_min = centreFactor* (- eyeDistance/2 - (tan(LR_maxAngle/57.2958) * depth));
  float X_max =  centreFactor* (eyeDistance/2 + (tan(LR_maxAngle/57.2958) * depth));

  float Z_min = centreFactor* (-(tan(UD_maxAngle/57.2958) * depth));
  float Z_max = centreFactor* (tan(UD_maxAngle/57.2958) * depth);

  point[0] = random(X_min, X_max);
  point[1] = depth;
  point[2] = random(Z_min, Z_max);

  return point;
}