/* DHT11 and ULtrasonic with PB on LCD
This code uses DHT 11 sensor to measure temperature and humidity values. It
also uses Ultrasonic sensor to calculate the distance of an object using the
speed of sound. Ultrsonic sensor uses the temperature and humidity of values
from the DHT 11 sensor to calculate the speed of sound. This code displays
temperature, humidity, speed of sound (cm/ms) and distance (cm) values on LCD
and on Serial Monitor. A pushbutton is also connected to alternate between
showing the values. 
*/

#include "DHT.h";   // Include DHT library for working with DHT11 sensor
#include "NewPing.h"; // For Ultrasonic sensor
// Include LiquidCrystal library for managing with LCD screen
#include <LiquidCrystal.h>

#define DHTPIN 8  // Define pin for DHT 11
#define DHTTYPE DHT11  // Define the type DHT sensor being used
#define TRIGGER_PIN 10 // Define trigger pin for Ultrasonic sensor
#define ECHO_PIN 9  // Define echo pin for Ultrasonic sensor
#define MAX_DISTANCE 400  // Maximum distance (in cm) for Ultrasonic sensor

// Initialize objects for sensors and LCD display
DHT dht(DHTPIN, DHTTYPE);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

const int PB = 6;   // PB connected to pin 10
int iterations = 5; // Number of iterations for Ultrasonic sensor readings
int buttonState = LOW;  // declare a variable for current PB state
int lastButtonState = HIGH;  // declare a variable for last PB state
int PBcount = 0;  // declare a variable for count button press

// Declacre varaible to store humidity, temperature, duration, distance,
// sound's speed in m/s and sound's speed in cm
float Hum, Temp, duration, distance, SOUNDm, SOUNDcm;

// Initialize the library by associating any needed LCD interfere pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
// Define a custom character for the LCD
byte customChar[8] = {
  B11100,
  B10100,
  B11100,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

void setup() {
 pinMode(PB, INPUT_PULLUP);  // Initialize PB as an INPUT_PULLUP
 Serial.begin (9600);  // Initialize Serial Monitor with baud rate of 9600
 dht.begin();  // Initialize DHT sensor
 lcd.begin(16, 2);  // Set up LCD's number of rows and columns
 // Create a custom character to display degree symbol 
 lcd.createChar(0,customChar); 
}

void loop() {
// Read values from DHT sensor
 Hum = dht.readHumidity();
 Temp = dht.readTemperature();

// Calculate speed of sound in m/s using temperature and humidity values
 SOUNDm = 331.4 + (0.606 * Temp) + (0.0124 * Hum);

// Convert speed of sound from m/s to cm/ms
 SOUNDcm = SOUNDm / 10000;

// Calculate the distance using Ultrasonic sensor
 duration = sonar.ping_median(iterations);
 distance = (duration / 2.0) * SOUNDcm;

// Display values on Serial Monitor
 Serial.print("Sound: ");
 Serial.print(SOUNDm);
 Serial.print(" m/s, ");
 Serial.print("Sound: ");
 Serial.print(SOUNDcm);
 Serial.print(" cm/ms, ");
 Serial.print("Humid: ");
 Serial.print(Hum);
 Serial.print(" %, Temp: ");
 Serial.print(Temp);
 Serial.print(" C, ");
 Serial.print("Distance: ");

// Print Out of Range on the Serial Monitor if the Distance is >=400 or <= 2
 if (distance >= 400 || distance <= 2) {
 Serial.print("Out of range");
}
 else {
 Serial.print(distance);
 Serial.println(" cm");
}
  // Read PB state and store results in the buttonState variable
   buttonState = digitalRead(PB);
  if (buttonState != lastButtonState ) {  // Check if button was just pressed
  // debounce delay
  delay(50);
 if (buttonState == HIGH) {
   PBcount++; 
   // Reset PBcount to 1 if it reaches the maximum count of 3
 if (PBcount == 3 ) {
   PBcount = 1;
 }
 }
  }
  // Switch statement to handle the different cases of the pushbutton press
 switch (PBcount) {
  case 1:  
    // Print Temperature value on LCD 
    lcd.setCursor(0,0);
    lcd.print(("Temp: "));
    lcd.print(Temp);
    lcd.write((byte)0);
    lcd.print(("C"));
    lcd.print("    ");
    // Print Humidity value on LCD
    lcd.setCursor(0,1);
    lcd.print("Hum: ");
    lcd.print(Hum);
    lcd.print("%");
    lcd.print("    ");
  break;

  case 2:   
    // Print Sound's speed in cm/ms on LCD
    lcd.setCursor(0,0);
    lcd.print("Sound:");
    lcd.print(SOUNDcm);
    lcd.print("cm/ms");
    // Print Distance value on LCD
    lcd.setCursor(0,1);
    lcd.print("Distance:");
    lcd.print(distance);
    lcd.print("cm");
  break;
  }
  // Store the PB's current state for the next cycle
  lastButtonState = buttonState;
}