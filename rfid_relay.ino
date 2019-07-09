/*
   ----------------------------------------------------------------------------
   This sketch uses the MFRC522 library ; see https://github.com/miguelbalboa/rfid
   for further details and other examples.

   NOTE: The library file MFRC522.h has a lot of useful info. Please read it.

   This sketch show a simple locking mechanism using the RC522 RFID module.
   ----------------------------------------------------------------------------
   Typical pin layout used:
   -----------------------------------------------------------------------------------------
               MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
               Reader/PCD   Uno           Mega      Nano v3    Leonardo/Micro   Pro Micro
   Signal      Pin          Pin           Pin       Pin        Pin              Pin
   -----------------------------------------------------------------------------------------
   RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
   SPI SS      SDA(SS)      10            53        D10        10               10
   SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
   SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
   SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15

*/
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above

const int rs = 8, en = 9, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

String read_rfid;                   // Add how many you need and don't forget to include the UID.
String ok_rfid = "508f4f73";      // This is for my main RFID Card. aka. The one I will be using to turn on my PC. Can also be used to shut it down if you want to.
int lock = 7;                       // For the ON.
int x =0 ;
/*
   Initialize.
*/
void setup() {
  Serial.begin(9600);         // Initialize serial communications with the PC
  while (!Serial);            // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();                // Init SPI bus
  mfrc522.PCD_Init();         // Init MFRC522 card

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");

  //Choose which lock below:
  pinMode(lock, OUTPUT);

}
/*
   Helper routine to dump a byte array as hex values to Serial.
*/
void dump_byte_array(byte *buffer, byte bufferSize) {
  read_rfid = "";
  for (byte i = 0; i < bufferSize; i++) {
    read_rfid = read_rfid + String(buffer[i], HEX);
  }
}

void loop() {

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return;

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
    return;

  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println(read_rfid);
  x = digitalRead(lock);
  if (read_rfid == ok_rfid)
  {

    switch (x) {
      case 0:
        digitalWrite(lock, HIGH);
        Serial.print("BIKE START");
        break;
      case 1:
        digitalWrite(lock, LOW);
        Serial.print("BIKE START");
        break;

      
    }
  }
  else
  { lcd.setCursor(0, 1);
    lcd.write("taha");
    Serial.print("Unauthorized Person");
  }
}
