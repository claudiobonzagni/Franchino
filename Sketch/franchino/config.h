// use physical switches
#define USE_SWITCHES     true

// debug output (enable/disable)
//#define DEBUG_MODE
//#ifdef DEBUG_MODE
  //#define DEBUG_PRINT(x)  Console.print(x)
  //#define DEBUG_PRINTLN(x)  Console.println(x)

  #define DEBUG_PRINT(x)  Serial.print(x);      // go to the fourth row
  #define DEBUG_PRINTLN(x)   Serial.println(x); 
  
//#else
//  #define DEBUG_PRINT(x)
//  #define DEBUG_PRINTLN(x)
//#endif

// outputs, digital PINs connected to the relays
#define OUT_1              7
#define OUT_2              6
#define OUT_3              5
//#define OUT_4              4


// switches, analog PINs that read the 3-way panel switches
#define SWITCH_1           5
#define SWITCH_2           4
#define SWITCH_3           3
//#define SWITCH_4           4

//#define SWITCH_1           0
//#define SWITCH_2           1
//#define SWITCH_3           2
//#define SWITCH_4           3
// rain sensor, digital PIN connected to the rain sensor
// (basically a 2-way switch, normally closed)
#define RAIN_SENSOR        10

// constants for switch positions
#define SWITCH_MANUAL_OFF  0
#define SWITCH_AUTO        1
#define SWITCH_MANUAL_ON   2

// constants for rain sensor status
#define NOT_RAINING        0
#define RAINING            1

