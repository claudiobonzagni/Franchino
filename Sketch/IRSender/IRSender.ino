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

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>

#define IR_LED 4  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(IR_LED);  // Set the GPIO to be used to sending the message.

// Example of data captured by IRrecvDumpV2.ino
uint16_t rawData2[67] = {9000, 4500, 650, 550, 650, 1650, 600, 550, 650, 550,
                        600, 1650, 650, 550, 600, 1650, 650, 1650, 650, 1650,
                        600, 550, 650, 1650, 650, 1650, 650, 550, 600, 1650,
                        650, 1650, 650, 550, 650, 550, 650, 1650, 650, 550,
                        650, 550, 650, 550, 600, 550, 650, 550, 650, 550,
                        650, 1650, 600, 550, 650, 1650, 650, 1650, 650, 1650,
                        650, 1650, 650, 1650, 650, 1650, 600};

uint16_t rawData[595] = {3318, 1672,  406, 1234,  406, 468,  406, 470,  406, 468,  404, 470,  432, 444,  432, 444,  432, 446,  406, 468,  406, 470,  406, 468,  406, 468,  406, 1232,  432, 444,  432, 444,  430, 448,  432, 444,  432, 444,  432, 442,  432, 444,  432, 444,  430, 448,  428, 448,  426, 452,  426, 448,  428, 448,  426, 448,  426, 448,  428, 446,  428, 448,  426, 1210,  428, 452,  426, 1212,  426, 1212,  426, 1212,  400, 1238,  404, 1232,  432, 1208,  430, 444,  432, 1210,  432, 1206,  432, 1206,  406, 1232,  430, 1212,  426, 1212,  428, 1210,  400, 1238,  406, 1236,  406, 468,  406, 470,  432, 442,  432, 442,  432, 448,  426, 448,  428, 448,  426, 452,  426, 448,  428, 448,  428, 1210,  428, 1210,  400, 474,  406, 470,  404, 1234,  430, 1210,  432, 1206,  406, 1236,  402, 474,  400, 474,  406, 1232,  430, 1206,  432, 442,  432, 446,  432, 444,  432, 1206,  404, 470,  406, 474,  400, 1236,  406, 470,  404, 470,  432, 1210,  430, 1206,  432, 444,  430, 1208,  404, 1238,  400, 474,  406, 1232,  430, 1206,  432, 448,  432, 1206,  432, 1208,  404, 470,  404, 474,  400, 1236,  406, 470,  406, 468,  406, 472,  406, 470,  406, 468,  432, 1206,  432, 1206,  432, 444,  432, 1206,  432, 1206,  430, 1212,  432, 444,  432, 442,  432, 444,  406, 468,  406, 470,  406, 468,  432, 444,  430, 1212,  404, 1236,  402, 1236,  402, 1238,  406, 1232,  432, 1206,  432, 1206,  432, 1206,  432, 448,  432, 442,  432, 444,  430, 444,  432, 444,  432, 444,  430, 444,  430, 444,  406, 474,  404, 1238,  406, 1232,  406, 1232,  432, 1206,  432, 1206,  432, 1206,  430, 1206,  432, 1212,  404, 474,  402, 474,  406, 468,  406, 470,  406, 468,  432, 444,  430, 444,  432, 448,  432, 1206,  430, 1206,  432, 1208,  404, 1238,  400, 1236,  406, 1232,  432, 1206,  432, 1212,  430, 444,  432, 442,  432, 444,  432, 444,  430, 444,  430, 444,  430, 444,  430, 448,  432, 1212,  402, 1236,  406, 1232,  432, 1206,  432, 1206,  432, 1208,  432, 1206,  432, 1212,  430, 444,  432, 444,  432, 442,  406, 472,  400, 474,  406, 468,  402, 474,  406, 474,  404, 1234,  406, 1232,  430, 1208,  432, 1206,  430, 1208,  432, 1206,  432, 1206,  432, 1210,  432, 442,  432, 444,  432, 444,  430, 444,  432, 444,  406, 470,  404, 470,  406, 474,  406, 1236,  402, 1236,  406, 1234,  430, 1208,  430, 1206,  432, 1206,  432, 1206,  432, 1210,  432, 444,  432, 1206,  432, 1206,  432, 444,  430, 1208,  406, 1236,  402, 474,  400, 478,  400, 1238,  402, 472,  402, 474,  402, 1236,  406, 470,  406, 468,  406, 1232,  432, 1212,  406, 1232,  430, 444,  406, 470,  430, 444,  430, 1208,  430, 1206,  432, 1206,  432, 1212,  430, 444,  406, 1236,  402, 1236,  406, 1232,  430, 444,  432, 444,  432, 442,  432, 448,  432, 444,  432, 442,  432, 444,  432, 442,  432, 444,  430, 444,  432, 444,  430, 448,  406, 1232,  404, 1236,  402, 1236,  406, 1232,  432, 1208,  430, 1206,  432, 1206,  432, 1212,  432, 442,  430, 444,  432, 448,  426, 448,  426, 448,  426, 448,  426, 448,  428, 452,  426, 1212,  426, 1212,  426, 1212,  402, 1236,  406, 1232,  432, 1206,  432, 1206,  432, 1212,  432, 448,  426, 444,  432, 448,  428, 448,  426, 448,  428, 448,  426, 448,  426, 452,  426, 1212,  426, 1210,  428, 1212,  428, 1210,  428, 1210,  426, 1212,  402, 1236,  402, 1240,  402, 1238,  402, 1236,  406, 468,  406, 470,  404, 468,  432, 444,  432, 442,  432, 448,  432, 448,  426, 448,  428, 1210,  428, 1210,  428, 1210,  400, 1236,  406, 1232,  432, 1210,  402};  // UNKNOWN 62E04776

void setup() {
  irsend.begin();
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
}

void loop() {

   Serial.println("mio");
  irsend.sendNEC(0x62E04776, 298);

  
#if SEND_NEC
  Serial.println("NEC");
  irsend.sendNEC(0x00FFE01FUL, 32);
#endif  // SEND_NEC
  delay(2000);
#if SEND_SONY
  Serial.println("Sony");
  irsend.sendSony(0xa90, 12, 2);
#endif  // SEND_SONY
  delay(2000);
#if SEND_RAW
  Serial.println("a rawData capture from IRrecvDumpV2");
  irsend.sendRaw(rawData, 595, 38);  // Send a raw data capture at 38kHz.
#endif  // SEND_RAW
  delay(2000);
}
