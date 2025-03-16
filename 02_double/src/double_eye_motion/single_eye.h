// SplitFlapModule.h (header file)

#ifndef EYE_H
#define EYE_H
#include "Arduino.h" 
#include "ServoEasing.h"

class Eye {
  public:
    // Constructor declaration
    Eye(int leftRight, int upDown, int eyeLid); 

    void setLeftRightLowerUpperCentre(int lower, int upper, int centre);
    void setUpDownLowerUpperCentre(int lower, int upper, int centre);
    void setEyeLidOpenClose(int open, int close);

    void init();
    void home();

    void eyeMotion();

    int leftRightLower;
    int leftRightUpper;
    int leftRightCentre;

    int upDownLower;
    int upDownUpper;
    int upDownCentre;

    int eyeLidOpen;
    int eyeLidClose;
    
    ServoEasing eyeLidServo; 
    ServoEasing upDownServo;
    ServoEasing leftRightServo;

  private:

    void moveServosTo(int leftRightServoPos, int upDownServoPos, int eyeLidServoPos,int speed);
    void blink(int closeSpeed, int openSpeed, int closeDelay, int openPosition);
    void moveEyeBall(int speed, int posLR, int posUD);
    int getRndEyePos(float lowerLimit, float upperLimit, float centrePos, float centreFactor);

    static constexpr int eyeOpenShort = 300;  //quick blink time
    static constexpr int eyeOpenMin = 600;
    static constexpr int eyeOpenMax = 2000;
    static constexpr int eyeStillMin = 400;
    static constexpr int eyeStillMax = 1200;

    unsigned long currTime;
    unsigned long lastBlinkTime;
    unsigned long lastMoveTime;

    int eyeOpenDuration;  //random time to keep eye open between blinks
    int eyeStillDuration;  //random time to keep eye still before moving


    int leftRightPin;
    int upDownPin;
    int eyeLidPin;


};

#endif
//          __
// (QUACK)>(o )___
//          ( ._> /
//           `---'
// Morgan Manly
// 12/03/2025

