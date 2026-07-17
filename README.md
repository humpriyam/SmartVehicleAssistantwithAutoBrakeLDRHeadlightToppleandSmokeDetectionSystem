# Arduino Advanced EV Safety System (ADAS) 🚗🔌

An Arduino-based Advanced Driver Assistance System (ADAS) and safety suite designed for Electric Vehicles. This project integrates multiple smart subsystems into a cohesive environment, focusing on driver convenience, battery safety, and emergency response.

## 🌟 Key Features

### 1. Automatic Parking Assistant & Emergency Braking
* **Dual Continuous Monitoring:** Utilizes two ultrasonic sensors (HC-SR04) to monitor obstacle distances continuously.
* **Proximity Alarm:** Activates a piezo buzzer when an obstacle is within 25 inches.
* **Auto-Braking System:** If an obstacle breaches the 10-inch critical threshold, a servo motor instantly engages (simulating brakes) and starts a 60-second lock-down timer.
* **Dynamic Cancellation & Manual Override:** Brakes automatically release if the obstacle is cleared. A manual pushbutton allows the driver to abort the lockdown, granting a 15-second grace period to safely reposition the vehicle.

### 2. Smart Ambient Auto-Headlights
* **LDR-Based Detection:** Constantly monitors ambient light levels using a photoresistor.
* **Dynamic Dimming (PWM):** Headlight (LED) brightness scales inversely with ambient light. As it gets darker outside, the headlights automatically get brighter.
* **Manual Toggle:** Includes a physical pushbutton to easily toggle the auto-headlight system on and off.

### 3. Battery & Motor Temperature Monitoring
* **Differential Temperature Sensing:** Uses two TMP36 sensors to measure temperatures at different ends of the battery pack (or motor).
* **LCD Readout:** Displays real-time temperatures on a 16x2 LCD screen.
* **Thermal Anomaly Detection:** Calculates the temperature delta (difference) between the two sensors. If the delta exceeds 5°C, the system detects a potential cell short or localized overheating and displays a "**Possible Danger!**" warning.

### 4. Crash & Topple (Rollover) Detection
* **G-Force Tracking:** Uses an MPU6050 Accelerometer/Gyroscope to monitor vehicle orientation and sudden impacts.
* **Crash Detection:** Triggers an emergency state if sudden acceleration/deceleration exceeds 2.5Gs (25 m/s²).
* **Rollover Detection:** Monitors the Z-axis gravity vector. If the Z-axis acceleration drops below 4.0 m/s², the system concludes the vehicle has rolled over.
* **Emergency Response:** Rapidly flashes an alert LED and outputs commands intended to trigger a GPS/GSM module for emergency SOS dispatch. *(Note: Code includes simulated GPS/GSM triggers ready for hardware integration).*

---

## 🛠️ Hardware & Components Required
To build the complete suite, you will need:
* 1x Arduino Uno R3 (or compatible microcontroller)
* 2x HC-SR04 Ultrasonic Distance Sensors (4-pin)
* 2x TMP36 Temperature Sensors
* 1x MPU6050 Accelerometer/Gyroscope Module
* 1x 16x2 LCD Display (with 250kΩ Potentiometer for contrast)
* 1x Micro Servo Motor (Positional)
* 1x Piezo Buzzer
* 1x Photoresistor (LDR) + 10kΩ Resistor
* LEDs (Red/Alert) + 220Ω/330Ω Resistors
* Pushbuttons (for toggles and overrides)
* Breadboards and Jumper Wires

*(Note: Future iterations include hardware integration for MQ-2 Smoke Detection and a physical SIM/GPS module).*

---

## 💻 Software & Libraries Used
This project is written in standard C++ for the Arduino IDE. It relies heavily on non-blocking programming (using `millis()`) to ensure all safety systems run concurrently without lagging.

**Required Arduino Libraries:**
* `Servo.h` (Built-in) - For brake actuation.
* `LiquidCrystal.h` (Built-in) - For the 16x2 Display.
* `Wire.h` (Built-in) - For I2C communication.
* `Adafruit_MPU6050.h` - For crash/topple detection.
* `Adafruit_Sensor.h` - Dependency for the MPU6050.

---

## ⚙️ Installation & Usage
1. Clone this repository to your local machine:
   ```bash
   git clone [https://github.com/YOUR-USERNAME/YOUR-REPO-NAME.git](https://github.com/YOUR-USERNAME/YOUR-REPO-NAME.git)