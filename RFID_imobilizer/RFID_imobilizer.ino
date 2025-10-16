#include <MFRC522.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int R1=3;
int R2=4;
int R3=5;
int R4=6;
int state=0;
#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key; 
int code[] = {73,141,168,90}; //This is the stored UID
int codeRead = 0;
String uidString;

void setup() {
pinMode(R1,OUTPUT);
pinMode(R2,OUTPUT);
pinMode(R3,OUTPUT);
pinMode(R4,OUTPUT);
digitalWrite(R1,HIGH);
digitalWrite(R2,HIGH);
digitalWrite(R3,HIGH);
digitalWrite(R4,HIGH);
SPI.begin(); // Init SPI bus
rfid.PCD_Init(); // Init MFRC522 
display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.clearDisplay();
  display.display();
  display.setTextColor(WHITE); // or BLACK);
  display.setTextSize(2);
  display.setCursor(5,0); 
  display.print("Swipe RFID");
  display.setTextSize(2);
  display.setCursor(38,17);//(horizontal/vertical) 
   display.print("CARD");
  display.display();
}
void loop() {
  if(  rfid.PICC_IsNewCardPresent())
  {
      readRFID(); }
  delay(100);
}
void readRFID()
{
  rfid.PICC_ReadCardSerial();
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    return;
  }
    clearUID();
    printDec(rfid.uid.uidByte, rfid.uid.size);
    uidString = String(rfid.uid.uidByte[0])+" "+String(rfid.uid.uidByte[1])+" "+String(rfid.uid.uidByte[2])+ " "+String(rfid.uid.uidByte[3]);
    printUID();
    int i = 0;
    boolean match = true;
    while(i<rfid.uid.size)
    {
      if(!(rfid.uid.uidByte[i] == code[i]))
      {
           match = false;
      }
      i++;
    }
    if(match)
    {
      printUnlockMessage();
    }else
    {
      digitalWrite(R1,HIGH);
digitalWrite(R2,HIGH);
digitalWrite(R3,HIGH);
digitalWrite(R4,HIGH);
    }
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
  void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
  }}
  void clearUID()
  {
    display.clearDisplay();
    display.setTextColor(WHITE); // or BLACK);
    display.setTextSize(1);
    display.setCursor(0,0); 
    display.print(uidString);
    display.display();
  }
   void printUID()
  {
    display.clearDisplay();
    display.setTextColor(WHITE); // or BLACK);
    display.setTextSize(1);
    display.setCursor(0,10); 
    display.print("This card's UID: ");
    display.setCursor(30,20); 
    display.print(uidString);
    delay(1000);
      display.clearDisplay();
        display.setTextSize(2);
    display.setCursor(0,10); 
    display.print("DENIED");
    display.display();
  }
  void printUnlockMessage()
  {
     display.clearDisplay();
    display.display();
    display.setTextColor(WHITE); // or BLACK);
    display.setTextSize(1);
    display.setCursor(0,0); 
    display.print("Scan Accepted");
    display.display(); 
    display.setTextColor(WHITE); // or BLACK);
    display.setTextSize(2);
    display.setCursor(13,18); 
    display.print("Enabled");
    display.display();
    if(state==0){
digitalWrite(R1,LOW);
digitalWrite(R2,HIGH);
digitalWrite(R3,HIGH);
digitalWrite(R4,LOW);
state=1;}
else if(state==1){
digitalWrite(R1,HIGH);
digitalWrite(R2,LOW);
digitalWrite(R3,LOW);
digitalWrite(R4,HIGH);
state=0;}
delay(10000);
display.clearDisplay();
    display.setTextColor(WHITE); // or BLACK);
    display.setTextSize(2);
    display.setCursor(15,10); 
    display.print("Scan CARD");
    display.display();
        digitalWrite(R1,HIGH);
digitalWrite(R2,HIGH);
digitalWrite(R3,HIGH);
digitalWrite(R4,HIGH);
  }
