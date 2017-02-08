/******************************************************************************
ESP32_Water_Sensor_SMS_Example.ino
Example for sending a text message using the ESP32 Thing and IFTTT
Alex Wende @ SparkFun Electronics
February 7th 2017
~

This sketch connects the ESP32 Thing to your local WiFi network and sends a
text message every time the capacitive touch sensor comes in contact with water.

Resources:
WiFi.h (included with the ESP32 board definitions)

Development environment specifics:
Arduino 1.0+
SparkFun ESP32 Thing (https://www.sparkfun.com/products/13907)

This code is beerware; if you see me (or any other SparkFun employee) at
the local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.   
******************************************************************************/

#include <WiFi.h>
WiFiClient client;

const char* myKey = "Maker_Key_Goes_Here";            // Your maker key goes here
const char* ssid = "Network_Name_Goes_Here";          // Your router's ssid goes here
const char* password = "Network_Password_Goes_Here";  // Password to router goes here
const byte  touchPin = 4;                             // IO pin for capacitive touch sensing

boolean oldWaterLevel=0, currentWaterLevel=0;


void setup() {
  Serial.begin(115200); // Start serial communication for debug information
  
  WiFi.mode(WIFI_STA);  // Setup the WiFi radio to connect to a wireless access point
  ConnectWifi();        // Connect to WiFi
}

void loop()
{  
  if(WiFi.status() == WL_CONNECTED) // Make sure we're still connected to our router
  {
    float sensorReading = 0;  // Read the analog value for the touch sensor
    byte sampleSize = 8;      // Number of samples we want to take
    
    for(byte i=0; i<sampleSize; i++)  // Average samples together to minimize false readings
    {
      sensorReading += touchRead(touchPin); // We sample the touch pin here
      delay(10);
    }
    sensorReading /= sampleSize;
    
    if(sensorReading < 5)
    {
      currentWaterLevel = HIGH;
    }
    else
    {
      currentWaterLevel = LOW;
    }
    
    if(oldWaterLevel != currentWaterLevel)      // Water level has changed, send SMS
    {
      Serial.println("connecting...");
      if(client.connect("maker.ifttt.com",80))  // Test the connection to the server
      {
        Serial.println("Connected to Maker");
        sendSMS(currentWaterLevel);             // Request SMS text to the number provided in IFTTT
        oldWaterLevel = currentWaterLevel;      // Update oldWaterLevel to the new water level
      }
      else
      {
        Serial.println("Failed to connect to Maker.");
      }
    }
  }
  else  // Reconnect to wireless access point
  {
    delay(1000);    // Wait 1 second between connection attempts
    ConnectWifi();  // Connect to WiFi
  }
}

bool ConnectWifi()  // Tries to connect to the wireless access point with the credentials provided
{
  bool timeOut = 0; // Change to 1 if connection times out
  byte count = 0;   // Counter for the number of attempts to connect to wireless AP
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password); // Connect to WiFi network

  while (WiFi.status() != WL_CONNECTED && (timeOut == 0)) // Test to see if we're connected
  {
    Serial.print('.');
    count++;
    
    if(count > 60) break; // Give up after ~30 seconds
    else delay(500);      // Check again after 500ms
  }
  
  if (WiFi.status() == WL_CONNECTED)  // We're connected
  {
    Serial.println("\r\nWiFi connected");
  }
  else  // Unable to connect
  {
    WiFi.disconnect();
    Serial.println("\r\nConnection Timed Out!\r\n");
  }
}


void sendSMS(boolean waterLevel)  // Built HTTP packet to send to Maker
{
  // Packet will end up looking similar to this:
  // 
  //  POST /trigger/ESP/with/key/(myKey) HTTP/1.1
  //  Host: maker.ifttt.com
  //  User-Agent: ESP32_Thing
  //  Connection: close
  //  Conent-Type: application/json
  //  Conent-Length: (postData length)
  //  
  //  {"value1":"(HIGH/LOW )"}
  
  String postData = "{\"value1\":\""; // JSON value to send in SMS message
  if(waterLevel) postData.concat("HIGH");
  else postData.concat("LOW ");
  postData.concat("\"}");
    
  client.print("POST /trigger/");
  client.print("ESP");
  client.print("/with/key/");
  client.print(myKey);
  client.println(" HTTP/1.1");

  client.println("Host: maker.ifttt.com");
  client.println("User-Agent: ESP32_Thing");
  client.println("Connection: close");

  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(postData.length());
  client.println();
  client.println(postData);
}

