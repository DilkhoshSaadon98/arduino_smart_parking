#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <hd44780.h>                        // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h>  // i2c expander i/o class header
hd44780_I2Cexp lcd;                         // declare lcd object: auto locate & config exapander chip
// LCD geometry
const int LCD_COLS = 20;
const int LCD_ROWS = 4;
Servo myservo;  

const int numParkingSpaces = 14;
const int doorServoPin = 52;
const int irSensorPins[numParkingSpaces] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

bool parkingSpaces[numParkingSpaces] = { false };  // false means empty, true means occupied

void setup() {
  lcd.begin(20, 4);
  Serial.begin(9600);
  lcd.setCursor(4, 2);
  lcd.print("Smart Parking");
  delay(2000);
  lcd.clear();
  closeDoor();                   // Initial state: door closed
  myservo.attach(doorServoPin);  // Attach the servo to digital pin 9
  for (int i = 0; i < numParkingSpaces; i++) {
    pinMode(irSensorPins[i], INPUT);
  }
}

void loop() {
  updateParkingStatus();
  displayParkingStatus();

  if (isCarDetected()) {
    openDoor();
    delay(5000);  // Wait for 5 seconds to simulate car parking time
    closeDoor();
  }

  delay(1000);  // Adjust the delay according to your system requirements
}

void updateParkingStatus() {
  for (int i = 0; i < numParkingSpaces; i++) {
    parkingSpaces[i] = digitalRead(irSensorPins[i]);
  }
}

void displayParkingStatus() {
  lcd.clear();
  lcd.print("Parking Status:");

  for (int i = 0; i < numParkingSpaces; i++) {
    lcd.setCursor(0, i + 1);
    lcd.print("Space ");
    lcd.print(i + 1);
    lcd.print(": ");
    lcd.print(parkingSpaces[i] ? "Occupied" : "Empty");
  }

  if (isParkingFull()) {
    lcd.setCursor(0, numParkingSpaces + 1);
    lcd.print("Garage Full!");
  }
}

bool isCarDetected() {
  for (int i = 0; i < numParkingSpaces; i++) {
    if (parkingSpaces[i]) {
      return true;  // Car detected in any space
    }
  }
  return false;
}

bool isParkingFull() {
  for (int i = 0; i < numParkingSpaces; i++) {
    if (!parkingSpaces[i]) {
      return false;  // At least one space is empty
    }
  }
  return true;  // All spaces are occupied
}

void openDoor() {
  lcd.clear();
  lcd.setCursor(2, 2);
  lcd.print("Opening Door...");
  delay(1000);
  lcd.clear();
  lcd.setCursor(4, 2);
  lcd.print("Door Opened!");
  myservo.write(180);
}

void closeDoor() {
  lcd.clear();
  lcd.setCursor(2, 2);
  lcd.print("Closing Door...");
  delay(1000);
  lcd.clear();
  lcd.setCursor(4, 2);
  lcd.print("Door Closed!");
  myservo.write(0);
}
