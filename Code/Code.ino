#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

//Variables
int i = 0;
int statusCode;
const char* ssid = "Yedida";
const char* password = "Balahari689$";
const char* softAp = "ESP-IoT";
const char* softApPw = "homeautomation";
String st;
String content;

int reayInput = 2;

ESP8266WebServer server(80);

void setup(){
  Serial.begin(115200); //Initializing if(DEBUG)Serial Monitor
  Serial.println();
  Serial.println("Disconnecting previously connected WiFi");
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  delay(100);
  for (int i = 0; i < 96; ++i) {
          EEPROM.write(i, 0);
        }

  if(TestWiFiConnection(ssid,password)){
      pinMode(LED_BUILTIN, OUTPUT);
      WiFi.begin(ssid,password);
      delay(500);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
    } else{
       Serial.println("Reading data from EEPROM");
       EEPROM.begin(512); //Initializing EEPROM
       delay(10);

       Serial.println("Reading EEPROM ssid");
       String essid = ReadDataFromEeprom(0);
       String epass = ReadDataFromEeprom(32);
       Serial.print("SSID: ");
       Serial.println(essid);
       Serial.print("PASSWORD: ");
       Serial.println(epass);
       
       if(TestWiFiConnection(essid,epass)){
          WiFi.begin(essid,epass);
          delay(100);
          Serial.print("IP address: ");
          Serial.println(WiFi.localIP());
       }else{
          ScanWiFiNetworks();
          if(WiFi.softAP(softAp, softApPw)){
            Serial.println("Initializing_Wifi_accesspoint");
            Serial.print("IP address:\t");
            Serial.println(WiFi.softAPIP()); 
            delay(500);
            createWebServer();
            server.begin();
            Serial.println("Server started");
            while ((WiFi.status() != WL_CONNECTED))
              {
                Serial.print(".");
                delay(100);
                server.handleClient();
              }
            digitalWrite(LED_BUILTIN,LOW);
            WiFi.softAPdisconnect(true);
            delay(5000);
            Serial.println("Disabling_Wifi_accesspoint");
          }else{
            Serial.println("Failed to create Wifi_accesspoint");
          }
       }

  }
}


void loop(){
}
