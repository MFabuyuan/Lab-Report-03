/* DHT 11 on LCD 
This code displays temperature and humidity readings from a DHT11 sensor on both LCD screen and Serial Monitor.
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

DHT_Unified dht(DHTPIN, DHTTYPE);   // create an object called DHT
uint32_t delayMS;   // define an unsigned integer for a delay in milliseconds

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
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

void loop() {
  // Print temperature and humdity from DHT11 sensor on the Serial Monitor
  delay(delayMS);     // Delay between measurements.
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  // If temperature is not a number then it will print Error reading temperature
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  // Print temperature value 
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
  // If humidity is not a number then it will print Error reading temperature
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  // Print Humidity value
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
  
  // Display temperature and humidity data from DHT11 sensor on the LCD Screen
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    lcd.setCursor(0,0);
    lcd.print(F("Error reading temperature!"));
  }
  // Display temperature with degree symbol on LCD
  else {
    lcd.setCursor(0,0);    
    lcd.print(F("Temp:")); 
    lcd.print(event.temperature);  
    lcd.write((byte)0);    
    lcd.print(F("C"));     
  }
    dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)){
    lcd.setCursor(0,1);
    lcd.print(F("Error reading humidity!"));
  }
  // Display humidty on LCD 
  else {
    lcd.setCursor(0,1);
    lcd.print(F("Hum:"));
    lcd.print(event.relative_humidity);
    lcd.print(F("%"));
  }
}
