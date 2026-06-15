#include "Adafruit_VL53L0X.h"
#include <SPI.h>
#include <Wire.h>
                                                                                                                                                                                                                   
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
const int TooClose     = 4;
const int WatchIt      = 5;
const int GoodPark     = 6;
const int BUZZER_PIN = 7;

void setup() {
pinMode(TooClose,OUTPUT);
pinMode(WatchIt,OUTPUT);
pinMode(GoodPark,OUTPUT);
pinMode( BUZZER_PIN,OUTPUT);
SPI.begin(); // Init SPI bus
Wire.begin();
lox.begin();}

void loop() {
VL53L0X_RangingMeasurementData_t measure;
lox.rangingTest(&measure, false);
if (measure.RangeStatus != 4) {
if (measure.RangeStatus >=8190){ 
digitalWrite(TooClose,LOW);
digitalWrite(WatchIt,LOW);
digitalWrite(GoodPark,LOW);
}
if((measure.RangeMilliMeter)>=10&&(measure.RangeMilliMeter)<200){
digitalWrite(TooClose,HIGH);
digitalWrite(WatchIt,LOW);
digitalWrite(GoodPark,LOW);
tone(BUZZER_PIN, 1000, 1000);
} else if((measure.RangeMilliMeter)>=200&&(measure.RangeMilliMeter)<300){
digitalWrite(WatchIt,HIGH); 
digitalWrite(TooClose,LOW);
digitalWrite(GoodPark,LOW);
 }
else if((measure.RangeMilliMeter)>=300&&(measure.RangeMilliMeter)<400){
digitalWrite(WatchIt,LOW); 
digitalWrite(TooClose,LOW);
digitalWrite(GoodPark,HIGH);
}
else if((measure.RangeMilliMeter)>=400&&(measure.RangeMilliMeter)<8200){
digitalWrite(WatchIt,LOW); 
digitalWrite(TooClose,LOW);
digitalWrite(GoodPark,LOW);
}}}
