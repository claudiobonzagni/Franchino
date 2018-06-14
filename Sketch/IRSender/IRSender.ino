/* IRremoteESP8266: IRsendDemo - demonstrates sending IR codes with IRsend.
 *
 * Version 1.0 April, 2017
 * Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009,
 * Copyright 2009 Ken Shirriff, http://arcfn.com
 *
 * An IR LED circuit *MUST* be connected to the ESP8266 on a pin
 * as specified by IR_LED below.
 *
 * TL;DR: The IR LED needs to be driven by a transistor for a good result.
 *
 * Suggested circuit:
 *     https://github.com/markszabo/IRremoteESP8266/wiki#ir-sending
 *
 * Common mistakes & tips:
 *   * Don't just connect the IR LED directly to the pin, it won't
 *     have enough current to drive the IR LED effectively.
 *   * Make sure you have the IR LED polarity correct.
 *     See: https://learn.sparkfun.com/tutorials/polarity/diode-and-led-polarity
 *   * Typical digital camera/phones can be used to see if the IR LED is flashed.
 *     Replace the IR LED with a normal LED if you don't have a digital camera
 *     when debugging.
 *   * Avoid using the following pins unless you really know what you are doing:
 *     * Pin 0/D3: Can interfere with the boot/program mode & support circuits.
 *     * Pin 1/TX/TXD0: Any serial transmissions from the ESP8266 will interfere.
 *     * Pin 3/RX/RXD0: Any serial transmissions to the ESP8266 will interfere.
 *   * ESP-01 modules are tricky. We suggest you use a module with more GPIOs
 *     for your first time. e.g. ESP-12 etc.
 */
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

ESP8266WebServer server(80);

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>

#define IR_LED 4  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(IR_LED);  // Set the GPIO to be used to sending the message.

// Example of data captured by IRrecvDumpV2.ino
uint16_t rawDataON[595] = {3318, 1672,  406, 1234,  406, 468,  406, 470,  406, 468,  404, 470,  432, 444,  432, 444,  432, 446,  406, 468,  406, 470,  406, 468,  406, 468,  406, 1232,  432, 444,  432, 444,  430, 448,  432, 444,  432, 444,  432, 442,  432, 444,  432, 444,  430, 448,  428, 448,  426, 452,  426, 448,  428, 448,  426, 448,  426, 448,  428, 446,  428, 448,  426, 1210,  428, 452,  426, 1212,  426, 1212,  426, 1212,  400, 1238,  404, 1232,  432, 1208,  430, 444,  432, 1210,  432, 1206,  432, 1206,  406, 1232,  430, 1212,  426, 1212,  428, 1210,  400, 1238,  406, 1236,  406, 468,  406, 470,  432, 442,  432, 442,  432, 448,  426, 448,  428, 448,  426, 452,  426, 448,  428, 448,  428, 1210,  428, 1210,  400, 474,  406, 470,  404, 1234,  430, 1210,  432, 1206,  406, 1236,  402, 474,  400, 474,  406, 1232,  430, 1206,  432, 442,  432, 446,  432, 444,  432, 1206,  404, 470,  406, 474,  400, 1236,  406, 470,  404, 470,  432, 1210,  430, 1206,  432, 444,  430, 1208,  404, 1238,  400, 474,  406, 1232,  430, 1206,  432, 448,  432, 1206,  432, 1208,  404, 470,  404, 474,  400, 1236,  406, 470,  406, 468,  406, 472,  406, 470,  406, 468,  432, 1206,  432, 1206,  432, 444,  432, 1206,  432, 1206,  430, 1212,  432, 444,  432, 442,  432, 444,  406, 468,  406, 470,  406, 468,  432, 444,  430, 1212,  404, 1236,  402, 1236,  402, 1238,  406, 1232,  432, 1206,  432, 1206,  432, 1206,  432, 448,  432, 442,  432, 444,  430, 444,  432, 444,  432, 444,  430, 444,  430, 444,  406, 474,  404, 1238,  406, 1232,  406, 1232,  432, 1206,  432, 1206,  432, 1206,  430, 1206,  432, 1212,  404, 474,  402, 474,  406, 468,  406, 470,  406, 468,  432, 444,  430, 444,  432, 448,  432, 1206,  430, 1206,  432, 1208,  404, 1238,  400, 1236,  406, 1232,  432, 1206,  432, 1212,  430, 444,  432, 442,  432, 444,  432, 444,  430, 444,  430, 444,  430, 444,  430, 448,  432, 1212,  402, 1236,  406, 1232,  432, 1206,  432, 1206,  432, 1208,  432, 1206,  432, 1212,  430, 444,  432, 444,  432, 442,  406, 472,  400, 474,  406, 468,  402, 474,  406, 474,  404, 1234,  406, 1232,  430, 1208,  432, 1206,  430, 1208,  432, 1206,  432, 1206,  432, 1210,  432, 442,  432, 444,  432, 444,  430, 444,  432, 444,  406, 470,  404, 470,  406, 474,  406, 1236,  402, 1236,  406, 1234,  430, 1208,  430, 1206,  432, 1206,  432, 1206,  432, 1210,  432, 444,  432, 1206,  432, 1206,  432, 444,  430, 1208,  406, 1236,  402, 474,  400, 478,  400, 1238,  402, 472,  402, 474,  402, 1236,  406, 470,  406, 468,  406, 1232,  432, 1212,  406, 1232,  430, 444,  406, 470,  430, 444,  430, 1208,  430, 1206,  432, 1206,  432, 1212,  430, 444,  406, 1236,  402, 1236,  406, 1232,  430, 444,  432, 444,  432, 442,  432, 448,  432, 444,  432, 442,  432, 444,  432, 442,  432, 444,  430, 444,  432, 444,  430, 448,  406, 1232,  404, 1236,  402, 1236,  406, 1232,  432, 1208,  430, 1206,  432, 1206,  432, 1212,  432, 442,  430, 444,  432, 448,  426, 448,  426, 448,  426, 448,  426, 448,  428, 452,  426, 1212,  426, 1212,  426, 1212,  402, 1236,  406, 1232,  432, 1206,  432, 1206,  432, 1212,  432, 448,  426, 444,  432, 448,  428, 448,  426, 448,  428, 448,  426, 448,  426, 452,  426, 1212,  426, 1210,  428, 1212,  428, 1210,  428, 1210,  426, 1212,  402, 1236,  402, 1240,  402, 1238,  402, 1236,  406, 468,  406, 470,  404, 468,  432, 444,  432, 442,  432, 448,  432, 448,  426, 448,  428, 1210,  428, 1210,  428, 1210,  400, 1236,  406, 1232,  432, 1210,  402};  // UNKNOWN 62E04776

