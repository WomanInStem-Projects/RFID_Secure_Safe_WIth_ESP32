#include <MFRC522.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// function declarations

// display functions
void blinkled();
void clearDisplay();
void waitForSerial(unsigned long timeout = 10000);


// rfid tag/fob functions
bool cardInacl();
void lookinforcard();

// access control list functions
void addCardtoACL();
void AccessControl();
boolean IsAdmin();
void AccessDenied();
void AccessGranted();


const int redled = 26;
const int greenled = 21;

String pswd ;
String guessed = "";
String cardID = "";
bool pswdset = false;

// oled pins
const int sda = 13;
const int scl = 14;

const int sck = 18;   // ESP32 default SPI clock
const int miso = 19;  // ESP32 default SPI MISO
const int mosi = 23;  // ESP32 default SPI MOSI is 23, 18
const int ss = 5;     // ESP32 default SPI CS
const int rst = 4;   // RFID reset pin
bool cardpresent = false;

String ACL[10]; // access controll list contsining each UI for RFID tag/card
int aclSize = 0;

MFRC522 rfid(ss, rst);
Adafruit_SSD1306 oled(128, 64, &Wire, -1);

void clearDisplay() {
  oled.clearDisplay();
  oled.setCursor(0, 0);  // Reset cursor to top-left
}

void addCardtoACL() {
  //check i card isnt in acl
  if (cardInacl() == false){
  // if its not add it to the acl
  ACL[aclSize] = cardID;
  aclSize++;
  }

  
}

bool cardInacl()
{
  for (int i = 0; i < aclSize; i++) 
  {
      if (ACL[i] == cardID) 
      {
        displayAndSerial("Card already in ACL");
        return true;
      }

      
  }
  return false;
}

void lookinforcard() //loops untill a card is found, if card is found prints to serial
{
  while (true){
    if ( rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) // loop ag
    {
      for (byte i = 0; i < rfid.uid.size; i++) 
      {
        cardID += String(rfid.uid.uidByte[i], HEX);
      }
      cardID.toUpperCase();
      displayAndSerial("Card Detected with UID: " + cardID);
      break; // out of while loop
    }
    
    Serial.println();
    displayAndSerial("No Card Detected - Please Tap Your Card");
    //delay(1000);
  }

  rfid.PICC_HaltA(); // this closed the reader's connection to the card, so we can read another card after this
  delay(1000);  

}

void AccessDenied()
{
  blinkled(redled);
  displayAndSerial("Access Denied");
  //digitalWrite(redled, HIGH);
  //delay(2000);
}

void AccessGranted()
{
  blinkled(greenled);
  displayAndSerial("Access Granted");
  //digitalWrite(greenled, HIGH);
  //delay(2000);
  //digitalWrite(greenled, LOW);
}
void AccessControl() {
  lookinforcard();
  // if card is in ACL, grant access, else deny access

  if (cardInacl() == true) 
  {
    AccessGranted();
  } else {
    AccessDenied();

    displayAndSerial("Would you like to add this card to the Access Control List? (y/n)");
    while (Serial.available() == 0) {}
    waitForSerial();
    String response = Serial.readStringUntil('\n');
    response.trim();

    // checks user response
    // if yes prompts for admin password
    // if no returns to main loop and waits for another card!!
    switch (response.c_str()[0]) { 
      // User wants to add card to ACL
      case 'y':
      case 'Y':
        if (IsAdmin()) {
          displayAndSerial("Adding Card to ACL");
          //delay(1000);
          addCardtoACL();
          //delay(1000);
          displayAndSerial("Card Added to ACL");
        break;
        } else {
          displayAndSerial("Admin Access Denied - Cannot Add Card to ACL");
          return;
        }
      // User doesnt want to add card to ACL
      case 'n':
      case 'N':
        displayAndSerial("Card Not Added to ACL");
        break;
      default:
        displayAndSerial("Invalid Response ");
  }

  }



}

