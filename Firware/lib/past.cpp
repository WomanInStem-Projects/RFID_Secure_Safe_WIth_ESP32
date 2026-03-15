#include <MFRC522.h>
#include <SPI.h>
//#include <LiquidCrystal_I2C.h>

// Questions:
// #define vs const?
//What defines what nbaud rate is used for serial?

// define const led pins 
const int redled = 26; //GPIO 32
const int greenled = 33;


// define non-const variables
String pswd = "";
bool authenticated[4]; //'binary' password of lenth 4
String guessed = "";

// define I2C backed lcd pins
const int sda = 13 ; //serial data
const int scl = 12; //clock

// SCK, MISO, MOSI, CS, rst
const int sck = 6;
const int miso = 9;
const int mosi = 8;
const  int ss = 7; // slave select aka cs(chip select) aka SDA pin #
const int rst = 10; //reset pin #

// Create object instances
// RFID instance
MFRC522 rfid(ss,rst);

// LCD instace
LiquidCrystal_I2C lcd(0x27,16,2);

// helper functions
void clearserial()
{ // the illusion or clearing
  for (int i = 0; i < 30; i++) {
    Serial.println();
  }
}
void blinkled(int pin)
{
  for (int i = 0; i < 4 ; i++){
      digitalWrite(pin, HIGH);
      delay(200);
      digitalWrite(pin, LOW);
      delay(200);
    }
}


void setup() {
  // put your setup code here, to run once:
  //initializing components
  Serial.begin(115200);
  SPI.begin(sck,miso,mosi,ss); 
  rfid.PCD_Init();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.autoscroll();
  delay(1000);

  // rfid.PCD_DumpVersionToSerial();
  Serial.println("Hello, ESP32-S2!");
  lcd.print("Hello, ESP32-S2!");
  delay(1000);
  clearserial(); 
  lcd.blink();
  Serial.println("Set a 4 bit binary password:  ");
  lcd.print("Set a 4 bit binary password:  ");


  while (Serial.available()== 0){} // if nothing is entered wait here untill user enters password
  delay(1000); //wait for input to come in
  pswd = Serial.readStringUntil('\n'); // read input till new line character('Enter')
  pswd.trim();//remove newlines and spaces

  Serial.println("Encrypting password...."); // thats bs im rlly not, maybe later
  lcd.print("Encrypting password....");
  delay(2000);
  Serial.println("Password set! ");
  lcd.print("Password set! ");

  //ask user for password to be authenticated (first iteration)
  Serial.println("Enter binary password:");
  lcd.print("Enter binary password: ");

  while (Serial.available()== 0){} // if nothing is entered wait here untill user enters password
  guessed = Serial.readStringUntil('\n');

  // LED PINMODES
  pinMode(redled, OUTPUT);
  pinMode(greenled, OUTPUT);
  digitalWrite(redled, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(redled,HIGH);
  clearserial();

  
  while (guessed != pswd)
  {
    blinkled(redled);
    Serial.println("Access Denied");
    lcd.print("Access Denied");
    Serial.println("Try Again");
    delay(2000);
    clearserial();

    Serial.println("Enter binary password:");
    lcd.print("Enter binary password:");
    while (Serial.available()== 0){} // if nothing is entered wait here untill user enters password
    delay(1000); //wait for input to come in

    guessed = Serial.readStringUntil('\n');
    guessed.trim();
  }
  Serial.println("Access Granted. Welcome!");
  lcd.print("Access Granted. Welcome!");
  digitalWrite(redled,LOW);

  //flash the green lef 4 times
  blinkled(greenled);
  digitalWrite(greenled,HIGH);
  pswd ="";
  delay(3000); // //time till password will be asked for again
}
