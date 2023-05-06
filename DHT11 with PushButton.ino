/* DHT11 with Pushbutton
This code displays temperature and humidity data readings from a DHT11 sensor on an LCD screen. A pushbutton is connected to alternate between showing temperature in both Celsius and Fahrenheit and humidity data.
*/

// Include Adadruit_Sensor library for working with sensors
#include <Adafruit_Sensor.h> 
// Include DHT library for working with DHT11 sensor
#include <DHT.h>
#include <DHT_U.h>
// Include LiquidCrystal library for managing with LCD screen
#include <LiquidCrystal.h>

#define DHTPIN 8   // Define pin for DHT11 
#define DHTTYPE DHT11   // Define the type of sensor being used

DHT_Unified dht(DHTPIN, DHTTYPE);   // Create an object called DHT
uint32_t delayMS;   // define an unsigned integer for a delay in milliseconds

const int PB = 6;   // pB connected to pin 10
int buttonState = LOW;   // declare a variable for current PB state
int lastButtonState = LOW;   // declare a variable for last PB state
int PBcount = 0;   // declare a variable for count button press
float x;   // declare a variable to hold temperature in Fahrenheit

// Initialize the library by associating any needed LCD interfere pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

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
  // Initialize pB as an INPUT_PULLUP
   pinMode(pB, INPUT_PULLUP);  
  // Initialize Serial Monitor with baud rate of 9600 and DHT sensor
  Serial.begin(9600);   
  dht.begin();
  // Set up the LCD's number of columns and rows 
  lcd.begin(16, 2);
  // Create a custom character to display degree symbol 
  lcd.createChar(0,customChar);

  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
}

void loop() {
  // Read PB state and store results in the buttonState variable
   buttonState = digitalRead(PB);
  if (buttonState != lastButtonState ) {  // Check if button was just pressed
  // debounce delay
  delay(50);
 if (buttonState == HIGH) {
   PBcount++; 
   // Reset PBcount to 1 if it reaches the maximum count of 4
 if (PBcount == 4 ) {
   PBcount = 1;
 }
 }
  }
  // Switch statement to handle the different cases of the pushbutton press
  switch (PBcount) {
  case 1:   // Get temperature event
    sensors_event_t event;
    dht.temperature().getEvent(&event);
  // If temperature is not a number then it will print Error reading temperature    
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {   // Display temperature on LCD (Celsius) with degree symbol
    lcd.setCursor(0,0);
    lcd.print(F("Temp: "));
    lcd.print(event.temperature);
    lcd.write((byte)0);
    lcd.print(F("C"));
    lcd.print("  ");
  }
  break;
  
  case 2:   // Get humidity event
    dht.humidity().getEvent(&event);
  // If humidity is not a number then it will print Error reading temperature
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {   // Display humidity on LCD 
    lcd.setCursor(0,0);
    lcd.print(F("Hum: "));
    lcd.print(event.relative_humidity);
    lcd.print("%");
    lcd.print("  ");
    }
  break; 

  case 3:   // Get temperature event 
    dht.temperature().getEvent(&event);
  // If temperature is not a number then it will print Error reading temperature
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {   // Display temperature on LCD (Fahrenheit) with degree symbol 
   x = (event.temperature*1.8)+32.0;   // Convert Celsius to Fahrenheit
    lcd.setCursor(0,0);
    lcd.print(F("Temp: "));
    lcd.print(x);
    lcd.write((byte)0);
    lcd.print(F("F"));
    lcd.print("  ");
  }
  break; 
  }
  // Store the PB's current state for the next cycle
  lastButtonState = buttonState;
}