// ask usrer for admin password to allow them to set UID
// always returns true for now because they have unlimited attempts (attack vector) , fix later
boolean IsAdmin() {
  boolean isAdmin;

  displayAndSerial("Enter Admin password:");
  while (Serial.available() == 0) {}
  waitForSerial();
  guessed = Serial.readStringUntil('\n');
  guessed.trim();
  displayAndSerial("You Entered: " + guessed);
  //delay(2000); //give em time to read display

  while (guessed != pswd){ 
    isAdmin = false;
    AccessDenied();
    //delay(1000);

    displayAndSerial("Try again? (y/n)");
    while (Serial.available() == 0) {}
    waitForSerial();
    //delay(2000);
    String answer = Serial.readStringUntil('\n');
    answer.trim();

    switch(answer.c_str()[0]) 
    {
      case 'y':
      case 'Y':
    
        displayAndSerial("Enter Admin password:");
        
        while (Serial.available() == 0) {}
        waitForSerial();
        //delay(500);
        
        guessed = Serial.readStringUntil('\n');
        guessed.trim();
        displayAndSerial("You Entered: " + guessed);      
        break; // continue while loop 

      case 'n':
      case 'N':
        return isAdmin;
      
      default:
        displayAndSerial("Invalid Response ");

    }



  }


  AccessGranted();
  displayAndSerial("Access Granted. ");
  // Will add code for buzzer sound here later.
  isAdmin = true;
  
  guessed = "";  // Reset for next round
  //delay(3000);

  return isAdmin;

}

void blinkled(int pin) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(pin, HIGH);
    delay(500);
    digitalWrite(pin, LOW);
    delay(500);
  }
}

void displayAndSerial(String message) {
  Serial.println(message);
  clearDisplay();
  oled.setTextSize(1.5); // can i set as decimal? 
  // how to center alll texts? 
  int16_t x1, y1;
  uint16_t w, h;
  oled.getTextBounds(message, 0, 0, &x1, &y1, &w, &h);
  oled.setCursor((oled.width() - w) / 2, (oled.height() - h) / 2);

  oled.setTextColor(SSD1306_WHITE);
  oled.println(message);
  oled.display();  //render to screen
  delay(2000);
}
void waitForSerial(unsigned long timeout) {
  unsigned long start = millis();
  while (Serial.available() == 0 && millis() - start < timeout) {}
  delay(500);
}
void setup() {
  Serial.begin(9600);
  //delay(5000);  // Give serial time to initialize
  while (!Serial){}
  // Initialize I2C pins
  Wire.begin(sda, scl);
  
  // Initialize OLED (ONLY ONCE)
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);  // Loop forever if OLED fails
  }
  
  // Initialize SPI for RFID, which will be added later
  SPI.begin(sck, miso, mosi, ss);
  rfid.PCD_Init();
  byte version = rfid.PCD_ReadRegister(MFRC522::VersionReg);
  Serial.print("MFRC522 version: 0x");
  Serial.println(version, HEX);
  displayAndSerial("Starting....");
  
  
  displayAndSerial("Hello, ESP32!");
  

  
  // LED PINMODES
  pinMode(redled, OUTPUT);
  pinMode(greenled, OUTPUT);
  //digitalWrite(redled, HIGH);
}

void loop() {
  
  // sets admin password once at the start
  if (pswdset == false) {
    delay(100);
    digitalWrite(redled, HIGH);
    displayAndSerial("Set a 6 character password:");
    while (Serial.available() == 0) {}
    waitForSerial();
    pswd = Serial.readStringUntil('\n');
    pswd.trim();

    int pswdlength = pswd.length();
    
    if (pswdlength < 6){ //if password aint 6 chars restart the entire loop
       displayAndSerial("Password must be 6 characters");
       //delay(2000);
       return ;
      }
    displayAndSerial("You Entered: ******");
    delay(1000);
    pswdset = true;
    displayAndSerial("Password set!");
    delay(1000);
    
  }

  AccessControl();

  
}