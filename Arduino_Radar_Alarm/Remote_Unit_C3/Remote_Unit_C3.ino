#include <SPI.h>             
#include <RF24.h>
#include <Wire.h> 
#include <nRF24L01.h>
#include <LiquidCrystal_I2C.h> 
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
RF24 radio(7, 8);

const byte addresses[][6] = {"00111", "01100"};
char next[32]="";
int Node =0;
int Spot1 = 4;
int Spot2 = 3;
int OutSpot = 2;
int Lite1;
int Lite2;
int LitesOUT=0;
int GoButton=0;
int wude=0;
int Loop1=0;
int Loop2=0;
int Loop3=0;
int Standbhaai=0;
const char Diagnopass[32]="DIAGnosis";
const char StanBypass[32]="Standby_Mode";
const char Almstatpass[32]="System_Online";

void WatSeJy(){
Standbhaai ++;  
LitesOUT=1;
}

void setup(){
attachInterrupt(digitalPinToInterrupt(OutSpot),WatSeJy,CHANGE);
Serial.begin(9600);
Wire.begin();
lcd.begin();
lcd.backlight();
lcd.setCursor(0, 0);
lcd.print(" nRF24_RCWL0516 ");
lcd.setCursor(0, 1);
lcd.print("  Alarm Remote  ");
delay(3000);
lcd.clear();
memset(next,0,sizeof(next));
radio.begin();
radio.openWritingPipe(addresses[0]);
radio.openReadingPipe(0, addresses[1]);
radio.setChannel(100);
radio.setDataRate(RF24_250KBPS);
radio.setPALevel(RF24_PA_MAX);
pinMode(Spot1,INPUT);
pinMode(Spot2,INPUT);
pinMode(OutSpot,INPUT);
lcd.setCursor(3, 0);
lcd.print("Monitor and");
lcd.setCursor(5, 1);
lcd.print("Control");
delay(2000);
lcd.clear();
wude=0;
lcd.backlight();
lcd.setCursor(0, 0);
lcd.print("Triggered Alarms");
lcd.setCursor(1, 1);
lcd.print("    ONLY        ");
 }

void loop(){
  Active();
  Button();
if(Standbhaai>=50){
radio.stopListening();
const char talkies[32] = "Stop_everything";
radio.write(&talkies,sizeof(talkies));    
delay(330);
Serial.println(talkies);
Standbhaai=0;
Serial.println("Standby activated");
}}

void Stendby(){
lcd.backlight();
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Standby Mode");
lcd.setCursor(1, 1);
lcd.print("(Active in 30s)"); 
for(int ty=30;ty>=0;ty--){
  delay(1000);
lcd.setCursor(9, 1);  
lcd.print(ty);}
lcd.setCursor(0, 0);
lcd.print("Standby Mode");
if(!(strncmp(next,"Alarm Reset",11))){
  Serial.println("Received clear command");
Standbhaai=-10;
Active();
}}

void Active(){
memset(next,0,sizeof(next));  
lcd.clear();
lcd.backlight();
lcd.setCursor(0, 0);
lcd.print("  Alarm Status  ");
delay(5);
radio.startListening();
radio.read(&next,sizeof(next));
if(!(strncmp(next,Almstatpass,13))){
lcd.setCursor(0,1);
lcd.print("                ");}
else{
lcd.setCursor(0,1);
lcd.print(next);
Serial.println(next);
delay(400);
}
if(!(strncmp(next,StanBypass,12))){
  Stendby();
  Serial.println("Standby scope");
  delay(25);}
if(!(strncmp(next,Diagnopass,9))){
Loop1=1;
while(Loop1==1){  
lcd.setCursor(0,0);
lcd.print("Sensor values   ");
if(!(strncmp(next,Diagnopass,9))){
  RefreshLoops();
lcd.setCursor(0,1);
lcd.print("                ");}
else {
lcd.setCursor(0,1);
lcd.print(next);
delay(100);
Serial.println(next);
RefreshLoops();
}}
}}

void Button(){
Lite1 = digitalRead(Spot1);
Lite2  = digitalRead(Spot2);
if(Lite1==HIGH){
lcd.setCursor(0,1);
lcd.print("Spotlight 1 ON");
radio.stopListening();
const char words[] = "SpotA";
radio.write(&words,sizeof(words));
Standbhaai --;
Serial.println(Standbhaai);
delay(25);}
if(Lite2==HIGH){
lcd.setCursor(0,1);
lcd.print("Spotlight 2 ON");
radio.stopListening();
const char boodskap[] = "SpotB";
radio.write(&boodskap,sizeof(boodskap));
Standbhaai --;
Serial.println(Standbhaai);
delay(25);}
if(LitesOUT==1){
lcd.clear();
lcd.setCursor(0,1);
lcd.print("Lites OFF");
lcd.clear();
lcd.backlight();
lcd.setCursor(0, 0);
Serial.println(Standbhaai);
lcd.print("  Active Alarm  ");
radio.stopListening();
const char praat[] = "OFF";
radio.write(&praat,sizeof(praat));
delay(44);
LitesOUT=0;
}}

void RefreshLoops(){
radio.startListening();
radio.read(&next,sizeof(next));
if(!(strncmp(next,"DIAGnosis",9))){
Loop1=1;
Loop2=0;
Loop3=0;
Serial.println(next);}
if(!(strncmp(next,"System_Reset",12))){
Loop1=0;
Loop2=1;
Loop3=0;
Serial.println(next);}
if(!(strncmp(next,"System_Online",13))){
Loop1=0;
Loop2=0;
Loop3=1;
Serial.println(next);
}}
