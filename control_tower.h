#include <SoftwareSerial.h>
#include "config.h"

SoftwareSerial CT(ct_rx, ct_tx); // RX, TX WIFI MODULE
//char responce[100];

String ReadUntil() {
  delay(100);
  bool tatus = true;
  String responce;
  byte trys;
  while (tatus) {
    char thechar;
    if (CT.available()) {
      thechar = CT.read();
      if (thechar != '\n') {
        responce += thechar;
        trys = 0;
      } else {
        tatus = false;
      }
    } else {
      if(trys >= 100){
      tatus = false;
      } else{
        delay(25);
        trys++;
      }
    }
  }
  while(CT.available())
  CT.read();
  return responce;
}

bool InicializeWiFi() {
  CT.begin(9600);
  CT.println('H');
//  String responce = ReadUntil();
//  Serial.println("THE RESPONCE: "+responce);
  if(ReadUntil().startsWith("OK")){ 
  return 1;
  }
  return 0;
}

String RegisterTag(String responce) {
  responce = 'R' + responce + '\n';
  CT.print(responce);
  responce = ReadUntil();
  return responce;
}
