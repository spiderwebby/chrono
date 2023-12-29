#include <Arduino.h>

// put function declarations here:
float calcEnergy(float, float, float);
float calcFPS(float, float);
float calcMPS(float, float);
void firstGateISR();
void secondGateISR();


const float dartMass = 0.0013; //Kg 1.3g
const int dist_chrono = 100; //10cm


const int ledPin = 13;
const int firstGatePin = 2;
const int secondGatePin = 3;
volatile bool detectected = 0;
volatile long time1 = 0;
volatile long time2 = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(firstGatePin, INPUT_PULLUP);
  pinMode(secondGatePin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(firstGatePin), firstGateISR, FALLING);
  Serial.begin(9600);
}

void loop() {
  if(detectected){
    noInterrupts();
    detectected=0;
    long deltaT=time2-time1;
    if(deltaT>500 && deltaT<100000){
      Serial.print(deltaT);
      Serial.print("us");
      Serial.print(", ");
      Serial.print(calcFPS(dist_chrono, deltaT),5);
      Serial.print("FPS, ");
      Serial.print(calcMPS(dist_chrono, deltaT),5);
      Serial.print("m/s, ");
      Serial.print(calcEnergy(dist_chrono, deltaT, dartMass),5);
      Serial.println("J");
    }
    delay(1000);
    interrupts();
    attachInterrupt(digitalPinToInterrupt(firstGatePin), firstGateISR, FALLING);
  }
}

void firstGateISR() {
  detachInterrupt(digitalPinToInterrupt(firstGatePin));
  time1 = micros();
  attachInterrupt(digitalPinToInterrupt(secondGatePin), secondGateISR, FALLING);
}

void secondGateISR() {
  detachInterrupt(digitalPinToInterrupt(secondGatePin));
  time2 = micros();
  detectected = 1;
}



float calcEnergy(float dist, float time, float mass){
  return (0.5*mass*powf(calcMPS(dist,time),2));
}

float calcFPS(float dist, float time){
  return calcMPS(dist,time)*3.28084;
}

float calcMPS(float dist, float time){
  return ((dist/1000)/(time/1000000));
}