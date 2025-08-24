#include <SPI.h>             
#include <RF24.h>
#include <Wire.h> 
#include <nRF24L01.h>
#include <LiquidCrystal_I2C.h> 
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00111", "01100"};
//pin declarions
const int RCWL1 = A0 ;
const int RCWL2 = A1 ;
const int RCWL3 = A2 ;
const int RCWL4 = A3 ;
const int Diagno = 5;
const int AlmReset = 2;
const int Alam  = 9;
const int Lighta = 3;
const int Lightb = 4;
//variable declarations
int w =0;
int x = 0;
int y = 0;
int z =0;
int Sen1=0;
int Sen2=0;
int Sen3=0;
int Sen4=0;
int Ring=0;
int SenAdd=0;
int Recall=4;
int AlmCount=0;

//interrupt function
void AlrmRing(){ 
digitalWrite(Alam,HIGH);
digitalWrite(Lighta,HIGH);
digitalWrite(Lightb,HIGH); 
Ring=0;
Sen1=0;
Sen2=0;
Sen3=0;
Sen4=0;
AlmCount=0;
}

void setup(){
//Serial.begin(9600);
attachInterrupt(digitalPinToInterrupt(AlmReset),AlrmRing,CHANGE);
Wire.begin();
lcd.begin();
lcd.backlight();
lcd.print("Master Unit");
delay(1000);
lcd.clear();
radio.begin();
radio.openWritingPipe(addresses[1]);
radio.openReadingPipe(0, addresses[0]);
radio.setChannel(100);
radio.setDataRate(RF24_250KBPS);
radio.setPALevel(RF24_PA_MAX);
pinMode(Diagno,INPUT);
pinMode(AlmReset,INPUT);
pinMode(Alam,OUTPUT);
pinMode(Lighta,OUTPUT);
pinMode(Lightb,OUTPUT);
lcd.setCursor(5, 0);
lcd.print("ARMED");
delay(500);
digitalWrite(Alam,HIGH);
digitalWrite(Lighta,HIGH);
digitalWrite(Lightb,HIGH);
delay(300); //29J25
while(!(radio.available())==0){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("nRF24 down");
  lcd.setCursor(0,1);
  lcd.print("Reset if Fixed");
  delay(200);
}
}

void loop(){ 
radio.startListening();
delay(35);
char Reesettd[]="Stop_everything";
char Remotewords[32]=""; 
if (radio.available()) {
radio.read(&Remotewords,sizeof(Remotewords));
if(strcmp(Remotewords,Reesettd) == 0) {
Standbye();
}}
if(digitalRead(Diagno)==HIGH){
lcd.clear();
Diagnosis();
}
else
SkreenLuister();
}

void Standbye(){
digitalWrite(Alam,HIGH); 
digitalWrite(Lighta,HIGH);
digitalWrite(Lightb,HIGH);  
delay(100);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("  Standby Mode  ");
radio.stopListening();
delay(250);
radio.stopListening();
const char ztans[32] = "Standby_Mode";
radio.write(&ztans,sizeof(ztans));
delay(25);
//Serial.println(ztans);
for(int tim=30;tim>=0;tim--){
lcd.setCursor(6,1);
if(tim<10){
lcd.setCursor(7,1);
lcd.print("        ");  
lcd.setCursor(6,1);
lcd.print((tim),1);}
else {
lcd.print(tim);}
delay(1000);}
lcd.clear();
radio.stopListening();
const char ytans[32] ="Alarm Reset";
radio.write(&ytans,sizeof(ytans));
delay(30);
//Serial.println(ytans);
SkreenLuister();
}

void Diagnosis(){
radio.stopListening();
const char tett[32] = "DIAGnosis";
radio.write(&tett,sizeof(tett));
lcd.backlight();
//to the lcd
int Ayone=(analogRead(RCWL1));
int Aytwo=(analogRead(RCWL2));
int Aytree=(analogRead(RCWL3));
int Ayfo=(analogRead(RCWL4));
if(Ayone!=0){
lcd.setCursor(0, 0);
lcd.print("ROL:");
lcd.setCursor(4,0);
lcd.print(Ayone);
char ValEen[32];
char Preone[32]="ROL:";
String Ree1=String(Ayone); 
Ree1.toCharArray(ValEen,32);
strncat(Preone,ValEen,(sizeof(ValEen)));
radio.stopListening();
radio.write(&Preone,sizeof(Preone));
delay(200);
}
if(Aytwo!=0){
lcd.setCursor(12,0);
lcd.print(Aytwo);
lcd.setCursor(8, 0);
lcd.print("Pag:");
char ValTwee[32];
char Preto[32]="PAG:";
String Ree2=String(Aytwo); 
Ree2.toCharArray(ValTwee,32);
strncat(Preto,ValTwee,(sizeof(ValTwee)));
radio.stopListening();
radio.write(&Preto,sizeof(Preto));
delay(200);
}
if(Aytree!=0){
lcd.setCursor(4,1);
lcd.print(Aytree);
lcd.setCursor(0, 1);
lcd.print("Bus:");
char ValDrie[32];
char Pretree[32]="BUS:";
String Ree3=String(Aytree); 
Ree3.toCharArray(ValDrie,32);
strncat(Pretree,ValDrie,(sizeof(ValDrie)));
radio.stopListening();
radio.write(&Pretree,sizeof(Pretree));
delay(200);
}
if(Ayfo!=0){
lcd.setCursor(12,1);
lcd.print(Ayfo);
lcd.setCursor(8, 1);
lcd.print("Zon:");
char ValVier[32];
char Prefor[32]="ZON:";
String Ree4=String(Ayfo); 
Ree4.toCharArray(ValVier,32);
strncat(Prefor,ValVier,(sizeof(ValVier)));
radio.stopListening();
radio.write(&Prefor,sizeof(Prefor));
delay(200);
}}

