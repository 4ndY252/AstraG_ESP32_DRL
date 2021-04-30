#include <MPU6050_light.h>
#include "DHT.h"
#define pinDHT 5
#define typDHT22 DHT22
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <Fonts/Dialog13.h>
#include <Fonts/Dialog20.h>
#define OLED_RESET -1
Adafruit_SH1106 display(OLED_RESET);
DHT DHT(pinDHT, typDHT22);
MPU6050 mpu(Wire);
int temp;
int hum;
int val;
int accelx, accely, accelz;
float volts;

#define trigPin 11
#define echoPin 10

long duration;
int distance;
int prevDistance;

const float referenceVolts = 5.0;
const int inputPin = 0;

#define opel_width  64
#define opel_height 64



int period = 50;
unsigned long time_now = 0;


void setup(){
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  DHT.begin();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  mpu.begin();
  Wire.begin();
  //mpu.calcOffsets(true,true); //kalibrácia MPU6050
  display.clearDisplay();
  display.setFont(&Dialog_plain_15);
  display.setTextColor(WHITE);
}

void loop(){  
  val = analogRead(inputPin);
  volts = (val / 1023.0) * referenceVolts;

  temp = DHT.readTemperature();
  hum = DHT.readHumidity();
   
  if(volts > 1.5){
    parkSensor();
    beep();
  } else {
    noTone(2);
    if (temp < 10){
    naDisplej();
    noTone(2);
 } else{
    naDisplej();
    noTone(2);
  }
 }
  
}
void naDisplej(){
  
  display.setFont(&Dialog_plain_15);
  if(millis() >= time_now + period){
    time_now += period;
    mpu.update();

    display.clearDisplay();
    display.setCursor(0, 12);
    display.print("X:");
    
    display.setCursor(18, 12);
    display.print(mpu.getAccX(), 1);
    
    display.setCursor(51, 12); //"-" = 3px, char = 9px?
    display.print("G");
      
    display.setCursor(0, 38);
    display.print("Y:");
    
    display.setCursor(18, 38);   
    display.print(mpu.getAccY(), 1);
    
    display.setCursor(51, 38);
    display.print("G");

    display.setCursor(0, 64);
    display.println("Z:");

    display.setCursor(18, 64);
    display.print(mpu.getAccZ()-0.1, 1);
    
    display.setCursor(51, 64);
    display.print("G");

    display.setCursor(70, 12);
    display.print("T:");

    display.setCursor(88, 12);
    display.print(temp);

    display.setCursor(117, 12);
    display.print("C");

    display.setCursor(70, 38);
    display.print("V:");

    display.setCursor(88, 38);
    display.print(hum);

    display.setCursor(113, 38);
    display.print("%");
    
    display.display();
    }    
}

void parkSensor(){
  display.clearDisplay();
  digitalWrite(trigPin, LOW);
  
  delayMicroseconds(5);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  
  distance = (duration*0.034/2)+2;
  //cas echoPinu na high * rychlost zvuku/2 + offset
  
  display.setFont(&Dialog_plain_20);
  if(distance > 100 && distance < 450){
    display.setCursor(30, 38);
    display.print(distance);
    display.setCursor(71, 38);
    display.print("CM");
   } else if(distance < 100 && distance > 25){
    display.setCursor(38, 38);
    display.print(distance);    
    display.setCursor(66, 38);
    display.print("CM");
   } else if(distance <= 25){    
    display.setCursor(37, 38);
    display.print("STOP");
   }
  
   display.display();
  delay(100);  

}

void beep(){
  if(distance > 30){
    noTone(2);
  } else if(distance < 31 && distance > 25){
    tone(2, 1000);
    delay(200);
    noTone(2);
    delay(200);
  } else if(distance < 26){
    tone(2, 1000);
  }
}