uint16_t rawDataOFF[595] = {3346, 1642,  430, 1210,  428, 448,  426, 448,  402, 474,  400, 474,  402, 474,  400, 474,  400, 478,  402, 474,  400, 474,  400, 474,  406, 468,  406, 1236,  426, 448,  426, 448,  426, 452,  426, 448,  428, 448,  426, 448,  428, 448,  426, 448,  426, 448,  428, 446,  428, 452,  426, 448,  428, 446,  428, 448,  426, 448,  402, 474,  400, 474,  400, 1238,  400, 478,  402, 1236,  406, 1236,  428, 1210,  426, 1212,  428, 1210,  428, 1210,  426, 448,  426, 1216,  426, 1210,  402, 1236,  402, 1236,  400, 1236,  430, 1212,  426, 1212,  428, 1210,  428, 1216,  426, 448,  426, 448,  428, 446,  402, 474,  400, 474,  400, 474,  400, 474,  402, 476,  402, 474,  400, 474,  406, 1232,  432, 1210,  428, 448,  428, 448,  426, 1210,  428, 1214,  428, 1212,  426, 1212,  400, 474,  400, 474,  402, 1236,  406, 1236,  426, 448,  428, 452,  428, 448,  426, 1212,  426, 448,  426, 448,  428, 1212,  426, 448,  400, 474,  402, 1242,  402, 1236,  400, 474,  402, 1238,  402, 1238,  400, 474,  402, 1236,  402, 1238,  402, 478,  400, 1238,  400, 1238,  406, 468,  406, 470,  406, 1232,  432, 444,  432, 448,  426, 448,  432, 444,  432, 448,  426, 1212,  426, 1212,  426, 448,  428, 1210,  428, 1212,  426, 1216,  400, 474,  402, 472,  402, 472,  402, 474,  400, 474,  402, 472,  400, 474,  406, 1238,  406, 1232,  430, 1208,  430, 1212,  428, 1210,  428, 1210,  428, 1210,  428, 1210,  428, 452,  428, 448,  426, 448,  426, 448,  428, 448,  400, 474,  402, 474,  400, 474,  402, 478,  400, 1236,  406, 1232,  432, 1210,  428, 1210,  428, 1212,  426, 1210,  428, 1210,  428, 1216,  426, 448,  428, 448,  426, 448,  426, 448,  428, 448,  428, 446,  428, 448,  428, 452,  428, 1210,  402, 1236,  402, 1238,  400, 1236,  406, 1234,  406, 1232,  406, 1236,  428, 1210,  432, 448,  426, 448,  428, 448,  426, 448,  428, 448,  426, 448,  428, 448,  428, 452,  426, 1212,  428, 1210,  402, 1236,  406, 1232,  432, 1210,  428, 1210,  426, 1210,  428, 1214,  428, 448,  400, 474,  402, 474,  400, 474,  400, 474,  402, 474,  400, 474,  404, 474,  406, 1232,  432, 1210,  426, 1212,  426, 1212,  426, 1212,  426, 1210,  400, 1238,  404, 1238,  406, 468,  432, 444,  430, 448,  428, 448,  426, 448,  428, 448,  426, 448,  428, 452,  428, 1210,  428, 1210,  426, 1212,  400, 1238,  400, 1238,  406, 1232,  404, 1234,  430, 1212,  430, 448,  428, 1210,  428, 1210,  426, 448,  426, 1212,  400, 1236,  400, 474,  406, 474,  406, 1234,  430, 442,  432, 442,  432, 1206,  432, 448,  426, 448,  428, 1210,  428, 1216,  426, 1212,  426, 448,  426, 448,  428, 446,  428, 448,  426, 1212,  428, 1210,  402, 1242,  400, 474,  402, 1236,  402, 1236,  406, 1232,  406, 1232,  406, 470,  432, 444,  430, 448,  432, 442,  432, 442,  432, 448,  428, 448,  426, 448,  428, 448,  426, 448,  428, 452,  428, 1210,  428, 1210,  428, 1210,  426, 1212,  400, 1238,  406, 1232,  432, 1206,  432, 1210,  432, 444,  430, 444,  406, 470,  406, 472,  402, 474,  402, 474,  400, 474,  402, 478,  400, 1236,  406, 1232,  406, 1234,  430, 1206,  432, 1206,  430, 1206,  432, 1206,  432, 1210,  432, 444,  432, 442,  432, 444,  432, 444,  430, 444,  432, 444,  432, 444,  430, 448,  432, 1206,  432, 1210,  402, 1238,  404, 1232,  406, 1234,  404, 1234,  430, 1206,  432, 1212,  432, 1206,  432, 1206,  432, 442,  432, 444,  432, 444,  430, 444,  432, 442,  430, 448,  432, 448,  426, 448,  428, 1210,  428, 1210,  402, 1236,  400, 1238,  404, 1232,  406, 1236,  402};  // UNKNOWN D68BD4F2


