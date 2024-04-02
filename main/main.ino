#include <Arduino.h>

#include "./libraries/VPloop.h"
#include "./libraries/VPpin.h"
#include "./libraries/VP3Dprinter.h"

Printer *printer = new Printer();

Loop half = Loop(500, 0);
Loop millis10 = Loop(10, 0);
Loop millis100 = Loop(100, 0);
Loop millis1000 = Loop(1000, 0);

void setup() {
  Serial.begin(115200);
  Serial.println("\"Welcome back sir!\" - 3D Printer");
  printer->def();
  printer->heatBed->setTemp(60);
  printer->hotEnd->setTemp(210);
  printer->hotEndFan->setSpeed(1);
  printer->printFan->setSpeed(0);
}

int heating =0;

void loop() {
  Loop::startLoop();
  millis1000.action([](){
  analogWrite(A3, 2.7);
    printer->printFan->writeSpeed();
    printer->hotEndFan->writeSpeed();
  });
  half.action([](){
    Serial.write(27);       // ESC command
    Serial.print("[2J");    // clear screen command
    Serial.write(27);
    Serial.print("[H");     // cursor to home command
    printer->print();
  });
  millis10.action([](){
    printer->readTemps();
    printer->heat();
  });
  if (Serial.available() > 0) {
    String incomingData = Serial.readString();
    if (incomingData == "h"){
      if (heating == 0){
        printer->hotEnd->setWork(1);
        printer->heatBed->setWork(1);
        heating = 1;
      }else if (heating == 1){
        printer->hotEnd->setWork(0);
        printer->heatBed->setWork(0);
        heating = 0;
      }
    }
    if (incomingData == "m"){
      /* code */
    }
    
    
  }
}
