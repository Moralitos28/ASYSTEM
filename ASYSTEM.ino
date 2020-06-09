
#include "screen.h"
//#include "filesystem.h"
#include "cardreader.h"
#include "config.h"
#include  "control_tower.h"

// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <SPI.h>
#include "RTClib.h"
RTC_DS3231 rtc;

void BTERROR(int error, boolean forever = true, byte rep = 1) {
  tell(F("page 1"));
  tell("typ.txt=\"ERR" + String(error) + "\"");
  do {
    for (int i = 0; i <= rep; i++) {
      digitalWrite(p_buzzer, HIGH);
      delay(100);
      digitalWrite(p_buzzer, LOW);
      delay(1000);
    }
  } while (forever);
  tell(F("page 2"));
}

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

  Serial.begin(9600);//Establecer instancia de puerto serial.
  if (!Inicializar_TFT()) { //TFT
    delay(250);
    setup();
    Serial.println("ERROR B0");
  }

  ADJ_PGR(10);
  if (!InicializeWiFi()) { //SD
    BTERROR(1);
  }
  ADJ_PGR(25);
  if (!Inicialize_CardReader()) { //CR
    BTERROR(2);
  }
  ADJ_PGR(50);
  if (!rtc.begin()) { //RTC
    BTERROR(3);
  }
  if (rtc.lostPower()) { //RTC

    BTERROR(4);
  }
  ADJ_PGR(75);
  /*
    if (!Inicialize_FS()) { //FILE SYSTEM
    BTERROR(5);
    }*/
  ADJ_PGR(100);
  Serial.println("Start Success!");
  tell(F("page 2"));

}

void ADJ_PGR(int porcentage) { // ACTUALIZAR EL PORCENTAJE E ARRANQUE
  tell("PB0.val=" + porcentage);
}

void loop() {
  String responce = ReadCard();
  if (responce != "") {
    responce = RegisterTag(responce);
    if (responce.charAt(0) == 'F') {
      //Serial.println("ERROR TAG NOT FOUND");
      BTERROR(5, false, 2);
    } else if (responce.charAt(0) == 'S') {
      //Serial.println("Server Error");
      BTERROR(6, false, 3);

    } else if (responce.charAt(0) == 'C') {
      tell(F("page 3"));
      responce.remove(1, 1);
      //Serial.println("Registed: " + responce);
      digitalWrite(p_buzzer, HIGH);
      delay(500);
      digitalWrite(p_buzzer, LOW);
      tell("t1.txt=\"" + responce + "\"");
      tell(F("vis t1,1"));
      delay(1000);
      tell(F("page 2"));
    } else {
      BTERROR(7, false, 5);
    }
    responce = "";
  }
  delay(10);
  UpdateTime();
}

void UpdateTime() {
  DateTime now = rtc.now();
  tell("hr.val=" + String(now.hour(), DEC));
  tell("mnt.val=" + String(now.minute(), DEC));
  /* String longd = String(daysOfTheWeek[now.dayOfTheWeek()]) + ' ' + String(now.day(), DEC) + " de " + String(now.month(), DEC) + " del " +String(now.year(), DEC)+'"';
    tell("long.txt=\"" + longd);*/
}
