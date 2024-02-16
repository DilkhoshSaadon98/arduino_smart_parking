#include <Servo.h>
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

hd44780_I2Cexp lcd;  // declare lcd object: auto locate & config exapander chip

// LCD geometry
const int LCD_COLS = 20;
const int LCD_ROWS = 4;

Servo myservo;

// Define the total number of sensors
const int totalSensors = 14;

// Array to store sensor pin numbers
const int sensorPins[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
const int enterSensorPin = 23;
const int doorServoPin = 22;
// Array to store sensor names
const String sensorNames[] = { "S1", "S2", "S3", "S4", "S5", "S6", "S7", "S8", "S9", "S10", "S11", "S12", "S13", "S14" };

// Variables to store sensor counts
int activeSensorCount = 0;
int inactiveSensorCount = 0;
bool isParkFull = false;
void setup() {
  Serial.begin(9600);

  myservo.attach(doorServoPin);
  myservo.write(0);
  // Set sensor pins as inputs
  for (int i = 0; i < totalSensors; i++) {
    pinMode(sensorPins[i], INPUT);
  }
  pinMode(enterSensorPin, INPUT);
}

void loop() {
  checkParkingStatus();
  if (isCarDetected() && isParkFull == true) {
    Serial.println("Carage is Full");
  } else if (isCarDetected() && isParkFull == false) {
    openDoor();
  }
  delay(1000);
}

void checkParkingStatus() {
  for (int i = 0; i < totalSensors; i++) {
    int sensorReading;

    // Check if the sensor is S (inverted logic for sensors 1-4 and 9-14)
    if (i < 3 || i > 6) {
      sensorReading = digitalRead(sensorPins[i]);
    } else {
      sensorReading = !digitalRead(sensorPins[i]);
    }

    if (sensorReading == HIGH) {
      activeSensorCount++;
    } else {
      inactiveSensorCount++;
    }
  }
  Serial.print("Active : ");
  Serial.println(activeSensorCount);
  Serial.print("UnActive : ");
  Serial.println(inactiveSensorCount);
  if (activeSensorCount == 14) {
    isParkFull = true;
    Serial.println("Full");
  } else {
    isParkFull = false;
    Serial.println("NOT Full");
  }
  // Reset counts for the next iteration
  activeSensorCount = 0;
  inactiveSensorCount = 0;
}

void displayParkingStatus() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Parking Status:");

  for (int i = 0; i < totalSensors; i++) {
    lcd.setCursor(0, i + 1);
    lcd.print(sensorNames[i]);
    lcd.print(": ");
    lcd.print(digitalRead(sensorPins[i]) == HIGH ? "Full" : "Empty");
  }

  // if (isParkingFull()) {
  //   lcd.setCursor(0, totalSensors + 1);
  //   lcd.print("Garage Full!");
  // }
}

bool isParkingFull(bool state) {
  // for (int i = 0; i < totalSensors; i++) {
  //   int stets = digitalRead(sensorPins[i]);
  //     Serial.println(stets);
  //   if (digitalRead(sensorPins[i]) == HIGH) {
  //     Serial.println("Is not full");
  //     return false;  // Parking is not full
  //   }
  // }
  Serial.println(state);
  return state;  // Parking is full
}
void openDoor() {
  Serial.println("Door opened");
  // lcd.clear();
  // lcd.print("Opening Door...");
  // delay(1000);
  // lcd.clear();
  // lcd.print("Door Opened!");
  myservo.write(90);
  delay(2000);  // Adjust the delay according to your servo motor

  myservo.write(0);
}

void closeDoor() {
  lcd.clear();
  lcd.print("Closing Door...");
  delay(1000);
  lcd.clear();
  lcd.print("Door Closed!");
  myservo.write(0);
}
bool isCarDetected() {
  int sensorState = digitalRead(enterSensorPin);
  Serial.println(sensorState);
  if (sensorState != HIGH) {
    Serial.println("There is a Car");
    return true;
  } else {
    Serial.println("There is no Car");
    return false;
  }
}