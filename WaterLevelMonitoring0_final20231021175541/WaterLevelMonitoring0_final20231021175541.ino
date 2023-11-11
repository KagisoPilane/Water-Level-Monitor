/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID "TMPL2klgA1u4M"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "DSlfQgKxREcmifvR57SSIeXXQJ4lbo-N"

// Your WiFi credentials.
char auth[] = "DSlfQgKxREcmifvR57SSIeXXQJ4lbo-N";   
char ssid[] = "HUAWEI-D755";
char pass[] = "ADEAGT63678";

//Set trigger value in percentage
int triggerPer =   10 ;  //alarm/pump will start when water level drop below triggerPer

#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>        
#include <BlynkSimpleEsp8266.h>
#include <AceButton.h>
using namespace ace_button; 

// Define connections to sensor
#define TRIGPIN    D6  //D6
#define ECHOPIN    D7  //D7
#define wifiLed    16  //D0

//Change the virtual pins according the rooms
#define VPIN_BUTTON_0    V0
#define VPIN_BUTTON_1    V1 
#define VPIN_BUTTON_2    V2
#define VPIN_BUTTON_3    V3 
#define VPIN_BUTTON_4    V4
#define VPIN_BUTTON_5    V5
#define VPIN_BUTTON_6    V6
#define VPIN_BUTTON_7    V7 
#define VPIN_BUTTON_8    V8
#define VPIN_BUTTON_9    V9
#define VPIN_FULL_TANK V10  // Text Input for Full Tank Distance
#define VPIN_EMPTY_TANK V11 // Text Input for Empty Tank Distance

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG
#define APP_DEBUG
// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

int emptyTankDistance = 170;  //Distance when tank is empty
int fullTankDistance  = 40;  //Distance when tank is full (must be greater than 25cm)

int newEmptyTankDistance = 0; // New empty tank distance from Blynk
int newFullTankDistance = 0;  // New full tank distance from Blynk

int analogInPin  = A0;    // Analog input pin
float voltage;
int bat_percentage;
float calibration = 0.40; // Check Battery voltage using multimeter & add/subtract the value

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float duration;
float distance;
int   waterLevelPer;

String currMode;

#include <SoftwareSerial.h>
#define BLYNK_PRINT Serial

//Install the following Libraries
#include <TinyGPS++.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//GPS RX to D3 & GPS TX to D4 and Serial Connection
const int RXPin = 0, TXPin = 2;
const uint32_t GPSBaud = 9600; 
SoftwareSerial gps_module(RXPin, TXPin);

TinyGPSPlus gps; 
WidgetMap myMap(V0); //V0 - virtual pin for Map

BlynkTimer timer;

float gps_speed;
float no_of_satellites;
String satellite_orientation;

//unsigned int move_index;         
unsigned int move_index = 1;      

void checkBlynkStatus() { // called every 3 seconds by SimpleTimer

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    //Serial.println("Blynk Not Connected");
    digitalWrite(wifiLed, HIGH);
  }
  if (isconnected == true) {
    digitalWrite(wifiLed, LOW);
    //Serial.println("Blynk Connected");
  }
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(VPIN_BUTTON_1);
  Blynk.syncVirtual(VPIN_BUTTON_2);
  Blynk.syncVirtual(VPIN_FULL_TANK); // Synchronize V10 (full tank distance) with the app
  Blynk.syncVirtual(VPIN_EMPTY_TANK); // Synchronize V11 (empty tank distance) with the app
}

void displayData(){
  display.clearDisplay();
   display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Water %:");
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.print(waterLevelPer);
  display.print(" ");
  display.print("%");
  display.setTextSize(1);
  display.setCursor(0,25);
  display.print(currMode);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(60, 0);
  display.print("Battery %:");
  display.setTextSize(1);
  display.setCursor(60, 10);
  display.print(bat_percentage);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(1);
  display.print("%");
  display.display();
  
}

void measureDistance(){
  // Set the trigger pin LOW for 2uS
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
 
  // Set the trigger pin HIGH for 20us to send pulse
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(20);
 
  // Return the trigger pin to LOW
  digitalWrite(TRIGPIN, LOW);
 
  // Measure the width of the incoming pulse
  duration = pulseIn(ECHOPIN, HIGH);
  
  distance = ((duration / 2) * 0.343)/10;

  if (distance > (fullTankDistance - 10)  && distance < emptyTankDistance ){
    waterLevelPer = map((int)distance ,emptyTankDistance, fullTankDistance, 0, 100);
    Blynk.virtualWrite(VPIN_BUTTON_1, waterLevelPer);
    Blynk.virtualWrite(VPIN_BUTTON_2, (String(distance) + " cm"));

  displayData();
  delay(100);
  }
   if (newFullTankDistance > 0 && newEmptyTankDistance > 0) {
    fullTankDistance = newFullTankDistance;
    emptyTankDistance = newEmptyTankDistance;
    newFullTankDistance = 0;
    newEmptyTankDistance = 0;
  }
}

