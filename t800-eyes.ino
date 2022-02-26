/**
 * @file t800-eyes.ino
 * @author John Scott
 * @brief Terminator T800 Flickering & Fading Eyes.
 * @version 1.0
 * @date 2022-01-10
 * 
 * @copyright Copyright (c) 2022 John Scott
 * 
 */
#include <Servo.h>        // Tools -> Manage Libraries...
#include <ServoEasing.h>  // ServoEasing v2.3.4
#include <AsyncTimer.h>   // AsyncTimer v2.2.0
#include <jled.h>         // JLed v4.8.0


//
// Eye LED settings...
//
#define LED_A 5
#define LED_B 6
#define LED_C 7

#define FADE_ON 125
#define FADE_ON_WAIT 500

#define FLICKER_TIME 2500

#define EYES_ON_WAIT 500

#define FADE_OFF 1500
#define FADE_OFF_WAIT 2500


//
// Eye Servo settings...
//
#define EYE_SERVO_A 9
#define EYE_SERVO_B 10
#define EYE_SERVO_C 11

#define EYE_SPEED 150

#define EYE_MIN 20
#define EYE_MAX 160
#define EYE_CENTER 90

#define EYE_INTERVAL 900


//
// Random eye movement...
//
AsyncTimer t;
ServoEasing eyeServoA;
ServoEasing eyeServoB;
ServoEasing eyeServoC;
unsigned short eyeTimer;
unsigned short scanTimer;
bool eyeMoveA = true;
bool eyeMoveB = true;
bool eyeMoveC = true;


//
// Eye flickering & fading movement.
//
JLed ledsA[] = {
    JLed(LED_A).FadeOn(FADE_ON).DelayAfter(FADE_ON_WAIT * 2),
    JLed(LED_A).Candle(6, 100, FLICKER_TIME * 2),
    JLed(LED_A).On().DelayAfter(EYES_ON_WAIT * 2),
//    JLed(LED_A).FadeOff(FADE_OFF).DelayAfter(FADE_OFF_WAIT * 2)
};
auto seqA = JLedSequence(JLedSequence::eMode::SEQUENCE, ledsA);
auto ledA = JLed(LED_A).FadeOff(FADE_OFF).DelayAfter(FADE_OFF_WAIT * 2);

JLed ledsB[] = {
    JLed(LED_B).FadeOn(FADE_ON).DelayAfter(FADE_ON_WAIT * 2),
    JLed(LED_B).Candle(6, 100, FLICKER_TIME * 2),
    JLed(LED_B).On().DelayAfter(EYES_ON_WAIT * 2),
//    JLed(LED_B).FadeOff(FADE_OFF).DelayAfter(FADE_OFF_WAIT * 2)
};
auto seqB = JLedSequence(JLedSequence::eMode::SEQUENCE, ledsB);
auto ledB = JLed(LED_B).FadeOff(FADE_OFF).DelayAfter(FADE_OFF_WAIT * 2);


//
// Generate variation for range -> range * 2
//
int vary(int range, int v = 11) {
  return range + random(v) * range / v;
}


//
// Change JLed timings so each flicker is independent.
//
void retime(JLed (&leds)[3], JLed &led) {
  leds[0].FadeOn(FADE_ON).DelayAfter(vary(FADE_ON_WAIT));
  leds[1].Candle(6,100,vary(FLICKER_TIME));
  leds[2].On().DelayAfter(vary(EYES_ON_WAIT));
  led.FadeOff(FADE_OFF).DelayAfter(vary(FADE_OFF_WAIT));
}


//
// Random eye movement.
//
void moveEyes() {
  if(eyeMoveA) {
    eyeServoA.startEaseTo(random(EYE_MIN, EYE_MAX+1));
  }

  if(eyeMoveB) {
    eyeServoB.startEaseTo(random(EYE_MIN, EYE_MAX+1));
  }
}


//
// Scan eyes back and forth.
//
void scanEyes() {
  eyeServoC.startEaseTo(eyeMoveC?EYE_MIN:EYE_MAX);
  eyeMoveC = !eyeMoveC;
}


//
// Standard setup function.
//
void setup() {
  Serial.begin(9600); // For debugging.
  randomSeed(analogRead(0)); // Init random number generator.

  eyeServoA.attach(EYE_SERVO_A); // Init eyes.
  eyeServoA.write(EYE_CENTER);
  eyeServoA.setSpeed(EYE_SPEED);
  
  eyeServoB.attach(EYE_SERVO_B);
  eyeServoB.write(EYE_CENTER);
  eyeServoB.setSpeed(EYE_SPEED);

  eyeServoC.attach(EYE_SERVO_C);
  eyeServoC.write(EYE_CENTER);
  eyeServoC.setSpeed(EYE_SPEED / 2); // Run slower is scanning mode.

  pinMode(LED_C, OUTPUT); // Make Led C constantly on (for scanning mode). 
  digitalWrite(LED_C, HIGH);

  t.setup();
  eyeTimer = t.setInterval(moveEyes, EYE_INTERVAL);
  scanTimer= t.setInterval(scanEyes, EYE_INTERVAL * 5);
}


//
// Standard loop.
//
void loop() {
  t.handle();
  
  if(!seqA.Update()) {
    eyeMoveA = false; // Stop random eye movement when sequence ends and then we fade off.
    if(!ledA.Update()) {
      retime(ledsA, ledA);
      seqA.Reset();
      ledA.Reset();
      eyeMoveA = true; // Restart eye movement for next eye flickering sequence.
    }
  }

  if(!seqB.Update()) {
    eyeMoveB = false; // Stop random eye movement when sequence ends and then we fade off.
    if(!ledB.Update()) {
      retime(ledsB, ledB);
      seqB.Reset();
      ledB.Reset();
      eyeMoveB = true; // Restart eye movement for next eye flickering sequence.
    }
  }
}
