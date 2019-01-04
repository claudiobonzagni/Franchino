// franchino firmware
// https://github.com/claudiobonzagni/

#define FIRMWARE_VERSION "2.0"

// library includes
#include <Bridge.h>
#include <Process.h>
#include <YunServer.h>
#include <YunClient.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Process.h>

// custom includes
#include "config.h"
#include "debug_functions.c"

// global variables
YunServer server;


float temperature;
const char *meteoStation = "0";

Process date; // process used to get the date
//int dates, month, years, hours, minutes;  // for the results
int hours, minutes, seconds;
int lastSecond = -1; // need an impossible value for comparison
int lastMinutes = -1;

// ****************************************************
// Inizializzo LCD
// ****************************************************
//
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

// ****************************************************
// Inizializzo sensore della temperatura
// ****************************************************
//
#define ONE_WIRE_BUS 8 //ds18b20 module attach to pin8
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// 3600000 millisecondi in un'ora
// 5000 millisecondi tra una lettura e l'altra
// / (3600000 / 5000) = 720
int timerMax = 10; //720
int timerCount = timerMax;
      
// ****************************************************
// SETUP
// ****************************************************
void setup()
{

  // start serial port
  Serial.begin(9600);



  lcd.init();                          //initialize the lcd
  lcd.backlight();                     //open the backlight
  lcd.setCursor(0, 0);                 // go to the top left corner
  lcd.print("** Franchino v." ); // write this string on the top row
  lcd.print( FIRMWARE_VERSION); // write this string on the top row
 
  DEBUG_PRINT2("Avvio ver: ");
  lcd.print(FIRMWARE_VERSION );

  Bridge.begin();

  DEBUG_PRINT("Booting franchino firmware version ");
  DEBUG_PRINTLN(FIRMWARE_VERSION);
  DEBUG_PRINTLN();
  DEBUG_PRINT2("Configuro PINs");

  DEBUG_PRINTLN("- Global variables initialized");

  // initialize the YunServer to receive commands
  DEBUG_PRINT2("Avvio YunServer");
  server.listenOnLocalhost();
  server.begin();
  DEBUG_PRINTLN("- YunServer started and listening");

  DEBUG_PRINTLN("- Boot ok, running...");
  DEBUG_PRINTLN();
  //lcd.print("                   ");
}

void loop()
{

  // ****************************************************
  // DATA ED ORA
  // ****************************************************
  if (lastSecond != seconds)
  { // if a second has passed

    // restart the date process:
    if (!date.running())
    {
      date.begin("date");
      //date.addParameter("+%T");
      date.addParameter("+%D-%T");
      date.run();
    }
  }

  //if there's a result from the date process, parse it:
  while (date.available() > 0)
  {
    // get the result of the date process (should be hh:mm:ss):
    String timeString = date.readString();
 
Serial.println(timeString);
    // find the colons:
    int firstColon = timeString.indexOf(":");
    int secondColon = timeString.lastIndexOf(":");

    int firstColon2 = timeString.indexOf("/");
    int secondColon2 = timeString.lastIndexOf("/");
    
    // get the substrings for hour, minute second:
    String hourString = timeString.substring(firstColon-2, firstColon);
    String minString = timeString.substring(firstColon + 1, secondColon);
    String secString = timeString.substring(secondColon + 1);

    // convert to ints,saving the previous second:
    hours = hourString.toInt();
    minutes = minString.toInt();
    lastSecond = seconds; // save to do a time comparison
    seconds = secString.toInt();

    //get date
    String monthString = timeString.substring(firstColon2-2, firstColon2);
    String dayString = timeString.substring(firstColon2 + 1, secondColon2);
    String yearString = timeString.substring(secondColon2 + 1);

    Serial.println(dayString);
    Serial.println(monthString);
    Serial.println(yearString);
    
    // timezone
    hours= hours +2;
    
    lcd.setCursor(0, 0);
   // lcd.print(timeString);

    lcd.print("          ");
    lcd.setCursor(0, 0);
    lcd.print(hours);
    lcd.print(":");
    lcd.print(minutes);
    lcd.print(":");
    lcd.print(seconds);
    
    
    if (lastMinutes != minutes)
    { // if a minute has passed
      // ****************************************************
      // TEMPERATURA
      // ****************************************************
      
      // TODO:utilizzare questa per la temp.
      float temp;
      
      // request to all devices on the bus
      Serial.print("Requesting temperatures...");
      sensors.requestTemperatures(); // Send the command to get temperatures
      Serial.println("DONE");
      // After we got the temperatures, we can print them here.
      // We use the function ByIndex, and as an example get the temperature from the first sensor only.
      Serial.print("Temperature for the device 1 (index 0) is: ");
      temperature =sensors.getTempCByIndex(0);
      Serial.println(temperature);
      lcd.setCursor(10, 0);
      lcd.print(" T: ");
      lcd.print(temperature);


      if (timerCount>timerMax) {
        // Salvo la temperatura in MySql
        postData();
    
        timerCount=0;
        }
        
      timerCount = timerCount +1;
      Serial.print(" timerCount ");
      Serial.print(timerCount);        
    }

    lastMinutes = minutes; // save to do a time comparison
  }
 

  
}
// ****************************************************
// FUNZIONI
// ****************************************************
void DEBUG_PRINT2(String x)
{
  lcd.setCursor(0, 3);
  lcd.print("-> " + x);
}

// Read the switch position


bool postData()
{
  Serial.print(" ***   postData *** ");
  Process p;
  p.begin("php-cli");
  p.addParameter("/www/sd/irrighino/php/log-temp.php");
 // p.addParameter("temp=" + String(temperature, 2) + "&v=" + meteoStation);
 p.addParameter("23.25");
 p.addParameter("0");
  p.run();

  DEBUG_PRINTLN("Log Temp update sent");
  while (p.available() > 0)
  {
    char c = p.read();
    DEBUG_PRINT(c);
  }
  DEBUG_PRINTLN();
}


