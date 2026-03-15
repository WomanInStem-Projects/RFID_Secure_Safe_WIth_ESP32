#include <MFRC522.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


const int redled = 26;
const int greenled = 33;

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  pinMode(redled, OUTPUT);
  pinMode(greenled, OUTPUT);
  
  Serial.println("Testing LEDs...");
}

void loop() {
  Serial.println("Red LED ON");
  digitalWrite(redled, HIGH);
  delay(1000);
  
  Serial.println("Red LED OFF");
  digitalWrite(redled, LOW);
  delay(1000);
  
  Serial.println("Green LED ON");
  digitalWrite(greenled, HIGH);
  delay(1000);
  
  Serial.println("Green LED OFF");
  digitalWrite(greenled, LOW);
  delay(1000);
}