#include <MFRC522.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ANSWERS TO YOUR QUESTIONS:
// #define vs const? 
// - #define: preprocessor directive, replaced at compile time, no type checking
// - const: actual variable with type checking, takes memory, safer
// 
// What defines baud rate?
// - Serial.begin(115200) in setup() defines the baud rate
// - Must match monitor_speed in platformio.ini

const int redled = 26;
const int greenled = 21;

String pswd ;
String guessed = "";
String cardID = "";
bool pswdset = false;

const int sda = 13;
const int scl = 14;

const int sck = 18;   // ESP32 default SPI clock
const int miso = 19;  // ESP32 default SPI MISO
const int mosi = 23;  // ESP32 default SPI MOSI
const int ss = 5;     // ESP32 default SPI CS
const int rst = 4;   // RFID reset pin
bool cardpresent = false;

String ACL[]; // access controll list contsining each UI for RFID tag/card

MFRC522 rfid(ss, rst);
Adafruit_SSD1306 oled(128, 64, &Wire, -1);

void clearDisplay() {
  oled.clearDisplay();
  oled.setCursor(0, 0);  // Reset cursor to top-left
}

void addCardtoACL(String cardID) {
  //ACL[sizeof(ACL) / sizeof(ACL[0])] = cardID; // Add card ID to ACL
}

void lookinforcard() //loops untill a card is found, if card is found prints to serial
{
  while (true){
    if ( rfid.PICC_IsNewCardPresent()) // loop ag
    {
      break; //card is close by so break out of loop

    } 
    displayAndSerial("No Card Detected - Please Tap Your Card");
    delay(1000);
  

    if ( ! rfid.PICC_ReadCardSerial())  // if there is a tag present but cant read it, loop again
    {
      displayAndSerial("Card Exists but Cannot get UID");
    } else { // get card UID
      for (byte i = 0; i < rfid.uid.size; i++) {
        cardID += String(rfid.uid.uidByte[i], HEX);
      }
      cardID.toUpperCase();
      displayAndSerial("Card Detected with UID: " + cardID);

      Serial.println();

      rfid.PICC_HaltA(); // this closed the reader's connection to the card, so we can read another card after this
      //rfid.PCD_StopCrypto1(); // stop encryption on PCD, i dont think its encrypted fr
    }
  }

}

void AccessControl() {
  lookinforcard();
  // if card is in ACL, grant access, else deny access
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
        delay(1000);
        addCardtoACL(cardID);
        delay(1000);
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
      displayAndSerial("Invalid Response - Card Not Added to ACL");
  }
}

// ask usrer for admin password to allow them to set UID
// always returns true for now because they have unlimited attempts (attack vector) , fix later
boolean IsAdmin() 
{
  boolean isAdmin;

  displayAndSerial("Enter Admin password:");
  while (Serial.available() == 0) {}
  waitForSerial();
  guessed = Serial.readStringUntil('\n');
  guessed.trim();
  displayAndSerial("You Entered: " + guessed);
  delay(2000); //give em time to read display

  while (guessed != pswd) {
    isAdmin = false;
    blinkled(redled);
    displayAndSerial("Access Denied - Try Again");
    delay(1000);
    
    displayAndSerial("Enter Admin password:");
    
    while (Serial.available() == 0) {}
    delay(500);
    
    guessed = Serial.readStringUntil('\n');
    guessed.trim();
    displayAndSerial("You Entered: " + guessed);

  }
  
  displayAndSerial("Access Granted. ");
  isAdmin = true;
  delay(1000);
  digitalWrite(redled, LOW);
  blinkled(greenled);
  digitalWrite(greenled, HIGH);
  
  guessed = "";  // Reset for next round
  delay(3000);
  digitalWrite(greenled, LOW);
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
  delay(500);
}
void waitForSerial(unsigned long timeout = 10000) {
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
  displayAndSerial("Starting....");
  
  
  displayAndSerial("Hello, ESP32!");
  

  
  // LED PINMODES
  pinMode(redled, OUTPUT);
  pinMode(greenled, OUTPUT);
  digitalWrite(redled, HIGH);
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
       delay(2000);
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