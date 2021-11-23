// Williams Road Show Lock Mod
// Demo version using MAX7219 Driver
#include <SPI.h>
#include <Adafruit_GFX.h>
#include "RoadShowImg.h"
#include "Max72xxPanel.h"
#define LampPin 5

// Setting up 7219 Display

/*#define SDA     D7      // Pin sda (I2C)
  #define SCL     D5     // Pin scl (I2C)
  #define CS      D8     // Pin cs  (SPI)*/
int pinCS = 15; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 2;
int numberOfVerticalDisplays = 1;
int x = numberOfHorizontalDisplays * 8 / 2;
int y = numberOfVerticalDisplays * 8 / 2;
Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

// Define timers to use to check what graphic loop need to be played
unsigned long previousMillis = 0;
const long RoadLockTimer = 3000; //Time to stay in lock mode after last lock lampflash!
unsigned long currentMillis = millis();
unsigned long triggerMillis;

// Interrupt that set time for last lock lamp update.
ICACHE_RAM_ATTR void  lower() {
  triggerMillis = millis();
  //Serial.println("Interrupt Trigger!");
}


void setup() {
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(LampPin), lower, RISING);
  matrix.setIntensity(1); // Use a value between 0 and 15 for brightness
  // Orientation of display display.
  matrix.setPosition(0, 1, 0);
  matrix.setPosition(1, 0, 0);
  matrix.setRotation(0, 3);
  matrix.setRotation(1, 3);
  // Splash Animation
  Road_Show_Splash();
}

void loop() {
  currentMillis = millis();
  if (currentMillis - triggerMillis <= RoadLockTimer) {
    Serial.println("Road Lock Mode!");
    int i = 0;
    matrix.fillScreen(0);
    while ( i <= 16) {
      matrix.drawBitmap(i, 0, arrow_bmp, 16, 8, HIGH);
      matrix.write();
      delay(500);
      i = i + 4;
    }
    matrix.fillScreen(0);
    matrix.drawBitmap(0, 0, lock_bmp, 16, 8, HIGH);
    matrix.write();
    delay(1000);
    // Keep Lock Running.
    int val = digitalRead(LampPin);
    if (val == HIGH) triggerMillis = millis();
  }
  else {
    Serial.println("Road Closed Mode!");
    matrix.fillScreen(0);
    matrix.setCursor(x, 0);
    matrix.drawBitmap(0, 0, x_bmp, 16, 8, HIGH);
    matrix.write();
    delay(1000);
    for (int i = 1; i < 7; i++) {
      matrix.fillScreen(0);
      matrix.drawBitmap(0, 0, flash_bmp, 16, 8, HIGH);
      matrix.write();
      delay(100);
      matrix.drawBitmap(0, 0, flash_bmp, 16, 8, LOW);
      matrix.write();
      delay(100);
    }
  }
}

void Road_Show_Splash() {
  for (int i = 1; i < 5; i++) {
    matrix.fillScreen(0);
    matrix.drawBitmap(0, 0, stop_bmp, 16, 8, HIGH);
    matrix.write();
    delay(250);
    matrix.drawBitmap(0, 0, stop_bmp, 16, 8, LOW);
    matrix.write();
    delay(250);
  }
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.drawBitmap(0, 0, Road_bmp, 16, 8, HIGH);
  matrix.write();
  delay(1500);
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.drawBitmap(0, 0, Show_bmp, 16, 8, HIGH);
  matrix.write();
  delay(1500);
  for (int i = 1; i < 5; i++) {
    matrix.fillScreen(0);
    matrix.drawBitmap(0, 0, stop_bmp, 16, 8, HIGH);
    matrix.write();
    delay(250);
    matrix.drawBitmap(0, 0, stop_bmp, 16, 8, LOW);
    matrix.write();
    delay(250);
  }
}
