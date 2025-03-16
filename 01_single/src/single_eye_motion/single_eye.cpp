#include "single_eye.h"
#include "ServoEasing.hpp"

Eye::Eye(int leftRight, int upDown, int eyeLid)  {
  leftRightPin = leftRight;
  upDownPin = upDown;
  eyeLidPin = eyeLid;
}

void Eye::init() {

  leftRightServo.attach(leftRightPin);
  upDownServo.attach(upDownPin);
  eyeLidServo.attach(eyeLidPin);

  currTime = millis();
  eyeOpenDuration = random(eyeOpenMin, eyeOpenMax);  //random time to keep eye open between blinks
  lastBlinkTime = currTime + eyeOpenDuration; //do this first
  eyeStillDuration = random(eyeStillMin, eyeStillMax);  //random time to keep eye still before moving
  lastMoveTime = currTime;

}


void Eye::setLeftRightLowerUpperCentre(int lower, int upper, int centre) {
  leftRightLower = lower;
  leftRightUpper = upper;
  leftRightCentre = centre;
}

void Eye::setUpDownLowerUpperCentre(int lower, int upper, int centre) {
  upDownLower = lower;
  upDownUpper = upper;
  upDownCentre = centre;
}

void Eye::setEyeLidOpenClose(int open, int close){
  eyeLidOpen = open;
  eyeLidClose = close;
}

void Eye::moveServosTo(int leftRightServoPos, int upDownServoPos, int eyeLidServoPos,int speed) {

  leftRightServoPos = constrain(leftRightServoPos,leftRightLower,leftRightUpper);
  upDownServoPos = constrain(upDownServoPos,upDownLower,upDownUpper);
  eyeLidServoPos = constrain(eyeLidServoPos,eyeLidClose,eyeLidOpen);

  leftRightServo.setEaseTo(leftRightServoPos,speed);
  upDownServo.setEaseTo(upDownServoPos,speed);
  eyeLidServo.setEaseTo(eyeLidServoPos,speed);
  
  synchronizeAllServosAndStartInterrupt(false);      // Do not start interrupt, because we use updateAllServos() every 20 ms below
    do {
        delay(20); 
    } while (!updateAllServos());
}

void Eye::home() {

  //home servos, move to central position with eyelid open
  moveServosTo(leftRightCentre,upDownCentre,eyeLidClose,60);
  delay(250);
  eyeLidServo.easeTo(eyeLidOpen,60);

}

void Eye::eyeMotion() {
  // while (1) {  //until exit condition, prehaps a set ammount of time, randomly defined before calling the function
    currTime = millis();
    if ((currTime - lastBlinkTime) > eyeOpenDuration) {  //blink event
      blink(random(800, 1000), random(900, 1000), 0, eyeLidOpen);
      if (!random(0, 2)) {  //sometimes don't move eye when blinking
        moveEyeBall(random(400, 600), getRndEyePos(leftRightLower, leftRightUpper, leftRightCentre, 0.4), getRndEyePos(upDownLower, upDownUpper, upDownCentre, 0.4));
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
      moveEyeBall(random(100, 200), getRndEyePos(leftRightLower, leftRightUpper, leftRightCentre, 1), getRndEyePos(upDownLower, upDownUpper, upDownCentre, 1));
      lastMoveTime = millis();
    }
  // }
}

int Eye::getRndEyePos(float lowerLimit, float upperLimit, float centrePos, float centreFactor) {
  float upperRange = abs(upperLimit - centrePos);
  float lowerRange = abs(centrePos - lowerLimit);
  // int centreFactor = 0.25;

  // Serial.print("Lower Range for Random");
  // Serial.println(centrePos-(lowerRange*centreFactor));

  if (random(0, 3)) {  //x-1 in x chance
    // Serial.println("Normal");
    return random(centrePos - (lowerRange * centreFactor), centrePos + (upperRange * centreFactor));
  } else {  //1 in x chance
    // Serial.println("Rare");
    return random(lowerLimit, upperLimit);
  }
}

// Move eyeball to set position and set a single speed, both servos should finish moving at the same time for a realistic motion
void Eye::moveEyeBall(int speed, int posLR, int posUD) {
  posLR = constrain(posLR, leftRightLower, leftRightUpper);
  posUD = constrain(posUD, upDownLower, upDownUpper);

  leftRightServo.setEaseTo(posLR,speed);
  upDownServo.setEaseTo(posUD,speed);
  synchronizeAllServosAndStartInterrupt(false);      // Do not start interrupt, because we use updateAllServos() every 20 ms below
  do {
    delay(20); // Optional 20ms delay. Can be less.
  } while (!updateAllServos());
}

void Eye::blink(int closeSpeed, int openSpeed, int closeDelay, int openPosition) {
  eyeLidServo.setEaseTo(eyeLidClose, closeSpeed);

  synchronizeAllServosAndStartInterrupt(false);      // Do not start interrupt, because we use updateAllServos() every 20 ms below
  do {
    delay(20); // Optional 20ms delay. Can be less.
  } while (!updateAllServos());
  delay(25);
  eyeLidServo.startEaseTo(openPosition, openSpeed);
}
