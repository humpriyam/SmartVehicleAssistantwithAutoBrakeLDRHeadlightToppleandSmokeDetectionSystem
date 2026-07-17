#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins based on the schematic
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int sensor1Pin = A0; 
  const int sensor2Pin= A1;

// Threshold for how much temperature difference triggers the danger warning
 const float dangerThreshold = 5.0; 

void setup() {
  Serial.begin(9600);
  
  // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    
  lcd.print("System Boot...");
  delay(1500);
   lcd.clear();
}

void loop() {
  
  // Read analog values from both TMP36 sensors
  int reading1 = analogRead(sensor1Pin);
   int reading2 = analogRead(sensor2Pin);

  // Convert the 0-1023 reading to voltage (assuming 5V Arduino)
  float voltage1 = reading1 * 5.0 / 1024.0;
  float voltage2= reading2 * 5.0 / 1024.0;

  // Convert voltage to Celsius
  // The TMP36 has a 500mV offset and 10mV/degree scale
  float temp1 = (voltage1 - 0.5) * 100.0;
    float temp2 = (voltage2 - 0.5) * 100.0;


  // Calculate the difference between the two ends of the battery/motor
  float tempDifference = abs(temp1 - temp2);

  
  // --- LCD Display Logic ---
  
  // Top row: Show both temperatures
  lcd.setCursor(0, 0);
  lcd.print("T1:");
  lcd.print((int)temp1); 
  lcd.print("C  T2:");
   lcd.print((int)temp2);
  lcd.print("C    "); // Extra spaces to clear old characters

  // Bottom row: Danger check
  lcd.setCursor(0, 1);
  
  if (tempDifference >= dangerThreshold) {
    lcd.print("Possible Danger!");
  } else {
     lcd.print("Status: Normal  ");
  }

  // Print to serial monitor just in case we need to debug
  Serial.print("Sensor 1: "); Serial.print(temp1); Serial.print(" C | ");
  Serial.print("Sensor 2: "); Serial.print(temp2); Serial.print(" C | ");
  Serial.print("Delta: "); Serial.println(tempDifference);

  
  delay(1000); 
}

/*
Here is a quick breakdown of how this temperature monitor works:

- Reading the Heat: The Arduino constantly takes voltage readings from the two TMP36 sensors on pins A0 and A1. It converts those raw numbers into actual Celsius temperatures using the standard TMP36 math formula.
- Side-by-Side Check: It compares the temperature of sensor 1 against sensor 2. If one side of the battery gets way hotter than the other (meaning something is probably failing or shorting out), it calculates that difference.
- The LCD Display: The top row of the screen always shows you the live temps of T1 and T2. 
- The Warning System: I set a threshold of 5 degrees. If the difference between the two sensors hits 5 degrees or more, the bottom row of the screen switches from "Status: Normal" to "Possible Danger!".
*/