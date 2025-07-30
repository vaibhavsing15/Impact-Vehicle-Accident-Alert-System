// Bluetooth Logger Code for Accident Monitoring
// This code sends analog sensor values over Bluetooth using HC-05
// Designed to be used with the Virtuino App

int analogPin1 = A0; // Front Bumper Sensor
int analogPin2 = A1; // Right Door Sensor
int analogPin3 = A2; // Left Door Sensor
int analogPin4 = A3; // Rear Bumper Sensor

void setup() {
  Serial.begin(9600); // Start serial communication with HC-05
}

void loop() {
  // Read values from all 4 sensors
  int val1 = analogRead(analogPin1); // Front Bumper
  int val2 = analogRead(analogPin2); // Right Door
  int val3 = analogRead(analogPin3); // Left Door
  int val4 = analogRead(analogPin4); // Rear Bumper

  // Print values with labels, format suitable for Virtuino or serial plotter
  Serial.print("Front:");
  Serial.print(val1);
  Serial.print(", Right:");
  Serial.print(val2);
  Serial.print(", Left:");
  Serial.print(val3);
  Serial.print(", Rear:");
  Serial.println(val4);

  delay(500); // Send data every half second
}