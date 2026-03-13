#include <MFRC522.h>
#include <SPI.h>
// Questions:
// #define vs const?
//What defines what nbaud rate is used for serial?

// define const pins. 
const  int ss = 7; // slave select pin #
const int rst = 10; //reset pin #'
const int redled = 34; //GPIO 32
const int greenled = 33;

// SCK, MISO, MOSI, CS pins
const int sck = 6;
const int miso = 9;
const int mosi = 8;

// Create object instances

// RFID instance
MFRC522 rfid(ss,rst);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SPI.begin(sck,miso,mosi,ss); 
  rfid.PCD_Init();
  delay(200);
  rfid.PCD_DumpVersionToSerial();
  Serial.println("Hello, ESP32-S2!");

  // LED PINMODES
  pinMode(redled, INPUT_PULLUP);
  pinMode(greenled, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(redled,LOW);
  delay(10); // this speeds up the simulation
}
