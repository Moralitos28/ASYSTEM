#include <SoftwareSerial.h>
#include "config.h"

SoftwareSerial CT(ct_rx, ct_tx); // RX, TX WIFI MODULE
char responce[100];
bool InicializeWiFi() {
  CT.begin(115200);
  //CT.println('H');
  delay(100);
  return 1;
}

String RegisterTag(String responce) {
  responce = 'R'+responce+'\n';
  CT.print(responce);
  responce = CT.readString();
  return responce;
}