void SkreenLuister(){
radio.stopListening();
const char Almstat[] = "System_Online";
radio.write(&Almstat,sizeof(Almstat));
String prevAlarm[]={"Roller_Gate","Pagola","Bush","Backyard","No Alarms"};  
int  Rada1 = analogRead(RCWL1);
int  Rada2 = analogRead(RCWL2);
int  Rada3 = analogRead(RCWL3);
int  Rada4 = analogRead(RCWL4);

if(Rada1+Rada2+Rada3+Rada4+AlmCount==0){
lcd.noBacklight();
lcd.setCursor(0, 0);
lcd.print("No Active Alarms");
radio.stopListening();
const char case0[] = "No Alarms";
radio.write(&case0,sizeof(case0));
lcd.setCursor(0, 1);
lcd.print("                ");
lcd.noBacklight();
delay(100);
}
//Sensor 1 Read the port, display and Transmit
if(Rada1>500){
Sen1=1;
if(Sen1==1){
lcd.backlight();
lcd.setCursor(0, 0);
lcd.print("   Roller Gate  ");
radio.stopListening();
const char case1[] = "Roller_Gate";
radio.write(&case1,sizeof(case1));
digitalWrite(Alam,LOW);
lcd.setCursor(0, 1);
lcd.print("                ");
lcd.setCursor(0, 1);
lcd.print(prevAlarm[Recall]);
//Serial.println(prevAlarm[Recall]);
Recall=0;
AlmCount++;
delay(100);
}}
//Sensor 2 Read the port, display and Transmit
if(Rada2>500){
Sen2=2;
if(Sen2==2){
lcd.backlight();
lcd.setCursor(0, 0);
lcd.print("    Pagola      ");
radio.stopListening();
const char case2[] = "Pagola";
radio.write(&case2,sizeof(case2));
digitalWrite(Alam,LOW);
lcd.setCursor(0, 1);
lcd.print("                ");
lcd.setCursor(0, 1);
lcd.print(prevAlarm[Recall]);
//Serial.println(prevAlarm[Recall]);
Recall=1;
AlmCount++;
delay(100);}}
//Sensor 3 Read the port, display and Transmit
if(Rada3>500){
Sen3=4;
if(Sen3==4){
lcd.backlight();
lcd.setCursor(0, 0);
lcd.print("     Bush       ");
radio.stopListening();
const char case4[] = "Bush";
radio.write(&case4,sizeof(case4));
digitalWrite(Alam,LOW);
lcd.setCursor(0, 1);
lcd.print("                ");
lcd.setCursor(0, 1);
lcd.print(prevAlarm[Recall]);
//Serial.println(prevAlarm[Recall]);
Recall=2;
AlmCount++;
delay(100);}}
//Sensor 4 Read the port, display and Transmit
 if(Rada4>500){
Sen4=8;
if(Sen4==8){
lcd.backlight();
lcd.setCursor(0, 0);
lcd.print("     Backyard   ");
radio.stopListening();
const char case8[] = "Backyard";
radio.write(&case8,sizeof(case8));
digitalWrite(Alam,LOW);
lcd.setCursor(0, 1);
lcd.print("                ");
lcd.setCursor(0, 1);
lcd.print(prevAlarm[Recall]);
//Serial.println(prevAlarm[Recall]);
Recall=3;
AlmCount++;
delay(100);}}
//Listen to the transmissions and do stuff when the correct words are received
char swtOne[]="SpotA";
char swtTwo[]="SpotB";
char swtTree[]="OFF";
radio.startListening();
delay(35);
char Fromether[32]=""; 
char zext[32] = "";
char woord[32] = "";
char laste[32]="";
if (radio.available()) {
radio.read(&zext,sizeof(zext));
strcpy(Fromether,zext);
strcpy(woord,zext);
strcpy(laste,zext);
if(strcmp(Fromether,swtOne) == 0) {
digitalWrite(Lighta,LOW);
lcd.backlight();
lcd.setCursor(0,1);
lcd.print("Spotlight 1 ON");
delay(230);    }
if(strcmp(woord,swtTwo) == 0) {
digitalWrite(Lightb,LOW);
lcd.backlight();
lcd.setCursor(0,1);
lcd.print("Spotlight 2 ON"); 
delay(230);
}
if(strcmp(laste,swtTree) == 0) {
digitalWrite(Lighta,HIGH);
digitalWrite(Lightb,HIGH);
lcd.clear();
lcd.backlight();
lcd.setCursor(0,1);
lcd.print("Lites Off");
lcd.clear();
lcd.print("  System Ready  ");     
}}}
