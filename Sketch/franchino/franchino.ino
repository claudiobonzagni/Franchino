// irrighino firmware
// www.lucadentella.it

#define FIRMWARE_VERSION "1.1"

// library includes
#include <Bridge.h>
#include <Process.h>
#include <YunServer.h>
#include <YunClient.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <Process.h>

Process date;                 // process used to get the date
//int dates, month, years, hours, minutes;  // for the results
int hours, minutes, seconds;
int lastSecond = -1;          // need an impossible value for comparison
int lastMinutes = -1;

// inizializzo LCD
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// custom includes
#include "config.h"
#include "debug_functions.c"

// inizializzo sensore della temperatura
 #define ONE_WIRE_BUS 8 //ds18b20 module attach to pin8
/* Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs) */
 OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
 DallasTemperature sensors(&oneWire);

// global variables
YunServer server;
int switch1Position;
int switch2Position;
int switch3Position;

bool rainSensorStatus;


void setup() {
  
  // start serial port
  Serial.begin(9600);

  // start clock
 if (!date.running()) {
    date.begin("date");
    date.addParameter("+%T");
    date.run();
  }
 
  lcd.init();  //initialize the lcd
  lcd.backlight();  //open the backlight 
  lcd.setCursor ( 0, 0 );            // go to the top left corner
  lcd.print(" *** Franchino ***    "); // write this string on the top row
  
  DEBUG_PRINT2("Avvio...");
  
  Bridge.begin();
  
  DEBUG_PRINT("Booting irrighino firmware version ");
  DEBUG_PRINTLN(FIRMWARE_VERSION);
  DEBUG_PRINTLN();
  DEBUG_PRINT2("Configuro PINs");
  // configure PINs
  pinMode(OUT_1, OUTPUT);
  pinMode(OUT_2, OUTPUT);
  pinMode(OUT_3, OUTPUT);
 // pinMode(OUT_4, OUTPUT);
//  pinMode(LED_1, OUTPUT);
//  pinMode(LED_2, OUTPUT);
//  pinMode(LED_3, OUTPUT);
 // pinMode(LED_4, OUTPUT); 
  pinMode(RAIN_SENSOR, INPUT_PULLUP);
  DEBUG_PRINTLN("- PINs configured");

  updateSwitchPosition(SWITCH_1, SWITCH_MANUAL_ON);
  updateSwitchPosition(SWITCH_2, SWITCH_MANUAL_ON);
  updateSwitchPosition(SWITCH_3, SWITCH_MANUAL_ON);
  
  // default value for variables
  rainSensorStatus = NOT_RAINING;
  switch1Position = SWITCH_AUTO;
  switch2Position = SWITCH_AUTO;
  switch3Position = SWITCH_AUTO;
  //switch4Position = SWITCH_AUTO;
  DEBUG_PRINTLN("- Global variables initialized");
  DEBUG_PRINT2("Avvio YunServer");
  // initialize the YunServer to receive commands
  server.listenOnLocalhost();
  server.begin();
  DEBUG_PRINTLN("- YunServer started and listening");  
  
  DEBUG_PRINTLN("- Boot ok, running...");
  DEBUG_PRINTLN();
  lcd.print("                   ");
 
}

