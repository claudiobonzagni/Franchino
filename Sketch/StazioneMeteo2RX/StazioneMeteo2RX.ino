/*  SERVER: 224    Connects to the home WiFi network
 *  Asks some network parameters
 *  Starts WiFi server with fix IP and listens
 *  Receives and sends messages to the client
 *  Communicates: wifi_client_01.ino
 *
 *  SERVER: 192.168.178.224
 *  CLIENT TX 1: 192.168.178.234
 *  CLIENT TX 3: 192.168.178.236
 *
 */
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

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

const char *host = "192.168.178.185";
const char *hostUrl = "//sd/irrighino/php/log-temp.php";

float temperature;
float temperatureRemote;
String temperatureRemote1String;
String temperatureRemote3String;
IPAddress  clientIPAddress;
const char *meteoStation = "2"; // Identificativo della stazione meteo
String valueToPrint;
//bool temperatureExt = false;
int meteoToShow = 1;


// 3600000 millisecondi in un'ora
// 5000 millisecondi tra una lettura e l'altra
// / (3600000 / 5000) = 720
int timerMax = 360; //720
int timerCount = timerMax;

byte ledPin = 2;
char ssid[] = "ASUS";       // SSID of your home WiFi
char pass[] = "EABE577DA8"; // password of your home WiFi
WiFiServer server(80);

void setup()
{

#ifdef SERIAL_DEBUG
  Serial.begin(115200);
  delay(200);
  Serial.println();
#endif

  Serial.println("setup");
  //WiFi.config(ip, gateway, subnet);       // forces to use the fix IP

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

  WiFi.begin(ssid, pass); // connects to the WiFi router
  Serial.println("WiFi.begin");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  server.begin(); // starts the server
 
  // Visualizzo informazioni
  Serial.println("Connected to wifi");
  Serial.print("Status: ");
  Serial.println(WiFi.status()); // some parameters from the network
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Subnet: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("Signal: ");
  Serial.println(WiFi.RSSI());
  Serial.print("Networks: ");
  Serial.println(WiFi.scanNetworks());

  pinMode(ledPin, OUTPUT);
}

void loop()
{

  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  Serial.print("Temperature for the device 1 (index 0) is: ");
  temperature = sensors.getTempCByIndex(0);
  Serial.println(temperature);

  // Avvio server
  WiFiClient client = server.available();
  if (client)
  {
    if (client.connected())
    {
      digitalWrite(ledPin, LOW); // to show the communication only (inverted logic)
      Serial.println(".");
      String request = client.readStringUntil('\r'); // receives the message from the client	 
		
	  clientIPAddress = client.remoteIP(); // Leggo IP del client
	  
	  Serial.print("From client: ");
      Serial.println(clientIPAddress);
	  Serial.println(request);
	  
	  // Controllo il client dall'IP
	  if (clientIPAddress ==  IPAddress(192,168,178,234)){
		temperatureRemote1String = request;  
    Serial.print("From meteo 1: ");
	  }
	  else{
		temperatureRemote3String = request;
    Serial.print("From meteo 3: ");
	  }
           
      client.flush();
      client.println("Hi client! No, I am listening.\r"); // sends the answer to the client
      digitalWrite(ledPin, HIGH);
    }
    client.stop(); // tarminates the connection with the client
  }

  display.setCursor(0, 5);

//  if (temperatureExt)
//  {
//    valueToPrint = temperatureRemote1String + " E";
//  }
//  else
//  {
//    valueToPrint = String(temperature, 2) + " I";
//  }
//  temperatureExt = !temperatureExt;

  if (meteoToShow==1){ 
	valueToPrint = temperatureRemote1String + " E"; // Esterno
  }
  
  if (meteoToShow==2){ 
	 valueToPrint = String(temperature, 2) + " I"; // Interno inferiore
  }
  
  if (meteoToShow==3){ 
	 valueToPrint = temperatureRemote3String + " S"; // Interno superiore
  }
  
  if (meteoToShow > 3) {	  
	  meteoToShow = 0; // Reset
  }
  
  display.print(valueToPrint);
  display.display();
  display.clearDisplay();

  if (timerCount > timerMax)
  {
    // Invio i dati della temperatura al server Arduino Yun
    postData();

    timerCount = 0;
  }

  timerCount = timerCount + 1;
  meteoToShow = meteoToShow + 1;
  
  Serial.println("timerCount: ");
  Serial.println(timerCount);

  Serial.println("meteoToShow: ");
  Serial.println(meteoToShow);

  delay(2000); // client will trigger the communication after two seconds
}

bool postData()
{
/*
Invio i dati della temperatura al server Arduino Yun (per salvarli nel database SqlLite)
*/
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
