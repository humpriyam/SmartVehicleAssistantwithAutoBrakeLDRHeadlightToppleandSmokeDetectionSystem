#include <Wire.h>
 #include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

 const int alertLedPin = 13; 

float velX = 0;
 float velY= 0;
float velZ = 0;

 unsigned long lastTime= 0;

void setup() {
  Serial.begin(9600);

  pinMode(alertLedPin, OUTPUT);

  Serial.println("Starting up MPU6050...");
  
   if (!mpu.begin()) {
    Serial.println("Sensor not found! Check wiring.");
    while (1) {
      delay(10); 
    }
  }

  // Set the sensor to read up to 8G (good for crashes)
   mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  
    lastTime = millis();
}

void loop() {
  
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);


  // Calculate how much time passed since last check (in seconds)
  unsigned long currentTime = millis();
  float dt = (currentTime - lastTime) / 1000.0;
  lastTime = currentTime;


  // Calculate Velocity (v = u + at)
  // Note: We subtract 9.8 from Z to remove gravity so the car doesn't think it's flying upwards
   velX = velX + (a.acceleration.x * dt);
  velY = velY + (a.acceleration.y * dt);
   velZ = velZ + ((a.acceleration.z - 9.81) * dt);


  Serial.print("Accel (m/s^2) -> X:");
   Serial.print(a.acceleration.x);
  Serial.print(" Y:");
  Serial.print(a.acceleration.y);
  Serial.print(" Z:");
    Serial.print(a.acceleration.z);

  Serial.print("  ||  Vel (m/s) -> X:");
  Serial.print(velX);
   Serial.print(" Y:");
  Serial.print(velY);
    Serial.println();


  // --- CRASH & TOPPLE DETECTION LOGIC ---
  
  bool crashDetected = false;
  bool toppleDetected= false;

  // 1. Crash Detection: If sudden acceleration is over 25 m/s^2 (approx 2.5 G-force)
  if(abs(a.acceleration.x) > 25.0 || abs(a.acceleration.y) > 25.0) {
      crashDetected = true;
  }

  // 2. Topple Detection: If gravity stops pulling down on the Z axis and pulls on X or Y instead
   if (abs(a.acceleration.z) < 4.0) {
    toppleDetected = true;
  }


  if (crashDetected || toppleDetected) {
     
     if(crashDetected) Serial.println("\n*** MASSIVE IMPACT DETECTED! ***");
      if(toppleDetected) Serial.println("\n*** VEHICLE ROLLOVER/TOPPLE DETECTED! ***");
     
     Serial.println("-> Triggering Emergency GPS/GSM Protocols...");

    // Blink the LED wildly to show the alarm is going off
     for(int i=0; i<10; i++){
       digitalWrite(alertLedPin, HIGH);
       delay(50);
       digitalWrite(alertLedPin, LOW);
        delay(50);
     }
     
     // reset velocity so it doesn't just infinitely climb while crashed
     velX = 0; velY = 0; velZ= 0;
     
  } else {
     // Make sure LED is off if everything is fine
      digitalWrite(alertLedPin, LOW);
  }

  delay(200); 
}

/*
Here is how this Accident/Topple detection actually works:

- Tracking G-Forces: It uses the MPU6050 accelerometer to constantly measure forces on the X, Y, and Z axes. 
- Math for Speed: It estimates your current velocity by tracking how long it's been accelerating. It subtracts standard gravity from the Z-axis so the math doesn't get messed up just by the car sitting flat on the earth. (Disclaimer: accelerometer-based velocity drifts a lot in real life, but works for the concept!).
- Crash Detection: If the car suddenly hits a wall or another car, the X or Y acceleration will spike massively. I set the limit to 25 m/s^2. If it goes over that, it knows you crashed.
- Rollover/Topple Detection: Normally, gravity (9.8 m/s^2) pulls straight down on the Z-axis of the sensor. If the car flips on its side or roof, that 9.8 moves to the X or Y axis, and the Z-axis drops near zero. If Z drops below 4.0, the system knows the car is no longer upright.
- Emergency Alarm: If either of those things happens, it spams a warning to the screen, simulates triggering the GPS/GSM module mentioned in the image, and rapidly flashes the LED like a strobe light.
*/