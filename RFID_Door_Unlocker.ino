/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read new NUID from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * Example sketch/program showing how to the read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
 * Reader on the Arduino SPI interface.
 * 
 * When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
 * then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
 * you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
 * will show the type, and the NUID if a new card has been detected. Note: you may see "Timeout in communication" messages
 * when removing the PICC from reading distance too early.
 * 
 * @license Released into the public domain.
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno           Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

//
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

int pos = 0;  
int flag = 0;

#define SS_PIN 10
#define RST_PIN 9

Servo myservo;
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[3];

void setup() { 
  myservo.attach(8);//servo at pin 8
  //
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  Serial.println(F("#38-305 Dormitory Lock system running"));
//  for (byte i = 0; i < 6; i++) {
//    key.keyByte[i] = 0xFF;
//  }
//
//  Serial.println(F("This code scan the MIFARE Classsic NUID."));
//  Serial.print(F("Using the following key:"));
//  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
}
 
void loop() {

  //permitted card flag
  flag = 0;
  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

//  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
//    rfid.uid.uidByte[1] != nuidPICC[1] || 
//    rfid.uid.uidByte[2] != nuidPICC[2] || 
//    rfid.uid.uidByte[3] != nuidPICC[3] ) {
//    Serial.println(F("hex block one is: "));
//    Serial.println(rfid.uid.uidByte[0]);  
//    
//    Serial.println(F("A new card has been detected."))
//    ;
//
//    // Store NUID into nuidPICC array
//    for (byte i = 0; i < 4; i++) {
//      nuidPICC[i] = rfid.uid.uidByte[i];
//    }
//   
//    Serial.println(F("The NUID tag is:"));
//    Serial.print(F("In hex: "));
//    printHex(rfid.uid.uidByte, rfid.uid.size);
//    Serial.println();
//    Serial.print(F("In dec: "));
//    printDec(rfid.uid.uidByte, rfid.uid.size);
//    Serial.println();
//  }
//  else Serial.println(F("Card read previously."));

  Serial.println(F("Card dectected"));
    Serial.println(F("The NUID tag is:"));
    Serial.print(F("In hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    Serial.print(F("In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    
  if ( (rfid.uid.uidByte[0] == 89 && rfid.uid.uidByte[1] == 168 && rfid.uid.uidByte[2] == 176) || (rfid.uid.uidByte[0] == 89 && rfid.uid.uidByte[1] == 35 && rfid.uid.uidByte[2] == 229 ) ||
       (rfid.uid.uidByte[0] == 39 && rfid.uid.uidByte[1] == 108 && rfid.uid.uidByte[2] == 229) )
  //card1: 89 168 176 51 lvzhichao
  //card2: 89 35 229 51 liuyang
  //card3: 39 108 229 249 linyuhao
    {
      flag = 1;
      Serial.println(F("This is my Card"));
    }
    
  if (flag)
  {
        Serial.println(F("Lock opened"));
        for (pos = 0; pos <= 180; pos += 1)
        { // goes from 0 degrees to 180 degrees
          // in steps of 1 degree
          myservo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(15);                       // waits 15ms for the servo to reach the position
        }
        
        delay (5000);
        for (pos = 180; pos >= 0; pos -= 1) 
        { // goes from 180 degrees to 0 degrees
          myservo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(15);                       // waits 15ms for the servo to reach the position
        }
        Serial.println(F("Locked"));
  }
   else 
   {
      Serial.println(F("This card not my card"));
    }


  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}


/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}