void loop() {
 
  
  // ****************************************************
  // DATA ED ORA
  // ****************************************************
  if (lastSecond != seconds) { // if a second has passed

    // restart the date process:
    if (!date.running()) {
      date.begin("date");
      date.addParameter("+%T");
      date.run();
    }
  }

  //if there's a result from the date process, parse it:
  while (date.available()>0) {
    // get the result of the date process (should be hh:mm:ss):
    String timeString = date.readString();  
      
   // find the colons:
    int firstColon = timeString.indexOf(":");
    int secondColon = timeString.lastIndexOf(":");

    // get the substrings for hour, minute second:
    String hourString = timeString.substring(0, firstColon);
    String minString = timeString.substring(firstColon + 1, secondColon);
    String secString = timeString.substring(secondColon + 1);

    // convert to ints,saving the previous second:
    hours = hourString.toInt();
    minutes = minString.toInt();
    lastSecond = seconds;          // save to do a time comparison
    
    seconds = secString.toInt();

    //lcd.print("                   ");
    lcd.setCursor(0, 0);
    lcd.print(timeString);
   
    if (lastMinutes != minutes) { // if a minute has passed
       // ****************************************************
        // TEMPERATURA
        // ****************************************************
      
        // request to all devices on the bus
        Serial.print("Requesting temperatures...");
        sensors.requestTemperatures(); // Send the command to get temperatures
        Serial.println("DONE");
        // After we got the temperatures, we can print them here.
        // We use the function ByIndex, and as an example get the temperature from the first sensor only.
        Serial.print("Temperature for the device 1 (index 0) is: ");
        Serial.println(sensors.getTempCByIndex(0));  
        lcd.setCursor(10, 0);
        lcd.print(" T: ");
        lcd.print(sensors.getTempCByIndex(0));
   }
   
   lastMinutes = minutes;          // save to do a time comparison
    
  }
  
  // ---------- CHECK PANEL SWITCHES ----------
  if(USE_SWITCHES) {
     
    int actualSwitchPosition;
    actualSwitchPosition = readSwitchStatus(SWITCH_1);
    if(actualSwitchPosition != switch1Position) {
      updateSwitchPosition(SWITCH_1, actualSwitchPosition);
      switch1Position = actualSwitchPosition;
    }
  
    actualSwitchPosition = readSwitchStatus(SWITCH_2);
    if(actualSwitchPosition != switch2Position) {
      updateSwitchPosition(SWITCH_2, actualSwitchPosition);
      switch2Position = actualSwitchPosition;
    }
    actualSwitchPosition = readSwitchStatus(SWITCH_3);
    if(actualSwitchPosition != switch3Position) {
      updateSwitchPosition(SWITCH_3, actualSwitchPosition);
      switch3Position = actualSwitchPosition;
    }
 
  }
  
  // ---------- CHECK RAIN SENSOR ----------
  // bool actualRainSensorStatus = digitalRead(RAIN_SENSOR);
  
  // if the new status if different from the previous one, debounce
 //  if(actualRainSensorStatus != rainSensorStatus) {
    
 //    delay(50);
 //    actualRainSensorStatus = digitalRead(RAIN_SENSOR);
    
    // if it's different again, notify
 //    if(actualRainSensorStatus != rainSensorStatus) {
      
  //     DEBUG_PRINT("Rain sensor status changed to --> ");
  //     DEBUG_PRINTLN(getRainSensorText(actualRainSensorStatus));
      
  //     rainSensorStatus = actualRainSensorStatus;
  //     updateRainSensorStatus();  
   //  }
   //}
  
 
   // ****************************************************
  // VISUALIZZO STATO RELE
  // ****************************************************
  
 
 
  lcd.setCursor(0, 1);
  lcd.print(getOutputDescription2(OUT_1));
  lcd.print(readReleStatus(OUT_1));
  lcd.print(readSwitchStatusDesc(switch1Position));
  
  lcd.setCursor(0, 2);
  lcd.print(getOutputDescription2(OUT_2));
  lcd.print(readReleStatus(OUT_2));
  lcd.print(readSwitchStatusDesc(switch2Position));
  
  lcd.setCursor(0, 3);
  lcd.print(getOutputDescription2(OUT_3));
  lcd.print(readReleStatus(OUT_3));
  lcd.print(readSwitchStatusDesc(switch3Position));
  
//  lcd.print(getOutputDescription2(OUT_4));
//  lcd.print(readReleStatus(OUT_4));
  
  // ---------- CHECK YunServer CONNECTIONS ----------
  YunClient client = server.accept(); 
  if (client) {
    String command = client.readStringUntil('/');
    int parameter = client.parseInt();
    DEBUG_PRINT("-> Command ");
    DEBUG_PRINT(command);
    DEBUG_PRINT(" with parameter ");
    Serial.println("pippo");
    DEBUG_PRINTLN(parameter);
    
    String retCode;
    if(command == "on") retCode = executeOutputCommand(parameter, false);
    else if(command == "off") retCode = executeOutputCommand(parameter, true);
    else retCode = "KO_UNKNOWN";
    client.println(retCode);
    client.stop();
  }
}

void DEBUG_PRINT2(String x) {
  lcd.setCursor ( 0, 3 );      
  lcd.print("-> " + x);
}

// Read the switch position
int readSwitchStatus(int switchId) {
  
  int analogValue, actualStatus;
  
  analogValue = analogRead(switchId);
  if(analogValue < 100) actualStatus = SWITCH_MANUAL_ON;
  else if(analogValue < 900) actualStatus = SWITCH_MANUAL_OFF;
  else actualStatus = SWITCH_AUTO;
  
  return actualStatus;
}

// Update the switch position
void updateSwitchPosition(int switchId, int switchPosition) {

  DEBUG_PRINT("-> Switch ");
  DEBUG_PRINT(switchId);
  DEBUG_PRINT(" moved to position ");
  DEBUG_PRINTLN(getSwitchPositionText(switchPosition));
  
  if(switchPosition == SWITCH_MANUAL_OFF) setOutput(getSwitchOutput(switchId), false);
  else if(switchPosition == SWITCH_MANUAL_ON) setOutput(getSwitchOutput(switchId), true);
}


