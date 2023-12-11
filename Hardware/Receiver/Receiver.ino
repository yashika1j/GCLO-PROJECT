/*
 * Subscribe Tech Vegan for More Hardware & Software Tech Videos
 * Program by: Ashish Vegan https://www.ashishvegan.com
 * For Help - Contact WhatsApp +919890345539
 * technologyvegan@gmail.com
 * Get Harware & Software Academic Projects
 * Code Courtesy: Lora.h
Youtube video link: https://www.youtube.com/watch?v=wH483V8fnN8
 */
#include <LoRa.h>

#define SS 15
#define RST 16
#define DIO0 2
// Library for 1.3 inch OLED display
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

//define OLED address and size
#define Address 0x3C
#define height 64
#define width 128
#define reset -1
//pass above parameters
Adafruit_SH1106G display = Adafruit_SH1106G(width, height, &Wire, reset);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Receiver Host");

  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa Error");
    while (1);
  }
// OLED display code start
   delay(250);
   display.begin(Address,true);
  delay(250);
  display.display();
  delay(2000);



}
void loop() {
   //Calls LoRa.parsePacket() to check if a LoRa packet has been received.
    // It returns the size of the received packet in bytes. If no packet is
    //  received, it returns 0.
  int packetSize = LoRa.parsePacket();
  // If a packet is received (packetSize is not zero), the code enters the block.
  if (packetSize) {
    // Prints "Receiving Data: " to the serial monitor.
    Serial.print("Receiving Data: ");
    // Enters a loop to read the available data from the LoRa module using LoRa.available()
    while (LoRa.available()) {
      // Reads the data as a string using LoRa.readString()
     String data = LoRa.readString(); //  "LoRa" is a class and "readString" is an object of this class to read data coming from transmitter
      // Prints the received data to the serial monitor.
      Serial.println(data);
      displayData(data);

    }
  }
}

void displayData(String data){
  
//   // clear  buffer
  display.clearDisplay();
   delay(500);
  display.setTextSize(1);
  display.setCursor(0,1);
  display.setTextColor(SH110X_WHITE);
  display.println("Sender: ");
  display.print(data);
  display.display();
  delay(2000);
  display.clearDisplay();
}
