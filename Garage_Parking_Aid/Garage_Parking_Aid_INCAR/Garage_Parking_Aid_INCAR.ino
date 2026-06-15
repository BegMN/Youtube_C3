#include <Wire.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <VL53L0X.h>  //pololu
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10
MD_Parola BlueDots = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
VL53L0X vlsensor;
String mixer;
void setup(){
Wire.begin();
BlueDots.begin();
BlueDots.setIntensity(20);
BlueDots.displayClear();
BlueDots.print("noBang!");
delay(1000);
vlsensor.setTimeout(500);
if(!vlsensor.init()){
  while(1);}
vlsensor.startContinuous();}

void loop()
{
int distance = vlsensor.readRangeContinuousMillimeters();
if(vlsensor.readRangeContinuousMillimeters()>=8190){
BlueDots.displayClear();
BlueDots.setTextAlignment(PA_CENTER);
BlueDots.displayAnimate();
BlueDots.print("invalid");}
else if(vlsensor.readRangeContinuousMillimeters()<=2000){
BlueDots.displayClear();
BlueDots.setTextAlignment   (PA_CENTER);
BlueDots.displayAnimate();
mixer =String((distance/10)) +"cm";
BlueDots.print(mixer);}
else  if (vlsensor.timeoutOccurred()){
BlueDots.displayClear();
BlueDots.setTextAlignment(PA_CENTER);
BlueDots.displayAnimate();
BlueDots.print("timeout");
}}                                                                                                                                                                                                                                                                            
