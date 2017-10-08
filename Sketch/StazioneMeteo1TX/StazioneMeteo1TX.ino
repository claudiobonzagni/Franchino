
/*  CLIENTnt 234  Connects to the home WiFi network
 *  Asks some network parameters
 *  Sends and receives message from the server in every 2 seconds
 *  Communicates: wifi_server_01.ino
 */ 
#include <SPI.h>

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#include <OneWire.h>
#include <DallasTemperature.h>
 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET LED_BUILTIN //4
Adafruit_SSD1306 display(OLED_RESET);

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

#define SERIAL_DEBUG // Uncomment this to dissable serial debugging

byte ledPin = 2;
char ssid[] = "ASUS";           // SSID of your home WiFi
char pass[] = "EABE577DA8";            // password of your home WiFi

unsigned long askTimer = 0;

IPAddress server(192,168,178,224);       // the fix IP address of the server
WiFiClient client;


const char *host = "192.168.178.185";
const char *hostUrl = "//sd/irrighino/php/log-temp.php";

float temperature;
const char *meteoStation = "1";

// 3600000 millisecondi in un'ora
// 5000 millisecondi tra una lettura e l'altra
// / (3600000 / 5000) = 720
int timerMax = 360; //720
int timerCount = timerMax;

bool connectedToWiFI = false;

void setup() {
  

#ifdef SERIAL_DEBUG
  Serial.begin(115200);
  delay(200);
  Serial.println();
#endif

  // per default by default viene impostata una tensione ionterna di 3.3V
  display.begin(SSD1306_SWITCHCAPVCC);

  // cancelliamo lo schermo
  display.clearDisplay();

  // impostiamo il colore a bianco (in questo tipo di dsiplay possimo impostare solo bianco e nero)
  display.setTextColor(WHITE);

  // fissiamo la dimensione del testo
  display.setTextSize(3);
  
 // Start up the library
  sensors.begin();

  delay(10); // short delay
  
  WiFi.begin(ssid, pass);             // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
/*  Serial.println("Connected to wifi");
  Serial.print("Status: "); Serial.println(WiFi.status());    // Network parameters
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());*/
  pinMode(ledPin, OUTPUT);
}

void loop () {
 

  
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  Serial.print("Temperature for the device 1 (index 0) is: ");
 temperature =sensors.getTempCByIndex(0);
  
  Serial.println(temperature);

  display.setCursor(0, 5);
  //display.print("T: ");
  display.print(temperature);
  display.print(" C");
  display.display();
  display.clearDisplay();
  
  //INVIO
 client.connect(server, 80);   // Connection to the server
  digitalWrite(ledPin, LOW);    // to show the communication only (inverted logic)
  Serial.println(".");
  client.println(temperature);  // sends the message to the server
  String answer = client.readStringUntil('\r');   // receives the answer from the sever
  Serial.println("from server: " + answer);
  client.flush();
  digitalWrite(ledPin, HIGH);
  
 
   Serial.println("temperatora inviata: ");
   Serial.println((temperature));
  if (timerCount>timerMax) {
    // Salvo la temperatura in MySql
    postData();

    timerCount=0;
    }
  
  timerCount = timerCount +1;
  Serial.println("timerCount: ");
  Serial.println(timerCount);

  delay(5000);                  // client will trigger the communication after two seconds

}

bool postData()
{

  WiFiClient client;
  if (client.connect(host, 80))
  {
    Serial.println("connection OK");

    // We now create a URI for the request
    String url = hostUrl;

    url += "?temp=" + String(temperature, 2) + "&v=" + meteoStation;

    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0)
    {
      if (millis() - timeout > 5000)
      {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return true;
      }
    }

    // Read all the lines of the reply from server and print them to Serial
    while (client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");
  }

  else
  {
    Serial.println("connection failed");
    return false;
  }
}

void debug(String message)
{
#ifdef SERIAL_DEBUG
  Serial.print(message);
#endif
}

void debugln(String message)
{
#ifdef SERIAL_DEBUG
  Serial.println(message);
#endif
}

void debugln()
{
#ifdef SERIAL_DEBUG
  Serial.println();
#endif
}
