
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11
#define DHT11_PIN 2
#define MOISTURE_PIN 0

DHT dht(DHT11_PIN, DHTTYPE); 

#include <ESP8266WiFi.h>


const char* ssid = ""; //ssid network name
const char* password = ""; //network password

WiFiServer server(80);

void setup()
{
  dht.begin();
  Serial.begin(115200); //Default Baudrate
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(LED_BUILTIN, HIGH); // Turn LED OFF
  Serial.print("Connecting to the Newtork");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected"); 
  server.begin();  // Starts the Server
  Serial.println("Server started");
 
  Serial.print("IP Address of network: "); // will IP address on Serial Monitor
  Serial.println(WiFi.localIP());
  Serial.print("Copy and paste the following URL: https://"); // Will print IP address in URL format
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
 
void loop()
{
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }
  Serial.println("Waiting for new client");
  while(!client.available())
  {
    delay(1);
  }
 
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
//*------------------HTML Page Code---------------------*//
 
  digitalWrite(LED_BUILTIN, LOW);// Turn LED ON
  client.println("HTTP/1.1 200 OK"); //
  client.println("Content-Type: text/html");
  client.println("");
  client.println("{");
  client.println("\"Air_Humidity\": "+GetAir_Humidity()+",");
  client.println("\"Temperature\": "+GetTemperature()+",");
  client.println("\"Luminosity\": "+GetLuminosity()+",");
  client.println("\"Soil_PercentHumidity\": "+GetSoil_PercentHumidity());
  client.println("}");

 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
  delay(200);
  digitalWrite(LED_BUILTIN, HIGH); // Turn LED OFF
}
//int humidity = 0;
String GetAir_Humidity()
{
  //humidity = humidity+1;
  //humidity = random(0, 100);
  //return String(humidity%100);
  return String(dht.readHumidity());
}
//int temp=0;
String GetTemperature()
{
  //temp = temp+1;
  //temp = random(0, 50);
  //return String(temp%40);
  return String(dht.readTemperature());
}
int luminosity = 50;
String GetLuminosity()
{
  luminosity = random(0, 100);
  return String(luminosity);
} 

String GetSoil_PercentHumidity()
{
    int PinValor;
    PinValor = analogRead(MOISTURE_PIN);   //1024 -> 3.3V  
     // The higher the number read from the PinValor, the lower the humidity.
     // Therefore, the percentage of humidity is calculated by:
     //      
     //   Valor lido                 Percent humidity
     //      _    0                           _ 100
     //      |                                |   
     //      |                                |   
     //      -   PinValor                     - PercentHumidity
     //      |                                |   
     //      |                                |   
     //     _|_  1024                         _|_ 0
     //
     //   (moisture-0) / (100-0)  =  (PinValor - 1024) / (-1024)
     //      Logo:
     //      moisture = 100 * ((1024-PinValor) / 1024)  
        
     float PercentHumidity = 100 * ((1024-(float)PinValor) / 1024); 
     return String(PercentHumidity);
}