byte ledPin = 2;
char ssid[] = "ASUS";       // SSID of your home WiFi
char pass[] = "EABE577DA8"; // password of your home WiFi



void setup() {
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  delay(10); // short delay

   WiFi.begin(ssid, pass);  //Connect to the WiFi network
 
    while (WiFi.status() != WL_CONNECTED) {  //Wait for connection
 
        delay(500);
        Serial.println("Waiting to connect...");
 
    }
 
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  //Print the local IP
 
    server.on("/body", handleBody); //Associate the handler function to the path
 
    server.begin(); //Start the server
    Serial.println("Server listening");
  irsend.begin();
  
}

void loop() {


  server.handleClient(); //Handling of incoming requests

char JSONMessage[] = " {\"SensorType\": \"Temperature\", \"Value\": 10}"; //Original message
  delay(2000);
}


void handleBody() { //Handler for the body path
 
      if (server.hasArg("plain")== false){ //Check if body received
 
//            server.send(200, "text/plain", "Body not received");
           // return;
 
      }
 DynamicJsonBuffer  JSONBuffer;   //Memory pool
  JsonObject& parsed = JSONBuffer.parseObject(server.arg("plain")); //Parse message
 
  if (!parsed.success()) {   //Check for errors in parsing
 
    Serial.println("Parsing failed");
    delay(5000);
    return;
 
  }
 
  const char * sensorType = parsed["device"]; //Get sensor type value
                                     //Get value of sensor measurement
 
  Serial.println(sensorType);


 
      String message = "Body received:\n";
             message += server.arg("plain");
             message += "\n";
 
      server.send(200, "text/plain", message);
      Serial.println(message);
}
bool SendON()
{
/*
Invio IR ON
*/
 Serial.println("Send ON");
  irsend.sendRaw(rawDataON, 595, 38);  // Send a raw data capture at 38kHz.
}


bool SendOFF()
{
/*
Invio IR OFF
*/
   Serial.println("Send OFF");
  //irsend.sendRaw(rawDataON, 595, 38);  // Send a raw data capture at 38kHz.
}