// Turn output (and corresponding led) ON/OFF
void setOutput(int outputId, bool outputStatus) {
 // digitalWrite(rele, LOW);
  digitalWrite(outputId, outputStatus);
 // digitalWrite(getOutputLed(outputId), outputStatus);

  Serial.println("outputId:");
//Serial.println(outputId);
//Serial.println(getOutputLed(outputId));
  DEBUG_PRINT("-> Output ");
  DEBUG_PRINT(getOutputDescription(outputId));
  DEBUG_PRINT(" set to ");
  DEBUG_PRINTLN(getOutputStatusText(outputStatus));
}


// Execute a command on the outputs received from YunClient
String executeOutputCommand(int outId, bool outputStatus) {
  
  // check if the switch is in AUTO position
  int outputId = getOutputId(outId);
  int switchId = getOutputSwitch(outputId);
  if(getSwitchStatus(switchId) == SWITCH_AUTO) {
    setOutput(outputId, outputStatus);
    return "OK";
  } else return "KO_MANUAL";
}


char* readReleStatus(int outId) {
  // ****************************************************
  // LEGGO LO STATO DEI RELE
  // ****************************************************
  char* releStatus1;
  
  int valRele = digitalRead(outId);
  if (valRele == HIGH) {
      releStatus1 = ": CHIUSA ";
    }
    else {
       releStatus1 = ": APERTA ";
    }
  
  return  releStatus1;
 
}


char* readSwitchStatusDesc(int outId) {
  // ****************************************************
  // LEGGO LO STATO DEI PULSANTI
  // ****************************************************
  char* releSwitch1;
  
  
  if (outId == SWITCH_MANUAL_OFF) {
      releSwitch1 = "  - ON   ";
    }
   if (outId == SWITCH_AUTO) {
      releSwitch1 = "  - AUTO";
    }
  if (outId == SWITCH_MANUAL_ON) {
      releSwitch1 = "  - OFF  ";
    }
    
  return  releSwitch1;
 
}

// Get the output PIN that corresponds to an outId (server-side)
int getOutputId(int outId) {
  
  if(outId == 0) return OUT_1;
  if(outId == 1) return OUT_2;
  if(outId == 2) return OUT_3;  
 // if(outId == 3) return OUT_4;   
}

// Get the output that corresponds to a switch
int getSwitchOutput(int switchId) {

  if(switchId == SWITCH_1) return OUT_1;
  if(switchId == SWITCH_2) return OUT_2;
  if(switchId == SWITCH_3) return OUT_3;
//  if(switchId == SWITCH_4) return OUT_4;   
}

// Get the switch that corresponds to an output
int getOutputSwitch(int outputId) {

  if(outputId == OUT_1) return SWITCH_1;
  if(outputId == OUT_2) return SWITCH_2;
  if(outputId == OUT_3) return SWITCH_3; 
 // if(outputId == OUT_4) return SWITCH_4;
}



// Get the switch status
int getSwitchStatus(int switchId) {

  if(switchId == SWITCH_1) return switch1Position;
  if(switchId == SWITCH_2) return switch2Position;
  if(switchId == SWITCH_3) return switch3Position;
  //if(switchId == SWITCH_4) return switch4Position;
}


// update rain sensor status
void updateRainSensorStatus() {

  Process p;            
  p.begin("php-cli");      
  p.addParameter("/www/sd/irrighino/php/updateRainSensorStatus.php");
  p.addParameter(String(rainSensorStatus));
  p.run();
  
  DEBUG_PRINTLN("Status update sent");
  while (p.available()>0) {
    char c = p.read();
    DEBUG_PRINT(c);
  }
  DEBUG_PRINTLN();
}


// print debug information

void printDebugInfo() {

  DEBUG_PRINT("irrighino firmware version: ");
  DEBUG_PRINTLN(FIRMWARE_VERSION);
  DEBUG_PRINTLN();

  DEBUG_PRINT("Switch 1 position: ");
  DEBUG_PRINTLN(getSwitchPositionText(switch1Position));  
  DEBUG_PRINT("Switch 2 position: ");
  DEBUG_PRINTLN(getSwitchPositionText(switch2Position));  
  DEBUG_PRINT("Switch 3 position: ");
  DEBUG_PRINTLN(getSwitchPositionText(switch3Position));
 // DEBUG_PRINT("Switch 4 position: ");
 // DEBUG_PRINTLN(getSwitchPositionText(switch4Position));  
 // DEBUG_PRINTLN();
  
  DEBUG_PRINT("Rain sensor status: ");
  DEBUG_PRINTLN(getRainSensorText(rainSensorStatus));
}
