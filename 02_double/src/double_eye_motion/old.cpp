// #include <Arduino.h>

// #include "ServoEasing.hpp"

// ServoEasing eyeLidServo;  // create servo object to control a servo
// ServoEasing upDownServo;
// ServoEasing leftRightServo;


// // Right RL Centre/Min/Max 55/90/118 (Pin 3)
// // Right UD Centre/Min/Max 45/90/120 (Pin 5)
// // Right Blink Close/Open 35/100 (Pin 6)

// // Left RL Centre/Min/Max 55/85/115 (Pin 9)
// // Left UD Centre/Min/Max 45/75/120 (Pin 10)
// // Left Blink Open/Close 80/145 (Pin 11)


// int leftRightPin = 3;
// int upDownPin = 5;
// int eyeLidPin = 6;

// // Servo Limits
// int eyeLidServoClose = 35;
// int eyeLidServoOpen = 100;

// int upDownServoLower = 45;
// int upDownServoCentre = 90;
// int upDownServoUpper = 120;

// int leftRightServoLower = 55;
// int leftRightServoCentre = 90;
// int leftRightServoUpper = 118;

// void setup() {

//   Serial.begin(115200);

//   eyeLidServo.attach(eyeLidPin);        // attaches the servo on pin 18 to the servo object
//   upDownServo.attach(upDownPin);        // attaches the servo on pin 18 to the servo object
//   leftRightServo.attach(leftRightPin);  // attaches the servo on pin 18 to the servo object
//                                         // different servos may require different min/max settings
//                                         // for an accurate 0 to 180 sweep

//   //home servos
//   eyeLidServo.setEaseTo(eyeLidServoClose);
//   upDownServo.setEaseTo(upDownServoCentre);
//   leftRightServo.setEaseTo(leftRightServoCentre);
    
//   synchronizeAllServosAndStartInterrupt(false);      // Do not start interrupt, because we use updateAllServos() every 20 ms below
//     do {
//         delay(20); 
//     } while (!updateAllServos());
    
//   delay(250);
//   eyeLidServo.easeTo(eyeLidServoOpen,60);
//   delay(10000000);

// }

// void loop() {
//   someFunction();
//   eyeMotion();
// }


// void someFunction() {

//   //Whatever Task You Want to Do Beside Moving the Eye, Leave Blank Otherwise
//   // vTaskDelay(20/portTICK_PERIOD_MS);
  
// }

// long currTime = millis();
// int eyeOpenShort = 300;  //quick blink time
// int eyeOpenMin = 600;
// int eyeOpenMax = 2000;
// int eyeOpenDuration = random(eyeOpenMin, eyeOpenMax);  //random time to keep eye open between blinks
// long lastBlinkTime = currTime + eyeOpenDuration; //do this first

// int eyeStillMin = 400;
// int eyeStillMax = 1200;
// int eyeStillDuration = random(eyeStillMin, eyeStillMax);  //random time to keep eye still before moving
// long lastMoveTime = currTime;
// // int nonSideEyeMin = 1000; //minimum time to not be in side eye, before potentially entering side eye
// // int nonSideEyeMax = 2000;
// // int nonSideEyeDuration = random(nonSideEyeMin,nonSideEyeMax);
// // long lastSideEyeTime = currTime;

// void eyeMotion() {

//   // while (1) {  //until exit condition, prehaps a set ammount of time, randomly defined before calling the function
//     currTime = millis();
//     if ((currTime - lastBlinkTime) > eyeOpenDuration) {  //blink event
//       blink(random(800, 1000), random(900, 1000), 0, eyeLidServoOpen);
//       if (!random(0, 2)) {  //sometimes don't move eye when blinking
//         moveEyeBall(random(400, 600), getRndEyePos(leftRightServoLower, leftRightServoUpper, leftRightServoCentre, 0.4), getRndEyePos(upDownServoLower, upDownServoUpper, upDownServoCentre, 0.4));
//         lastMoveTime = millis();
//       }
//       if (!random(0, 5)) {                                           //1 in x chance
//         eyeOpenDuration = random(eyeOpenShort, eyeOpenShort + 100);  //short blink (double)
//       } else {
//         eyeOpenDuration = random(eyeOpenMin, eyeOpenMax);  //next random duration for blinking
//       }
//       lastBlinkTime = millis();
//     }

//     currTime = millis();
//     if ((currTime - lastMoveTime) > eyeStillDuration) {  //move eye event
//       moveEyeBall(random(100, 200), getRndEyePos(leftRightServoLower, leftRightServoUpper, leftRightServoCentre, 1), getRndEyePos(upDownServoLower, upDownServoUpper, upDownServoCentre, 1));
//       lastMoveTime = millis();
//     }
//   // }
// }
// int getRndEyePos(float lowerLimit, float upperLimit, float centrePos, float centreFactor) {
//   float upperRange = abs(upperLimit - centrePos);
//   float lowerRange = abs(centrePos - lowerLimit);
//   // int centreFactor = 0.25;

//   // Serial.print("Lower Range for Random");
//   // Serial.println(centrePos-(lowerRange*centreFactor));

//   if (random(0, 3)) {  //x-1 in x chance
//     // Serial.println("Normal");
//     return random(centrePos - (lowerRange * centreFactor), centrePos + (upperRange * centreFactor));
//   } else {  //1 in x chance
//     // Serial.println("Rare");
//     return random(lowerLimit, upperLimit);
//   }
// }

// // Move eyeball to set position and set a single speed, both servos should finish moving at the same time for a realistic motion
// void moveEyeBall(int speed, int posLR, int posUD) {
//   posLR = constrain(posLR, leftRightServoLower, leftRightServoUpper);
//   posUD = constrain(posUD, upDownServoLower, upDownServoUpper);

//   leftRightServo.setEaseTo(posLR,speed);
//   upDownServo.setEaseTo(posUD,speed);
//   synchronizeAllServosAndStartInterrupt(false);      // Do not start interrupt, because we use updateAllServos() every 20 ms below
//   do {
//     delay(20); // Optional 20ms delay. Can be less.
//   } while (!updateAllServos());
// }

// void blink(int closeSpeed, int openSpeed, int closeDelay, int openPosition) {
//   eyeLidServo.setEaseTo(eyeLidServoClose, closeSpeed);
//   // setEaseToForAllServosSynchronizeAndStartInterrupt(closeSpeed);
//   synchronizeAllServosAndStartInterrupt(false);      // Do not start interrupt, because we use updateAllServos() every 20 ms below
//   do {
//     delay(20); // Optional 20ms delay. Can be less.
//   } while (!updateAllServos());
//   // vTaskDelay(closeDelay/portTICK_PERIOD_MS);
//   eyeLidServo.startEaseTo(openPosition, openSpeed);
// }