void setup() {
    // Set up serial monitor
  Serial.begin(9600);
 
  // Set pinmodes for sensor connections
  pinMode(ECHOPIN, INPUT);
  pinMode(TRIGPIN, OUTPUT);
  pinMode(wifiLed, OUTPUT);
  
  digitalWrite(wifiLed, HIGH);
   
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(1000);  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();

  WiFi.begin(ssid, pass);
  timer.setInterval(2000L, checkBlynkStatus); // check if Blynk server is connected every 2 seconds
  timer.setInterval(1000L,  measureDistance); // measure water level every 1 seconds
  Blynk.config(auth);
  delay(1000);

  Serial.println();
  gps_module.begin(GPSBaud);
  timer.setInterval(5000L, checkGPS);
}

void checkGPS(){
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
      Blynk.virtualWrite(V8, "GPS ERROR");  
  }
}

 void loop() {
  int sensorValue = analogRead(analogInPin);
  voltage = (((sensorValue * 3.3) / 1024) * 2 + calibration); //multiply by two as voltage divider network is 100K & 100K Resistor

  bat_percentage = mapfloat(voltage, 2.8, 4.2, 0, 100); //2.8V as Battery Cut off Voltage & 4.2V as Maximum Voltage

  if (bat_percentage >= 100)
  {
    bat_percentage = 100;
  }
  if (bat_percentage <= 0)
  {
    bat_percentage = 1;
  }
  
  // send battery percentage to Blynk
  Blynk.virtualWrite(V9, bat_percentage); // V9 is the virtual pin you can use in your Blynk app

  Serial.print("Analog Value = ");
  Serial.println(sensorValue);
  Serial.print("Output Voltage = ");
  Serial.println(voltage);
  Serial.print("Battery Percentage = ");
  Serial.println(bat_percentage);

  Serial.println();
  Serial.println("****************************");
  Serial.println();
  delay(1000);
 
  if (bat_percentage <= 30)
  {
    Serial.println("Battery level below 30%, Charge battery on time");
    // You can add additional actions here when the battery level is low.
    delay(500);
  }
  while (gps_module.available() > 0) 
  {
    //displays information every time a new sentence is correctly encoded.
    if (gps.encode(gps_module.read()))
    displayInfo();
  }
  Blynk.run();
  timer.run(); // Initiates SimpleTimer
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void displayInfo()
{
  if (gps.location.isValid()) 
  {
    //Storing the Latitude. and Longitude
    float latitude = (gps.location.lat());
    float longitude = (gps.location.lng()); 
    
    //Send to Serial Monitor for Debugging
    Serial.print("LAT:  ");
    Serial.println(latitude, 6);  // float to x decimal places
    Serial.print("LONG: ");
    Serial.println(longitude, 6);
    
    Blynk.virtualWrite(V3, String(latitude, 6));   
    Blynk.virtualWrite(V4, String(longitude, 6));  
    myMap.location(move_index, latitude, longitude, "GPS_Location");
    
    //get speed
    gps_speed = gps.speed.kmph();
    Blynk.virtualWrite(V5, gps_speed);
       
    //get number of satellites
    no_of_satellites = gps.satellites.value();
    Blynk.virtualWrite(V6, no_of_satellites);

    // get the satellite orientation/direction
    satellite_orientation = TinyGPSPlus::cardinal(gps.course.value());
    Blynk.virtualWrite(V7, satellite_orientation);
  }
  
  
  Serial.println();
}

BLYNK_WRITE(VPIN_FULL_TANK) {
  int newFullTankDistance = param.asInt(); // Retrieve the value sent from the Blynk app
  // Optionally, add validation here to ensure the received value is within an acceptable range.
  fullTankDistance = newFullTankDistance; // Update the full tank distance
}

BLYNK_WRITE(VPIN_EMPTY_TANK) {
  int newEmptyTankDistance = param.asInt(); // Retrieve the value sent from the Blynk app
  // Optionally, add validation here to ensure the received value is within an acceptable range.
  emptyTankDistance = newEmptyTankDistance; // Update the empty tank distance
}

