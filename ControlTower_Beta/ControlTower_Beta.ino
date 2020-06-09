#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
 
#include "config.h"  // Sustituir con datos de vuestra red
#include "API.hpp"
#include "ESP8266_Utils.hpp"
char message[100];

void setup() 
{
   Serial.begin(115200);
   Serial.setTimeout(1000);
   ConnectWiFi_STA();
   Serial.println("OK");
}
 
void loop() 
{
  if(Serial.available() > 0){
    Serial.readBytesUntil('\n',message,100);
    switch (message[0]){
      case 'R':
      message[0] = '=';
      Report();
      break;
    }
    memset(message, 0, 100);
  }
delay(100);
}


void Report(){
  HTTPClient http;
  http.begin(ApiHost +"/marca?id"+String(message));
  Serial.println(ApiHost +"/marca?id"+String(message));
  int httpCode = http.sendRequest("POST");
  if(httpCode == 201){
      String payload = http.getString();
      Serial.println('C'+payload);
  } else if (httpCode == 400){
    Serial.println('F');
  } else{
    Serial.println('S');
  }
  http.end();
}
