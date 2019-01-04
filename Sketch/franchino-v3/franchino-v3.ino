// franchino firmware
// https://github.com/claudiobonzagni/

#define FIRMWARE_VERSION "3.0"

// library includes
#include <Bridge.h>
#include <Process.h>
#include <YunServer.h>
#include <YunClient.h>
#include <Process.h>

// custom includes
#include "config.h"
#include "debug_functions.c"

// global variables
YunServer server;

Process date; // process used to get the date
//int dates, month, years, hours, minutes;  // for the results
int hours, minutes, seconds;
int lastSecond = -1; // need an impossible value for comparison
int lastMinutes = -1;

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

  DEBUG_PRINT2("Avvio ver: ");

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
    String hourString = timeString.substring(firstColon - 2, firstColon);
    String minString = timeString.substring(firstColon + 1, secondColon);
    String secString = timeString.substring(secondColon + 1);

    // convert to ints,saving the previous second:
    hours = hourString.toInt();
    minutes = minString.toInt();
    lastSecond = seconds; // save to do a time comparison
    seconds = secString.toInt();

    //get date
    String monthString = timeString.substring(firstColon2 - 2, firstColon2);
    String dayString = timeString.substring(firstColon2 + 1, secondColon2);
    String yearString = timeString.substring(secondColon2 + 1);

    Serial.println(dayString);
    Serial.println(monthString);
    Serial.println(yearString);



    lastMinutes = minutes; // save to do a time comparison
  }
}
// ****************************************************
// FUNZIONI
// ****************************************************
void DEBUG_PRINT2(String x)
{
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
