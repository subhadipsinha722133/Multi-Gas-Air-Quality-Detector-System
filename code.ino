#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Sensor Analog Pins
const int mq2Pin = A0;      // Smoke (MQ-2)
const int mq3Pin = A1;      // Alcohol (MQ-3)
const int mq135Pin = A2;    // Toxic Air Quality (MQ-135)

// Output & Input Pins
const int buttonPin = 2;    // Mode Selector Button
const int buzzerPin = 8;    // Buzzer
const int redLedPin = 9;    // Red LED (Alert)
const int greenLedPin = 10; // Green LED (Safe)

// Sensitivity Thresholds
const int SMOKE_THRESHOLD = 350;    // MQ-2
const int ALCOHOL_THRESHOLD = 300;  // MQ-3
const int AIR_THRESHOLD = 600;      // MQ-135

// Variables
int currentMode = 0; // 0 = MQ-2 (Default pehle yehi rahega), 1 = MQ-3, 2 = MQ-135

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Button pin HIGH rehta hai, press par LOW

  // Initial outputs
  digitalWrite(buzzerPin, LOW);
  digitalWrite(redLedPin, LOW);
  digitalWrite(greenLedPin, HIGH);

  // OLED Init
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // Startup Screen
  display.setTextSize(1);
  display.setCursor(15, 15);
  display.print("MULTI GAS DETECTOR");
  display.setCursor(20, 35);
  display.print("Warming Sensors...");
  display.display();

  // 5 second basic warm-up
  for (int i = 0; i < 5; i++) {
    digitalWrite(greenLedPin, LOW);
    delay(500);
    digitalWrite(greenLedPin, HIGH);
    delay(500);
  }

  display.clearDisplay();
}

void loop() {
  // 1. Button Press Logic (Reliable Loop Switcher)
  if (digitalRead(buttonPin) == LOW) {
    delay(30); // Debounce delay
    if (digitalRead(buttonPin) == LOW) {
      // Mode change: 0 -> 1 -> 2 -> wapas 0
      currentMode++;
      if (currentMode > 2) {
        currentMode = 0;
      }

      // Button dabane par confirmation click/beep
      digitalWrite(buzzerPin, HIGH);
      delay(40);
      digitalWrite(buzzerPin, LOW);

      // Jab tak button chhod na dein tab tak yahan wait karega (koi freeze ya multiple skip nahi hoga)
      while (digitalRead(buttonPin) == LOW) {
        delay(10);
      }
    }
  }

  // 2. Read Active Sensor
  int reading = 0;
  String modeName = "";
  int threshold = 0;

  if (currentMode == 0) {
    modeName = "MODE: Smoke (MQ-2)";
    reading = analogRead(mq2Pin);
    threshold = SMOKE_THRESHOLD;
  } 
  else if (currentMode == 1) {
    modeName = "MODE: Alcohol (MQ-3)";
    reading = analogRead(mq3Pin);
    threshold = ALCOHOL_THRESHOLD;
  } 
  else if (currentMode == 2) {
    modeName = "MODE: AirQual (MQ-135)";
    reading = analogRead(mq135Pin);
    threshold = AIR_THRESHOLD;
  }

  // 3. Alert / Safe Logic
  bool isAlert = (reading > threshold);

  if (isAlert) {
    digitalWrite(buzzerPin, HIGH);  // Buzzer ON
    digitalWrite(redLedPin, HIGH);   // Red LED ON
    digitalWrite(greenLedPin, LOW);  // Green LED OFF
  } else {
    digitalWrite(buzzerPin, LOW);   // Buzzer OFF
    digitalWrite(redLedPin, LOW);    // Red LED OFF
    digitalWrite(greenLedPin, HIGH); // Green LED ON
  }

  // 4. OLED Display
  display.clearDisplay();

  // Mode Header
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(modeName);
  display.drawLine(0, 11, 128, 11, SSD1306_WHITE);

  // Live Value
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.print("Live Value: ");
  display.setTextSize(2);
  display.print(reading);

  // Status Box
  display.setTextSize(2);
  display.setCursor(15, 45);
  if (isAlert) {
    display.print("! ALERT !");
  } else {
    display.print("  SAFE   ");
  }

  display.display();
}
