#include "screen.h"
#include "filesystem.h"
#include "cardreader.h"
// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"
#include <SPI.h>
#define p_buzzer 9
RTC_DS3231 rtc;

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
    BTERROR(0);
  }

  ADJ_PGR(10);
  if (!Inicialize_SD()) { //SD
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
  if (!Inicialize_FS()) { //FILE SYSTEM
    BTERROR(5);
  }
  ADJ_PGR(100);
  tell(F("page 2"));
  
}

void ADJ_PGR(int porcentage) { // ACTUALIZAR EL PORCENTAJE E ARRANQUE
  tell("PB0.val=" + porcentage);
}

void BTERROR(int error) {
  tell(F("page 1"));
  tell("typ.txt=\"BT" + String(error) + "\"");
  while (true) {
    digitalWrite(p_buzzer, HIGH);
    delay(100);
    digitalWrite(p_buzzer, LOW);
    delay(100);
  }
}

void loop() {
  delay(1000);
  UpdateTime();
}

void UpdateTime() {
  DateTime now = rtc.now();
  tell("hr.val=" + String(now.hour(), DEC));
  tell("mnt.val=" + String(now.minute(), DEC));
  /* String longd = String(daysOfTheWeek[now.dayOfTheWeek()]) + ' ' + String(now.day(), DEC) + " de " + String(now.month(), DEC) + " del " +String(now.year(), DEC)+'"';
    tell("long.txt=\"" + longd);*/
}
