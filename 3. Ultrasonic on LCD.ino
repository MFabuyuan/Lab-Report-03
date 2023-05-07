/* Ultrasonic on LCD
This code uses an ultrasonic sensor that measures the distance between the sensor and the object in front of it. The measured distance value will be displayed on both Serial Monitor and LCD.
*/

// Define pins for Trigger and Echo of the ultrasonic sensor
#define TRIGpin 10   // This sends trigger signal
#define ECHOpin 9  // This send echo signal

// Include LiquidCrystal library for managing with LCD screen
#include <LiquidCrystal.h>

// Initialize the library by associating any needed LCD interfere pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

// Declare variables to store the duration and distance values 
float duration, distance;

void setup() {
  pinMode(ECHOpin, INPUT);   // Initialize TRIGpin as an OUTPUT
  pinMode(TRIGpin, OUTPUT);  // Initialize ECHOpin as an INPUT
  Serial.begin(9600);   // Initialize Serial Monitor with baud rate of 9600
  lcd.begin(16,2);   // Set up the LCD's number of columns and rows 
}

void loop() {
  // Set TRIGpin to LOW
  digitalWrite(TRIGpin, LOW);
  // Put delay to make the TRIGpin stable
  delayMicroseconds(2);
  // Set TRIGpin to HIGH to deliver 10ms pulse to ultrasonic sensor
  digitalWrite(TRIGpin, HIGH);  
  // Recommended minimum delay between the trigger signal and the start of of the echo pulse
  delayMicroseconds(20);
  // Set TRIGpin to LOW state to complete trigger signal
  digitalWrite(TRIGpin, LOW);  
  // Measure the the duration of the Echo pulse
  duration = pulseIn(ECHOpin, HIGH);
  // Calculate the distance in cm
  distance = (duration/2.0) * 0.0343;

  // Display the meausured Distance on the Serial Monitor
  Serial.print("Distance = ");
  // Print Out of Range on the Serial Monitor if the Distance is >=400 or <= 2
  if (distance >= 400 || distance <= 2) {
  Serial.println("Out of Range");
  }
  else {
  Serial.print(distance);
  Serial.println("cm");  // Print cm on Serial Monitor
  delay(500); 
  }
  // Display the measured Distance on the LCD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Distance = ");
   // Print Out of Range on the LCD if the Distance is >=400 or <= 2
  if (distance >= 400 || distance <= 2) {
  lcd.setCursor(0,1);
  lcd.print("Out of Range");
  }
  else {  // Display the unit (cm) for the Distance
    lcd.setCursor(0,1);
    lcd.print(distance);
    lcd.print("cm");
  }
}

