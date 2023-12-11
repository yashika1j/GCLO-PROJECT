// Library for GPS Module
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
// Library for LoRa
#include <LoRa.h>
// Pin define for LoRa
#define SS 15  // ss -> chip select pin

#define RST 16  // reset pin
#define DIO0 2  // thi is an interrupt pin
// Library for 1.3 inch OLED display
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define Buzzer 13

//define OLED address and size
#define Address 0x3C
#define height 64
#define width 128
#define reset -1
// Pin define for GPS Module
const int GPS_TX = D3;
const int GPS_RX = D4;
//
//double lati, longi;
SoftwareSerial myGPS(GPS_TX, GPS_RX);
TinyGPSPlus gps;
//pass above parameters
Adafruit_SH1106G display = Adafruit_SH1106G(width, height, &Wire, reset);
double prevLatitude = 28.677765;
double prevLongitude = 77.494630;
double totalDistance = 0.0;
double latitude = 0.0, longitude = 0.0;

void setup() {
  Serial.begin(9600);
  myGPS.begin(9600);  // initiate GPS serial communication

  while (!Serial)
    ;
  Serial.println("Sender Host");
  LoRa.setPins(SS, RST, DIO0);     //Sets up the pin configuration for the LoRa module using LoRa.setPins()
  if (!LoRa.begin(433E6)) {        //Initializes the LoRa module using LoRa.begin(). The parameter 433E6 sets the frequency to 433 MHz.
    Serial.println("LoRa Error");  //If the LoRa module fails to initialize, the code prints "LoRa Error" and enters an infinite loop.
    delay(100);
    while (1)
      ;
  }

  // OLED display code start
  delay(250);
  display.begin(Address, true);
  delay(250);
  display.display();
  delay(2000);
}

void loop() {
  
  gpsLocation();
  getDistance();
  //  lora();
  // displayData();
}

void gpsLocation() {
  while (myGPS.available() > 0) {
    if (gps.encode(myGPS.read())) {
      if (gps.location.isValid()) {

        latitude = gps.location.lat();
        longitude = gps.location.lng();

        Serial.print("Latitiude: ");
        Serial.print(latitude, 6);
        Serial.print(", Longitude: ");
        Serial.print(longitude, 6);
        Serial.println("");
        // Display Latitude and Longitude over LED
        display.clearDisplay();
        delay(500);
        display.setCursor(0, 0);
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.setTextColor(SH110X_WHITE);
        display.print("Lati: ");
        display.print(latitude, 6);
        display.println("");
        // Display Longitude
        display.setCursor(0, 15);
        display.print("Longi: ");
        display.print(longitude, 6);
        display.println("");
        display.display();
        delay(1500);
        display.clearDisplay();

        LoRa.beginPacket();
        // LoRa send data
        LoRa.print("Lat: ");
        LoRa.print(latitude,6);
        LoRa.print("\n");
        LoRa.print("Long: ");
        LoRa.print(longitude,6);
        LoRa.print("\n");
        LoRa.endPacket();
      }
    }
  }
  // to check GPS is connected properly or not
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
    while (true)
      ;
  }
}


void getDistance() {
  //        convert current coordinates into radians
  double rLatitude = radians(latitude);
  double rLongitude = radians(longitude);
  // previous and fixed coordinates and convert into radians
  double rprevLatitude = radians(prevLatitude);
  double rprevLongitude = radians(prevLongitude);

  //        find difference
  double dLat = rLatitude - rprevLatitude;
  double dLon = rLongitude - rprevLongitude;
  // apply sine formulal̥l̥
  double a = pow(sin(dLat / 2), 2) + cos(rprevLatitude) * cos(rLatitude) * pow(sin(dLon / 2), 2);
  double c = 2 * atan2(sqrt(a), sqrt(1 - a));

  const double radius = 6371000;  // Earth's radius in meters
  totalDistance = radius * c;
  Serial.println(totalDistance);

  // check if troops in the zone or not
  if (totalDistance > 30) {
    digitalWrite(Buzzer, 1);
    Serial.println("Out of zone!");
  } else {
    digitalWrite(Buzzer, 0);
  }

  LoRa.beginPacket();
  // LoRa send data
  LoRa.print("Dis: ");
  LoRa.print(totalDistance);
  LoRa.print("\n");
  LoRa.endPacket();
}

// LoRa send data
/*
void lora(){
  LoRa.beginPacket();
  LoRa.print(latitude);
  LoRa.print(longitude);
  LoRa.endPacket();
  delay(1000);
}

 //Function to display data 
void displayData(){
  
  // displayData();
//    clear  buffer
  display.clearDisplay();
   delay(500);
  display.setTextSize(1);
  display.setCursor(0,1);
  display.setTextColor(SH110X_WHITE);
  display.println("Lati: ");
  display.print(latitude);
  display.display();
  delay(2000);
  display.clearDisplay();
}
*/