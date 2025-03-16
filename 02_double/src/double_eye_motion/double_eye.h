// Double eye header file
#ifndef D_EYE_H
#define D_EYE_H

#include <Arduino.h>
#include "single_eye.h"

class Eyes {
  public:
    Eyes(int left_leftRightPin, int left_upDownPin, int left_eyeLidPin,int right_leftRightPin, int right_upDownPin, int right_eyeLidPin); 

    Eye leftEye;
    Eye rightEye;

    void init();
    void home();
    void eyeMotion(float depth=300);
    void lookAt(float posX,float posY, float posZ,int speed=60);

  private:
    float getInverseKin(float target,float coeffs[],int degree);
    void blink(int closeSpeed, int openSpeed, int closeDelay);

    float* generateRandomPoint(float depth,float centreFactor);

    static constexpr int eyeOpenShort = 300;  //quick blink time
    static constexpr int eyeOpenMin = 600;
    static constexpr int eyeOpenMax = 2000;
    static constexpr int eyeStillMin = 400;
    static constexpr int eyeStillMax = 1000;

    unsigned long currTime;
    unsigned long lastBlinkTime;
    unsigned long lastMoveTime;

    int eyeOpenDuration;  //random time to keep eyes open between blinks
    int eyeStillDuration;  //random time to keep eyes still before moving


    static constexpr float eyeDistance = 65.8; //distance between eyeball centres

};

#endif
//          __
// (QUACK)>(o )___
//          ( ._> /
//           `---'
// Morgan Manly
// 16/03/2025
