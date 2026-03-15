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
const int greenled = 33;

String pswd = "";
String guessed = "";
bool pswdset = false;

const int sda = 13;
const int scl = 12;

const int sck = 18;   // ESP32 default SPI clock
const int miso = 19;  // ESP32 default SPI MISO
const int mosi = 23;  // ESP32 default SPI MOSI
const int ss = 5;     // ESP32 default SPI CS
const int rst = 22;   // RFID reset pin

MFRC522 rfid(ss, rst);
Adafruit_SSD1306 oled(128, 64, &Wire, -1);

void clearDisplay() {
  oled.clearDisplay();
  oled.setCursor(0, 0);  // Reset cursor to top-left
}

void blinkled(int pin) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(pin, HIGH);
    delay(200);
    digitalWrite(pin, LOW);
    delay(200);
  }
}

void displayAndSerial(String message) {
  Serial.println(message);
  clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.println(message);
  oled.display();  // IMPORTANT: Render to screen
  delay(500);
}
void waitForSerial(unsigned long timeout = 10000) {
  unsigned long start = millis();
  while (Serial.available() == 0 && millis() - start < timeout) {}
  delay(500);
}
void setup() {
  Serial.begin(115200);
  delay(10000);  // Give serial time to initialize
  
  // Initialize I2C pins (optional for most boards)
  Wire.begin(sda, scl);
  
  // Initialize OLED (ONLY ONCE)
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);  // Loop forever if OLED fails
  }
  
  // Initialize SPI for RFID
  SPI.begin(sck, miso, mosi, ss);
  rfid.PCD_Init();
  delay(10000);
  displayAndSerial("Starting....");
  
  
  displayAndSerial("Hello, ESP32!");
  
  // displayAndSerial("Set a 4-digit binary password:");
  
  // waitForSerial(10000);  // Wait max 10 seconds
  // delay(500);
  // pswd = Serial.readStringUntil('\n');
  // pswd.trim();
  
  // displayAndSerial("Password set!");
  // delay(1000);
  
  // displayAndSerial("Enter binary password:");
  
  // waitForSerial(10000);  // Wait max 10 seconds
  // delay(500);
  // guessed = Serial.readStringUntil('\n');
  // guessed.trim();
  
  // LED PINMODES
  pinMode(redled, OUTPUT);
  pinMode(greenled, OUTPUT);
  digitalWrite(redled, HIGH);
}

void loop() {
  digitalWrite(redled, HIGH);

  if (pswdset == false) {
    displayAndSerial("Set a 4-digit binary password:");
    while (Serial.available() == 0) {}
    pswd = Serial.readStringUntil('\n');
    pswd.trim();
    pswdset = true;
    displayAndSerial("Password set!");
    delay(1000);
    displayAndSerial("Enter binary password:");
    while (Serial.available() == 0) {}
    guessed = Serial.readStringUntil('\n');
    guessed.trim();
  }
  
  while (guessed != pswd) {
    blinkled(redled);
    displayAndSerial("Access Denied - Try Again");
    delay(1000);
    
    displayAndSerial("Enter binary password:");
    
    while (Serial.available() == 0) {}
    delay(500);
    
    guessed = Serial.readStringUntil('\n');
    guessed.trim();
  }
  
  displayAndSerial("Access Granted. Welcome!");
  digitalWrite(redled, LOW);
  
  blinkled(greenled);
  digitalWrite(greenled, HIGH);
  
  // pswd = "";
  guessed = "";  // Reset for next round
  delay(3000);
  
  // displayAndSerial("Enter binary password:");
  // while (Serial.available() == 0) {}
  // delay(500);
  // guessed = Serial.readStringUntil('\n');
  // guessed.trim();
}