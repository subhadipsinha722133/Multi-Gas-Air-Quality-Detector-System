# 🛡️ Multi-Gas & Air Quality Detector System

Yeh ek compact, Arduino-based safety system hai jo alag-alag gas threats ko detect karta hai. Isme single button ki madad se teen alag-alag detection modes ke beech switch kiya ja sakta hai, aur real-time data ek 0.96" OLED display par show hota hai.

---

## 📌 Features

- **3-in-1 Detection Modes:**
  - **Smoke Mode (MQ-2):** Cigarette smoke aur combustible gases detect karne ke liye.
  - **Alcohol Mode (MQ-3):** Breath alcohol aur vapors detect karne ke liye.
  - **Air Quality Mode (MQ-135):** Zehrili gas, toxic fumes aur overall room air quality track karne ke liye.
- **Interactive Switching:** Single push button se teenon sensors ke beech seamless mode toggling.
- **Audio-Visual Alerts:** 
  - Danger limit cross hote hi high-pitch buzzer alert.
  - Dual LED status (Green = Safe, Red = Alert).
- **Live OLED Display:** Active mode, real-time sensor value aur SAFE/ALERT status ka clean display.

---

## 🛠️ Hardware Requirements

- **Microcontroller:** Arduino Uno ya Arduino Nano
- **Gas Sensors:** 
  - MQ-2 Gas & Smoke Sensor Module
  - MQ-3 Alcohol Sensor Module
  - MQ-135 Hazardous Gas & Air Quality Sensor Module
- **Display:** 0.96 Inch I2C 4-Pin OLED Display (SSD1306)
- **Audio Output:** 5V Active Buzzer
- **LED Indicators:** 1x Red LED, 1x Green LED (sath me 2x 220Ω Resistors)
- **Inputs:** 1x Push Button Switch, 1x ON/OFF Power Switch
- **Power Source:** 7.4V Li-ion Battery pack ya 5V Power Bank

---

## 🔌 Circuit Pin Connections

### Sensors & Display
| Component | Pin Name | Arduino Uno/Nano Pin |
| :--- | :--- | :--- |
| **0.96" OLED** | GND / VDD | GND / 5V |
| | SCK (SCL) | **A5** |
| | SDA | **A4** |
| **MQ-2 (Smoke)** | VCC / GND / A0 | 5V / GND / **A0** |
| **MQ-3 (Alcohol)** | VCC / GND / A0 | 5V / GND / **A1** |
| **MQ-135 (Air Quality)** | VCC / GND / A0 | 5V / GND / **A2** |

### Alert & Controls
| Component | Connection | Arduino Pin |
| :--- | :--- | :--- |
| **Push Button** | Terminal 1 to Pin, Terminal 2 to GND | **D2** (Internal Pullup) |
| **Buzzer** | Positive (+) to Pin, Negative (-) to GND | **D8** |
| **Red LED** | Anode (+) via 220Ω to Pin, Cathode (-) to GND | **D9** |
| **Green LED** | Anode (+) via 220Ω to Pin, Cathode (-) to GND | **D10** |

---

## 💻 Software & Libraries

Code ko Arduino IDE mein compile aur upload karne ke liye in libraries ki zarurat hogi:
1. `Wire.h` (Built-in)
2. `Adafruit GFX Library`
3. `Adafruit SSD1306`

> **Note:** Libraries ko install karne ke liye Arduino IDE me jayein: `Sketch -> Include Library -> Manage Libraries...` aur search karke install karein.

---

## ⚙️ Calibration & Thresholds

Sensors ki baseline values ke hisaab se code ke shuruat mein threshold values set ki gayi hain:
- `SMOKE_THRESHOLD = 350`
- `ALCOHOL_THRESHOLD = 300`
- `AIR_THRESHOLD = 650` *(MQ-135 ki normal fresh air reading aam taur par 500-580 ke beech rehti hai)*

---

## 🚀 How It Works

1. **Warm-Up Phase:** System power on hone ke baad sensors ko garam hone ke liye 5–10 seconds ka time milta hai (Green LED blink karegi).
2. **Default State:** System default roop se **Smoke (MQ-2)** mode par start hota hai.
3. **Mode Toggling:** Push button dabane par sequence chalega:  
   `MQ-2 (Smoke) ➔ MQ-3 (Alcohol) ➔ MQ-135 (Air Quality) ➔ Back to MQ-2`
4. **Trigger Action:** Agar active sensor ki value threshold cross karti hai, OLED par `! ALERT !` likh kar aayega, Red LED on ho jayegi aur buzzer sound karega.
