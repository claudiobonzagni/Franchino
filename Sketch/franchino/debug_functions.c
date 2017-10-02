#include "config.h"

// get descriptive text for costants

char* getSwitchPositionText(int switchPosition) {
  
  if(switchPosition == 2) return "MANUAL OFF";
  if(switchPosition == 1) return "AUTO"; 
  if(switchPosition == 0) return "MANUAL_ON";  
}

char* getOutputStatusText(int outputStatus) {
  
  if(outputStatus == 0) return "ON";
  if(outputStatus == 1) return "OFF";  
}

char* getRainSensorText(int sensorStatus) {
  
  if(sensorStatus == 0) return "NOT_RAINING";
  if(sensorStatus == 1) return "RAINING";  
}

char* getOutputDescription(int outputId) {

  if(outputId == OUT_1) return "SUD V1";
  if(outputId == OUT_2) return "NORD V2";
  if(outputId == OUT_3) return "OVEST V3";
  //if(outputId == OUT_4) return "REAR";
}

char* getOutputDescription2(int outputId) {

  if(outputId == OUT_1) return "1";
  if(outputId == OUT_2) return "2";
  if(outputId == OUT_3) return "3";
 // if(outputId == OUT_4) return "4";
}
