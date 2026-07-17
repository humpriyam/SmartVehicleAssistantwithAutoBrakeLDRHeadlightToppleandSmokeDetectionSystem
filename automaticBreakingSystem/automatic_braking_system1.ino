#include <Servo.h> 

const int trigPin1= 2;
const int echoPin1 =3;
const int trigPin2 = 4;
 const int echoPin2=5;
const int buttonPin = 6;
const int servoPin =9;
const int buzzerPin = 10;

Servo brakeServo; 

unsigned long lastDistancePrint=0; 
unsigned long lastTimerTick = 0;     
  unsigned long gracePeriodStart=0;  

bool isBraking =false;
bool inGracePeriod = false;
int countdownRemaining = 60;

void setup() {
  Serial.begin(9600);

  pinMode(trigPin1,OUTPUT);
   pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

 pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  brakeServo.attach(servoPin);
    brakeServo.write(0); 
}

void loop() {
  
  long distance1 = getDistance(trigPin1, echoPin1);
  long distance2= getDistance(trigPin2, echoPin2);

  long safeDist1 = (distance1 > 0) ? distance1: 999;
  long safeDist2 = (distance2 >0) ? distance2 : 999;


  if(millis() - lastDistancePrint >= 500) {
    lastDistancePrint = millis();
    Serial.print("DIST1: ");
      Serial.print(distance1);
    Serial.print(" inches  |  DIST2: ");
    Serial.print(distance2);
    Serial.println(" inches");
  }

  if (safeDist1 < 25 || safeDist2 < 25) {
    tone(buzzerPin,1000); 
  } else{
    noTone(buzzerPin);
  }

  if (inGracePeriod) {
      if(millis() - gracePeriodStart >= 15000) {
      inGracePeriod = false;
      Serial.println("-> Grace period ended. Resuming normal obstacle scanning.");
    } 
    else if (safeDist1 > 10 && safeDist2 > 10) {
        inGracePeriod = false;
      Serial.println("-> Obstacle cleared during grace period! Normal life resumed.");
    }
  }

  if (!inGracePeriod) {
    if (safeDist1 <= 10 || safeDist2 <= 10) {
      
      if (!isBraking) {
        isBraking= true;
        brakeServo.write(90);
        countdownRemaining =60;
        lastTimerTick = millis();
        Serial.println("\n*** OBSTACLE DETECTED! BRAKES APPLIED! ***");
      } 
      else {
        if(millis() - lastTimerTick >= 1000) {
          countdownRemaining--;
          lastTimerTick = millis();
          Serial.print("Countdown: ");
          Serial.print(countdownRemaining);
          Serial.println(" seconds remaining...");
        }


        if (digitalRead(buttonPin)== LOW) {
          isBraking = false;
           brakeServo.write(0); 
          inGracePeriod = true;
          gracePeriodStart = millis();
          Serial.println("\n-> MANUAL RESET PRESSED! Brakes Released.");
          Serial.println("-> You have 15 seconds to move the vehicle!");
        }

        if (countdownRemaining <= 0) {
          isBraking = false;
          brakeServo.write(0); 
           inGracePeriod = true;
          gracePeriodStart = millis();
          Serial.println("\n-> COUNTDOWN HIT 0! Brakes Released.");
          Serial.println("-> You have 15 seconds to move the vehicle!");
        }
      }
    } 
    else {
      if(isBraking) {
        isBraking = false;
        brakeServo.write(0); 
        Serial.println("\n*** OBSTACLE CLEARED! Brakes Released, Timer Stopped. ***");
        Serial.println("*** Normal life resumed as it should be! ***\n");
      }
    }
  }
}

long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); 

  return duration * 0.0135 /2;
}

/*
-  It's constantly checking both sensors to see how far away stuff is in inches. It prints the distance to your screen every half second so it doesn't totally spam the console.
-If anything gets closer than 25 inches to either sensor, the buzzer starts screaming. It shuts up as soon as the thing moves away.
- If something gets way too close (10 inches or less), the servo instantly flips to 90 degrees to slam the brakes, and starts a 60-second countdown.
- If whatever was blocking the way moves while the brakes are on, it instantly kills the timer, lets go of the brakes, and goes back to normal without you having to wait around.
-If you hit the physical button (or if the 60 seconds finally run out), the brakes release and you get a 15-second "grace period". This just gives you time to back the car up without the brakes auto-triggering again.
*/