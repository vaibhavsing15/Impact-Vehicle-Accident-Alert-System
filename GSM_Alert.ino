#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial mySerial(2, 3); // SIM800L Tx & Rx
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Impact Level ");
  delay(200);
  lcd.setCursor(0, 1);
  lcd.print("Monitoring");
  delay(2000);
  lcd.clear();
  mySerial.println("AT+CMGF=1");
  updateSerial();
  mySerial.println("AT+CMGS="+919359516172"");
  updateSerial();
  mySerial.print("Impact Level Based Vehicle Monitoring");
  updateSerial();
  mySerial.write(26);
  lcd.setCursor(0, 0);
  lcd.print("Impact Level =");
}

void loop() {
  delay(1);
  if (analogRead(A0) > 450) {
    int impact1 = (analogRead(A0)-340)*3;
    sendSMS("Front Bumper", impact1);
  }
  if (analogRead(A2) > 450) {
    int impact2 = (analogRead(A1)-340)*3;
    sendSMS("Left Door", impact2);
  }
  if (analogRead(A1) > 450) {
    int impact3 = (analogRead(A2)-340)*3;
    sendSMS("Right Door", impact3);
  }
  if (analogRead(A3) > 450) {
    int impact4 = (analogRead(A3)-340)*3;
    sendSMS("Rear Bumper", impact4);
  }
}

void sendSMS(String location, int impact) {
  mySerial.println("AT+CMGS="+919359516172"");
  updateSerial();
  mySerial.print("Impact Detected " + location);
  mySerial.print(" impact level = ");
  mySerial.print(impact);
  mySerial.print(" gram");
  updateSerial();
  mySerial.write(26);
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print(impact);
  lcd.print(" gram " + location);
}

void updateSerial() {
  delay(500);
  while (Serial.available()) {
    mySerial.write(Serial.read());
  }
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
}