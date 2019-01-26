/*  SERVER: 224    Connects to the home WiFi network
 *  Asks some network parameters
 *  Starts WiFi server with fix IP and listens
 *  Receives and sends messages to the client
 *  Communicates: wifi_client_01.ino
 *
 *  SERVER: 192.168.178.224
 *  CLIENT TX 1: 192.168.178.234
 *  CLIENT TX 3: 192.168.178.236
 *  CLIENT TX 4: 192.168.178.228
 *
 */
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

//const char *host = "192.168.178.185";
//const char *hostUrl = "//sd/irrighino/php/log-temp.php";

const char *host = "franchino.azurewebsites.net";
const char *hostUrl = "/php/log-temp.php";

float temperature;

String temperatureLast1;
String temperatureLast2;
String temperatureLast3;
String temperatureLast4;
int tempLogCount =1;

float temperatureRemote;
String temperatureRemote1String;
String temperatureRemote3String;
String temperatureRemote4String;
IPAddress clientIPAddress;
const char *meteoStation = "2"; // Identificativo della stazione meteo
String valueToPrint;
//bool temperatureExt = false;
int meteoToShow = 1;

// 3600000 millisecondi in un'ora
// 5000 millisecondi tra una lettura e l'altra
// / (3600000 / 5000) = 720
int timerMax = 25; //720
int timerCount = 0;

byte ledPin = 2;
char ssid[] = "ASUS";       // SSID of your home WiFi
char pass[] = "EABE577DA8"; // password of your home WiFi
WiFiServer server(80);
 

  
void setup()
{

#ifdef SERIAL_DEBUG
  Serial.begin(115200);
  delay(200);
  ////Serial.println();
#endif

  ////Serial.println("setup");
 // WiFi.config(ip, gateway, subnet);       // forces to use the fix IP

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
 //WiFi.config(ip,  gateway, subnet);
  WiFi.begin(ssid, pass); // connects to the WiFi router
  ////Serial.println("WiFi.begin");

  while (WiFi.status() != WL_CONNECTED)
  {
    //Serial.print(".");
    delay(500);
  }
  server.begin(); // starts the server

  // Visualizzo informazioni
  ////Serial.println("Connected to wifi");
  //Serial.print("Status: ");
  ////Serial.println(WiFi.status()); // some parameters from the network
  //Serial.print("IP: ");
  ////Serial.println(WiFi.localIP());
  //Serial.print("Subnet: ");
  ////Serial.println(WiFi.subnetMask());
  //Serial.print("Gateway: ");
  ////Serial.println(WiFi.gatewayIP());
  //Serial.print("SSID: ");
  ////Serial.println(WiFi.SSID());
  //Serial.print("Signal: ");
  ////Serial.println(WiFi.RSSI());
  //Serial.print("Networks: ");
  ////Serial.println(WiFi.scanNetworks());

  pinMode(ledPin, OUTPUT);
}

void loop()
{

  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  //Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  ////Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  //Serial.print("Temperature for the device 1 (index 0) is: ");
  temperature = sensors.getTempCByIndex(0);
  temperatureLast2=String(temperature, 2);
  ////Serial.println(temperature);

  // Avvio server
  WiFiClient client = server.available();
  if (client)
  {
    if (client.connected())
    {
      digitalWrite(ledPin, LOW); // to show the communication only (inverted logic)
      ////Serial.println(".");
      String request = client.readStringUntil('\r'); // receives the message from the client

      clientIPAddress = client.remoteIP(); // Leggo IP del client

      //Serial.print("From client: ");
      ////Serial.println(clientIPAddress);
      ////Serial.println(request);

      // Controllo il client dall'IP
      if (clientIPAddress == IPAddress(192, 168, 178, 234))
      {
        temperatureRemote1String = request;
        temperatureLast1=request;
        //Serial.print("From meteo 1 (Esterno): ");
      }
      else
      {
        if (clientIPAddress == IPAddress(192, 168, 178, 228))
        {
          // Casetta
          temperatureRemote4String = request;
          temperatureLast4=request;
          //Serial.print("From meteo 4 (Casetta): ");
        }
        else
        {
          temperatureRemote3String = request;
          temperatureLast3=request;
          //Serial.print("From meteo 3 (Superiore): ");
        }
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

  if (meteoToShow == 1)
  {
    valueToPrint = temperatureRemote1String + " E"; // Esterno
  }

  if (meteoToShow == 2)
  {
    valueToPrint = String(temperature, 2) + " I"; // Interno inferiore
  }

  if (meteoToShow == 3)
  {
    valueToPrint = temperatureRemote3String + " S"; // Interno superiore
  }

  if (meteoToShow == 4)
  {
    valueToPrint = temperatureRemote4String + " C"; // Casetta
  }

  if (meteoToShow > 4)
  {
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

  ////Serial.println("meteoToShow: ");
  ////Serial.println(meteoToShow);

  delay(1000); // client will trigger the communication after two seconds
}

bool postData()
{
  /*
Invio i dati della temperatura al server Arduino Yun (per salvarli nel database SqlLite)
*/

    Serial.println("postData azurewebsites");
WiFiClient client2;

  Serial.printf("\n[Connecting to %s ... ", host);
  if (client2.connect(host, 80))
  {
    Serial.println("connected]");


    // We now create a URI for the request
    String url = hostUrl;

    if (tempLogCount == 1)    {
       url += "?temp=" + temperatureLast1 + "&v=1" ; //Esterno
      }
      else
      {
         if (tempLogCount == 2){
            url += "?temp=" + temperatureLast2 + "&v=2"; //Interno giu
          }
      
         else
          {
             if (tempLogCount == 3){
                url += "?temp=" + temperatureLast3 + "&v=3"; // Interno superiore
              }
               else
                {
                   if (tempLogCount == 4){
                      url += "?temp=" + temperatureLast4 + "&v=0" ; //Casetta
                    }
                  }
               }
            }
  //  url += "?temp=" + String(temperature, 2) + "&v=" + meteoStation;

    //Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    client2.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

  tempLogCount = tempLogCount +1;
  if (tempLogCount  > 3)
   {
    tempLogCount = 1;
  }

  
    unsigned long timeout = millis();
    while (client2.available() == 0)
    {
      if (millis() - timeout > 5000)
      {
        Serial.println(">>> Client Timeout !");
        client2.stop();
        return true;
      }
    }

    // Read all the lines of the reply from server and print them to Serial
    while (client2.available())
    {
      String line = client2.readStringUntil('\r');
      Serial.print(line);
    }

    ////Serial.println();
    ////Serial.println("closing connection");
  }

  else
  {
    Serial.println("connection failed");
    return false;
  }
}
