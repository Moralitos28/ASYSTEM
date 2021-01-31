#include "cardreader.h"
#include "config.h"
#include  "control_tower.h"

#include <SPI.h>

void setup() {
  SPI.begin();          // Init SPI bus
  pinMode(p_buzzer, OUTPUT);
  delay(500);
  for (int i = 0; i < 2; i++) {
    digitalWrite(p_buzzer, HIGH);
    delay(250);
    digitalWrite(p_buzzer, LOW);
    delay(250);
  }

  
 // Serial.begin(9600);//Establecer instancia de puerto serial.
      //Serial.println("STRTP");

  if( !Inicialize_CardReader()){
      //Serial.println("FCRD");
  }

    if( !InicializeWiFi()){
      //Serial.println("FCT");
      while(!InicializeWiFi()){
        delay(1000);
      }
  }
  
  //Serial.println("Start Success!");

}


void loop() {
  String responce = "";
   responce = ReadCard();
  if (responce != "") {
    //Serial.println(responce);
    responce = RegisterTag(responce);
    //Serial.println(responce);
    responce = ReadUntil();
    //Serial.println("RESPONCE WAS: "+ responce);
    if (responce.charAt(0) == 'F') {
      //Serial.println("ERROR TAG NOT FOUND");
      digitalWrite(p_buzzer, HIGH);
      delay(1500);
      digitalWrite(p_buzzer, LOW);
      
    } else if (responce.charAt(0) == 'S') {
      //Serial.println("Server Error: " + responce);
      

    } else if (responce.charAt(0) == 'C') {
      
      responce.remove(1, 1);
      //Serial.println("Registed: " + responce);
      digitalWrite(p_buzzer, HIGH);
      delay(500);
      digitalWrite(p_buzzer, LOW);
      digitalWrite(p_door, HIGH);
      delay(3500);
      digitalWrite(p_door, LOW);
      //Serial.println("t1.txt=\"" + responce + "\"");
      delay(1000);
    } else {
      //Serial.println("ERROR");
    }
   responce = "";
  }
  delay(100);
  //UpdateTime();
}
