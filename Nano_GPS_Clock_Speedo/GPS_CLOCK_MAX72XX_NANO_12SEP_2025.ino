#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h> 
SoftwareSerial ss(7, 8);
TinyGPSPlus gps;
int thour;
int tmin;
int mode=4;
String tspeed;
String tclock;
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN 10
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

void setup() {
ss.begin(9600);
pinMode(mode,INPUT);
myDisplay.begin();
myDisplay.setIntensity(0);
myDisplay.displayClear();
if (ss.available()==0){
myDisplay.setTextAlignment(PA_LEFT);
myDisplay.print("No Sig.");}
}

void loop() {     
if(digitalRead(mode)==HIGH){
gwatch();}
if(digitalRead(mode)==LOW){
gweedo();}}

void gwatch(){
if (ss.available() > 0) {
if (gps.encode(ss.read())){
if (gps.time.isValid()) {
thour = (gps.time.hour()+2);
tmin =  (gps.time.minute());
if ((thour<=9) && (tmin>9)){
    tclock = '0' + String(thour) +':'+ String(tmin);
    myDisplay.setTextAlignment(PA_CENTER);
myDisplay.print(tclock); 
  }
      else if ((thour>9) && (tmin<=9)){
    tclock = String(thour) + ':' +  '0' + String(tmin);
    myDisplay.setTextAlignment(PA_CENTER);
myDisplay.print(tclock); }
  else if ((tmin<=9) && (thour>9)){
    tclock = '0'+String(thour) +':' + String(tmin);
    myDisplay.setTextAlignment(PA_CENTER);
myDisplay.print(tclock); 
  }
  else if ((thour<=9) && (tmin<=9)){
    tclock = '0' + String(thour) +':'+ '0' + String(tmin);
    myDisplay.setTextAlignment(PA_CENTER);
myDisplay.print(tclock); 
  }
  else{
    tclock = String(thour) +':'+ String(tmin);
    myDisplay.setTextAlignment(PA_CENTER);
myDisplay.print(tclock); 
  }}}}}
  
  void gweedo(){
if (ss.available() > 0){
if (gps.encode(ss.read())) {
if (gps.speed.isValid()) {
 int tspeed =(gps.speed.kmph());
  myDisplay.setTextAlignment(PA_CENTER);
    myDisplay.print(tspeed);}
    else if(gps.speed.isValid()) {
 int tspeed =(gps.speed.kmph());
  myDisplay.setTextAlignment(PA_CENTER);
    myDisplay.print(tspeed);}}}}
