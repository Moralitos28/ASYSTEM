#include <SPI.h>
#include <MFRC522.h>
#include "config.h"
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key;

// Init array that will store new NUID
//byte nuidPICC[4];

bool Inicialize_CardReader() {
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  return true;
}

String ReadCard() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return "";

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial()) {
    return "";
  } 
  String responce = "";
   rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
    for (byte i = 0; i < 4; i++) {
    responce += String(rfid.uid.uidByte[i], HEX);
  }
  responce.toUpperCase();
  return responce;
}
