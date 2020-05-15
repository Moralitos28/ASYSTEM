#include <SoftwareSerial.h>
SoftwareSerial Screen(2, 3); // RX, TX TFT SCREEN

void tell(String message) {
  Screen.print(message);
  Screen.write(0xff);
  Screen.write(0xff);
  Screen.write(0xff);
  delay(1);
}
String getit() {
  String message = "";
  while (Screen.available()) {
    int incomingByte = Screen.read();
    if (incomingByte == 0) {
      message += F("00");
    } else {
      message += String(incomingByte, HEX);
    }

  }
  message.toUpperCase();
  if (message == "" || !message.endsWith("FFFFFF")) {
    return message + F("E:RP");
  }
  else {
    return message;
  }
}

bool Inicializar_TFT() {
  Screen.begin(9600);// INICIANDO CONEXION CON EL TFT
  delay(50);
  tell(F("rest"));
  delay(4000);
  getit();
  tell(F("sendme"));
  if (getit().endsWith(F("6600FFFFFF"))) {
    return true;
  } else {
    return false;
  }
}
