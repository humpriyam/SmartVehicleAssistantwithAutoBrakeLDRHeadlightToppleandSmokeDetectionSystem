const int ambientButtonPin= 7; 
 const int ledPin = 11;  
const int ldrPin =A0; 

bool autoHeadlights = false; 
  bool lastBtnState = HIGH; 

void setup() {
  Serial.begin(9600);

   pinMode(ambientButtonPin, INPUT_PULLUP);
  pinMode(ledPin,OUTPUT);
}

void loop() {
  
  bool currentBtn = digitalRead(ambientButtonPin);
  
  if (currentBtn == LOW && lastBtnState == HIGH) {
    autoHeadlights = !autoHeadlights; 
    Serial.print("Auto headlights are now: ");
      Serial.println(autoHeadlights ? "ON" : "OFF");
    delay(50); 
  }
   lastBtnState = currentBtn;


  if(autoHeadlights) {
     int lightLevel = analogRead(ldrPin);
     
     int brightness= map(lightLevel, 200, 800, 255, 0);
     brightness = constrain(brightness, 0, 255);
     
    analogWrite(ledPin, brightness);
  } else{
     analogWrite(ledPin, 0); 
  }
  
  delay(10); 
}

/*
Here is how this little headlight setup works:

- The On/Off Switch: The pushbutton acts like a toggle switch for the system. Press it once, auto-headlights turn on. Press it again, they turn off. I added a little delay in there so it doesn't accidentally trigger a hundred times from a single press.
- Reading the Room: When the system is on, the LDR (photoresistor) constantly measures how much light is hitting it. 
- Smart Dimming: Instead of just turning the LED strictly on or off, it uses a neat trick (the map function). If the room is getting kinda dim, the LED turns on a little bit. If it's pitch black, the LED blasts at full brightness. 
- No Glitching out: The constrain part is just a safety net. If a super bright flashlight hits the sensor, or it gets darker than expected, it stops the LED math from glitching out and doing weird things. It forces the brightness to stay between 0 (off) and 255 (max bright).
*/