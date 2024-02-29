#include <Servo.h>
#include <Wire.h>
#include <hd44780.h>
#include <LiquidCrystal_I2C.h>

// Initialize LCD object with I2C address, columns, and rows
LiquidCrystal_I2C lcd(0x27, 20, 4);
Servo myservo;

// Define the total number of sensors
const int totalSensors = 14;

// Array to store sensor pin numbers
const int sensorPins[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

// Array to store sensor names
const String sensorNames[] = { "P1", "P2", "P3", "P4", "P5", "P6", "P7", "P8", "P9", "P10", "P11", "P12", "P13", "P14" };

// Array to store the status of each parking space
String parkingSpaces[totalSensors];

// Defined Pins:
const int inSensorPin = 23;
const int outSensorPin = 24;
const int doorServoPin = 22;
const int relayPin = 16;

// Variables to store sensor counts
int activeSensorCount = 0;
int inactiveSensorCount = 0;
bool isParkFull = false;

// Setup function runs once at the beginning
void setup() {
  Serial.begin(9600);

  // Initialize and configure LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print("Welcome to");
  lcd.setCursor(3, 2);
  lcd.print("Smart Parking");
  delay(2000);
  // Initialize Servo and set initial position
  myservo.attach(doorServoPin);
  myservo.write(0);

  // Set sensor pins as inputs
  for (int i = 0; i < totalSensors; i++) {
    pinMode(sensorPins[i], INPUT);
  }

  // Set other pins
  pinMode(inSensorPin, INPUT);
  pinMode(outSensorPin, INPUT);
  pinMode(A0, INPUT);
  pinMode(relayPin, OUTPUT);
}

// Loop function runs repeatedly
void loop() {
  // Initialize parking spaces status
  initializeParkingSpaces();

  // Update parking status based on sensor readings
  updateParkingStatus();

  // Display parking spaces status on LCD
  displayParkingSpaces();

  // Check for cars and perform actions accordingly
  checkCarIn();
  checkCarOut();

  // Turn on/off lights based on LDR reading
  turnOnLight();

  // Delay to avoid excessive processing
  delay(1000);
  Serial.println("----------");
}



// Function to control the lights based on LDR reading
void turnOnLight() {
  int ldrValue = analogRead(A0);
  // Check if it's dark
  if (ldrValue > 100) {
    // Turn on the relay and, consequently, the LED
    digitalWrite(relayPin, HIGH);
  } else {
    // Turn off the relay and, consequently, the LED
    digitalWrite(relayPin, LOW);
  }
}

// Check the overall parking status, if it's full or not
void checkParkingStatus() {
  for (int i = 0; i < totalSensors; i++) {
    int sensorReading;
    // Adjusting sensor readings for specific sensors
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
  if (activeSensorCount == 14) {
    isParkFull = true;
    lcd.clear();
    lcd.setCursor(2, 1);
    lcd.print("Park is Full");
  } else {
    isParkFull = false;
  }
  Serial.println(isParkFull);
  activeSensorCount = 0;
  inactiveSensorCount = 0;
}

// Open the parking door if a car is detected
void openDoor() {
  lcd.clear();
  lcd.setCursor(3, 1);
  lcd.print("Opening Door...");

  for (int i = 0; i <= 90; i++) {
    myservo.write(i);
    delay(10);
  }

  delay(1000);

  lcd.clear();
  lcd.setCursor(3, 1);
  lcd.print("Door Opened!");
  delay(3000);  // Adjust the delay according to your servo motor

  lcd.clear();
  lcd.setCursor(3, 1);
  lcd.print("Closing Door...");
  for (int i = 90; i >= 0; i--) {
    myservo.write(i);
    delay(10);
  }
}

// Initialize parking spaces with default values
void initializeParkingSpaces() {
  for (int i = 0; i < totalSensors; i++) {
    parkingSpaces[i] = sensorNames[i];
  }
}

// Update the status of each parking space based on sensor readings
void updateParkingStatus() {
  for (int i = 0; i < totalSensors; i++) {
    int sensorReading;

    // Adjusting sensor readings for specific sensors
    if (i < 3 || i > 6) {
      sensorReading = digitalRead(sensorPins[i]);
    } else {
      sensorReading = !digitalRead(sensorPins[i]);
    }

    // Update the parking space status
    if (sensorReading == HIGH) {
      parkingSpaces[i] = "USED";
      activeSensorCount++;
    } else {
      inactiveSensorCount++;
    }
  }
  if (activeSensorCount == 14) {
    isParkFull = true;
    lcd.clear();
    lcd.setCursor(4, 1);
    lcd.print("Park is Full");
    delay(2000);
  } else {
    isParkFull = false;
  }
  activeSensorCount = 0;
  inactiveSensorCount = 0;
}

// Display the parking spaces status on the LCD
void displayParkingSpaces() {
  lcd.clear();

  int row = 0;
  int col = 0;

  for (int i = 0; i < totalSensors; i++) {
    lcd.setCursor(col, row);
    lcd.print(parkingSpaces[i]);
    col += 5;  // Assuming each character takes up 5 columns

    if (col >= 16) {
      col = 0;
      row++;  // Move to the next line after every 4 items
    }
  }
}
// Check if there is a car detected by the entrance sensor
bool isCarDetected() {
  int sensorState = digitalRead(inSensorPin);
  return !sensorState;
}
void checkCarOut() {
  int sensorState = digitalRead(outSensorPin);
  Serial.print("OUT sensorState : ");
  Serial.println(sensorState);
  if (sensorState != HIGH) {
    openDoor();
  }
}
// Check for cars at the entrance and perform actions accordingly
void checkCarIn() {
  if (isCarDetected() && isParkFull == true) {
    delay(1000);
    lcd.clear();
    lcd.setCursor(7, 1);
    lcd.print("Sorry");
    lcd.setCursor(4, 2);
    lcd.print("Park is Full");
    delay(1000);
  } else if (isCarDetected() && isParkFull == false) {
    openDoor();
  }
}
