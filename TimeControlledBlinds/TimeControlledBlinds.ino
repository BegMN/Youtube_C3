#include "Adafruit_VL53L0X.h"
#include <DS3231.h>
#include <string.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
DS3231  rtc(A4, A5);				
Time t;									
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
/////////////////////////////////////////////////////////////////
const int VeranderK = 2;	
const int HuurK     = 5;	
const int MinuK     = 6;
const int WysMy     = 7;
const int Oop       = 8;
const int Toe       = 9;
int moIn1           = 14;
int moIn2           = 15;

int hour;
int min;
String clock;
int number;
int count=0;
int Optime;
int Cltime;
int CBass;
int ZBass;
int DBcount=0;
String Horiz="xx:xx";
String Verti="zz:zz";
////////////////////////////////////////////////////////////////////////
void WieIsJouPa(){
count ++;  
DBcount=(count);
if(count>21){
  count=0;
  DBcount=0;
}}
///////////////////////////////////////////////////////////////////////////
void setup() {
attachInterrupt(digitalPinToInterrupt(VeranderK),WieIsJouPa,CHANGE);
lox.begin(0x29);
SPI.begin();
Wire.begin();
lcd.begin();
lcd.backlight();
lcd.clear();      
lcd.setCursor(0, 0);
lcd.print("Time Controlled");
lcd.setCursor(0, 1);
lcd.print("     BLINDS     ");
delay(2000);
lcd.clear();
	pinMode(VeranderK, INPUT_PULLUP);
	pinMode(HuurK, INPUT_PULLUP);
	pinMode(MinuK, INPUT_PULLUP);
  pinMode(WysMy,OUTPUT);
  pinMode(moIn1,OUTPUT);
  pinMode(moIn2,OUTPUT);
  pinMode(Oop,OUTPUT);
  pinMode(Toe,OUTPUT);
  String Horiz="";
  String Verti="";
  rtc.begin();  
  digitalWrite(moIn1,LOW);
  digitalWrite(moIn2,LOW);
}
void OpenTime(){
while(clock==Horiz){
digitalWrite(Toe,LOW);
digitalWrite(WysMy,LOW);
digitalWrite(Oop,LOW);
DisplayOly();
lox.begin(0x29);
VL53L0X_RangingMeasurementData_t measure;
lox.rangingTest(&measure, false);
if (measure.RangeStatus != 4) {
if (measure.RangeStatus>=8190){
digitalWrite(moIn1,LOW);
digitalWrite(moIn2,LOW);}
if((measure.RangeMilliMeter)<=40){
digitalWrite(moIn1,LOW);
digitalWrite(moIn2,HIGH);}
else if((measure.RangeMilliMeter)>=50){
  digitalWrite(moIn1,LOW);
digitalWrite(moIn2,LOW);}
}}}
///////////////////////////////////////////////////////////////////
void CloseTime(){
while(clock==Verti){
digitalWrite(Toe,LOW);
digitalWrite(WysMy,LOW);
digitalWrite(Oop,LOW);
DisplayOly();
lox.begin(0x29);
VL53L0X_RangingMeasurementData_t measure;
lox.rangingTest(&measure, false);
if (measure.RangeStatus != 4) { 
if (measure.RangeStatus ==8190){ 
digitalWrite(moIn1,LOW);
digitalWrite(moIn2,LOW);}
if((measure.RangeMilliMeter)>=50){
digitalWrite(moIn1,HIGH);
digitalWrite(moIn2,LOW);}
else if((measure.RangeMilliMeter)<=40){
  digitalWrite(moIn1,LOW);
digitalWrite(moIn2,LOW);}
}}}
///////////////////////////////////////////////////////////////////
void DisplayOly(){
lcd.clear();
lcd.setCursor(5, 0);
lcd.print(clock);
lcd.setCursor(0,1);
lcd.print("Op");
lcd.setCursor(2,1);
lcd.print(Horiz);
lcd.setCursor(9,1);
lcd.print("Cd");
lcd.setCursor(11,1);
lcd.print(Verti);
if ((hour<=9) && (min>9)){
clock = '0' + String(hour) +':'+ String(min);
}
else if ((min<=9) && (hour>9)){
clock = String(hour) +':'+ '0' + String(min);
}
else if ((hour<=9) && (min<=9)){
clock = '0' + String(hour) + ':'+'0' + String(min);
}
else{
clock = String(hour) +':'+ String(min);
}
t = rtc.getTime();
hour = t.hour;
min = t.min;
delay(300);}
//////////////////////////////////////////////////////////////////////////////////  
void loop()
{	
DisplayOly();  
if(Horiz==Verti){
lcd.backlight();
lcd.clear();      
lcd.setCursor(0, 0);
lcd.print("Open & Close Hrs");
lcd.setCursor(0, 1);
lcd.print("match - adj.Open");
digitalWrite(moIn1,LOW);
digitalWrite(moIn2,LOW);
DBcount=9;
delay(1200);}
if(Horiz==Verti&&Verti==clock){
lcd.backlight();
lcd.clear();      
lcd.setCursor(0, 0);
lcd.print("Open & Close Hrs");
lcd.setCursor(0, 1);
lcd.print("  times cleared ");
digitalWrite(moIn1,LOW);
digitalWrite(moIn2,LOW);
Horiz="xx:xx";
Verti="zz:zz";
DBcount=9;
delay(1200);}
if(clock==Horiz){
OpenTime();}
if(clock==Verti){ 
CloseTime();   
}
digitalWrite(moIn1,LOW);
digitalWrite(moIn2,LOW);
digitalWrite(Toe,LOW);
digitalWrite(WysMy,LOW);
digitalWrite(Oop,LOW);
DisplayOly();                                    //Standby mode
while (DBcount>0&&DBcount<=4){
DisplayOly();
OpenTime();
CloseTime();
lcd.noBacklight(); 
}
while(DBcount==5){                             //Setting RTC
lcd.backlight();
lcd.clear(); 
lcd.setCursor(0, 0);
lcd.print("Wait for BlueLED");
lcd.setCursor(1, 1);
lcd.print(" to adjust RTC  ");
  delay(300);
}
while (DBcount>=5&&DBcount<=7){
lcd.backlight();
lcd.clear();
DisplayOly();
        digitalWrite(WysMy,HIGH);
        digitalWrite(Oop,LOW);
        digitalWrite(Toe,LOW);
  if (digitalRead(HuurK) == LOW){
    delay(300);
      if (hour>22){
        hour=0;
      }
      else{
        hour++;
      }
   rtc.setTime(hour, min, 0);
  }
      if (digitalRead(MinuK) == LOW){ 
        delay(300);
      if (min>58){
        min=0;
      }
      else{
        min++;
      }   }
      rtc.setTime(hour, min, 0);  
  t = rtc.getTime();
  hour = t.hour;
  min = t.min;
}
while(DBcount==8){         // Setting the open time 
lcd.backlight();
lcd.clear(); 
lcd.setCursor(0, 0);
lcd.print("Wait for RED LED");
lcd.setCursor(1, 1);
lcd.print("to adj. OPEN hr ");
  delay(300);
}
 while((DBcount>=8&&DBcount<=10)){
  lcd.clear();
  digitalWrite(Oop,HIGH);
  digitalWrite(WysMy,LOW);
  digitalWrite(Toe,LOW);
  lcd.setCursor(1,1);
  lcd.print("oo:--");
  lcd.setCursor(1,1);
  lcd.print(Horiz);
  delay(100);
  if (digitalRead(HuurK) == LOW){
    delay(300);
  if (Optime>22){
  Optime=0;}
  else{
  Optime++;
CBass=Optime;}
  if (CBass<10){
   Horiz ='0'+String(CBass)+ ':'+'0'+'0';
  }
  else if(CBass>10){ Horiz =String(CBass) + ':'+'0'+'0';
  lcd.print(Horiz);
  delay(100);
 }}}
while(DBcount==11){               //Setting the close time
   lcd.setCursor(0,0);
  lcd.print(" Wait for GREEN ");
   lcd.setCursor(0,1);
  lcd.print("to adj. Close Hr");
  delay(300);
}
while((DBcount>=11&&DBcount<=13)){          
lcd.clear();
lcd.backlight();
digitalWrite(Toe,HIGH);
digitalWrite(WysMy,LOW);
digitalWrite(Oop,LOW);
lcd.setCursor(1,1);
lcd.print("--:oo");
lcd.setCursor(1,1);
lcd.print(Verti);
delay(100);
if (digitalRead(HuurK) == LOW){
    delay(300);
  if (Cltime>22){
  Cltime=0;}
  else{
  Cltime++;
ZBass=Cltime;}
  if (ZBass<10){
   Verti ='0'+String(ZBass)+ ':'+'0'+'0';
  }
  else if(ZBass>10){ Verti =String(ZBass) + ':'+'0'+'0';
  lcd.print(Verti);
  delay(100);
}}}} 
//////////////////////////////////////////////////////////////////////////////////////////////////
