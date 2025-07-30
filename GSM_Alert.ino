#include <SoftwareSerial.h>              // Library for software serial communication
#include <Wire.h>                        // Library for I2C communication
#include <LiquidCrystal_I2C.h>           // Library for LCD with I2C interface

// Initialize software serial on pins 2 (Rx) and 3 (Tx)
SoftwareSerial mySerial(2, 3);           // Used to communicate with GSM module

// Initialize LCD at I2C address 0x27 with 16 columns and 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);                   // Start serial monitor for debugging
  mySerial.begin(9600);                 // Start communication with GSM module

  lcd.init();                           // Initialize the LCD
  lcd.backlight();                      // Turn on LCD backlight
  lcd.clear();                          // Clear LCD screen
  lcd.setCursor(0, 0);
  lcd.print("Impact Level ");           // Display initial message
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print("Monitoring...");           // Display sub-message
  delay(2000);
  lcd.clear();

  mySerial.println("AT+CMGF=1");        // Set GSM module to text mode
  updateSerial();

  mySerial.println("AT+CMGS=\"+919359516172\""); // Set recipient number (in quotes)
  updateSerial();
  mySerial.print("Impact Level Based Vehicle Monitoring"); // Initial message
  updateSerial();
  mySerial.write(26);                   // ASCII code for CTRL+Z (end of SMS)

  lcd.setCursor(0, 0);
  lcd.print("Impact Level =");          // Ready state on LCD
}

void loop() {
  delay(1);                             // Small delay to reduce CPU usage

  // Read from analog pin A0 for front bumper impact
  if (analogRead(A0) > 450) {
    int impact1 = (analogRead(A0) - 340) * 3;
    sendSMS("Front Bumper", impact1);
  }

  // Read from analog pin A2 for left door impact
  if (analogRead(A2) > 450) {
    int impact2 = (analogRead(A2) - 340) * 3;
    sendSMS("Left Door", impact2);
  }

  // Read from analog pin A1 for right door impact
  if (analogRead(A1) > 450) {
    int impact3 = (analogRead(A1) - 340) * 3;
    sendSMS("Right Door", impact3);
  }

  // Read from analog pin A3 for rear bumper impact
  if (analogRead(A3) > 450) {
    int impact4 = (analogRead(A3) - 340) * 3;
    sendSMS("Rear Bumper", impact4);
  }
}

// Function to send SMS using GSM module
void sendSMS(String location, int impact) {
  mySerial.println("AT+CMGS=\"+919359516172\"");  // Send to registered number
  updateSerial();
  mySerial.print("Impact Detected: " + location + "\nImpact level: ");
  mySerial.print(impact);               // Include calculated impact
  mySerial.print(" grams");             // Add unit
  updateSerial();
  mySerial.write(26);                   // End the message with CTRL+Z
  delay(1000);                          // Wait to ensure SMS is sent

  // Show message on LCD screen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(location);                  // Show location
  lcd.setCursor(0, 1);
  lcd.print("Impact: ");
  lcd.print(impact);                    // Show impact value
  lcd.print("g");
}

// Function to forward serial data between Arduino and GSM module
void updateSerial() {
  delay(500);                           // Wait before transferring
  while (Serial.available()) {
    mySerial.write(Serial.read());     // Forward from PC to GSM
  }
  while (mySerial.available()) {
    Serial.write(mySerial.read());     // Forward from GSM to PC
  }
}