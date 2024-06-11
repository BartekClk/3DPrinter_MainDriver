#include <Arduino.h>
#include <Async_Operations.h>

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

  printer->Ms->setMs(1);
  printer->xStepper->enable();

  printer->xStepper->setSpeed(10);
  printer->xStepper->move(100);
}

int heating =0;

unsigned long micros1;
unsigned long micros2;

uint8_t operation = 0;
uint8_t maxOperation = 1000;

void loop() {
  Loop::startLoop();
  Stepper::update();

  printer->xStepper->asyncStep();

  // switch (operation)
  // {
  // case 0:
  //   // printer->printFan->writeSpeed();
  //   break;
  // case 1:
  //   // printer->hotEndFan->writeSpeed();
  //   // printer->readTemps();
  //   // printer->heat();
  //   break;
  // case 2:
  //   // printer->readTemps();
  //   // printer->heat();
  //   break;
  // case 3:
  //   // printer->readTemps();
  //   // printer->heat();
  //   break;
  // default:
  //   break;
  // }


  // digitalWrite(31, HIGH);
  // delayMicroseconds(10);
  // digitalWrite(31, LOW);
  // delayMicroseconds(20);
  // micros1 = micros();
  // Loop::startLoop();
  // Motor::startLoop();


  millis1000.action([](){
    // printer->xMotor->move(6400);
    printer->printFan->writeSpeed();
    printer->hotEndFan->writeSpeed();

    // printer->readTemps();
    // printer->heat();

    // if (Serial.available() > 0) {
    //   String incomingData = Serial.readString();
    //   if (incomingData == "h"){
    //     if (heating == 0){
    //       printer->hotEnd->setWork(1);
    //       printer->heatBed->setWork(1);
    //       heating = 1;
    //     }else if (heating == 1){
    //       printer->hotEnd->setWork(0);
    //       printer->heatBed->setWork(0);
    //       heating = 0;
    //     }
    //   }
    //   if (incomingData == "m"){
    //     // printer->xMotor->setDir(1);
    //     // printer->xMotor->setSpeed(0);
    //   }
    // }
  });
  // half.action([](){
  //   printer->xMotor->step();
    // Serial.write(27);       // ESC command
    // Serial.print("[2J");    // clear screen command
    // Serial.write(27);
    // Serial.print("[H");     // cursor to home command
    // printer->print();
    // Serial.println("Code duration:\t"+String(micros2)+"μS");
  //   printer->xMotor->step();
  // });
  millis100.action([](){
    long micros1 = micros();
    long micros2;
    // double x = micros();
    // printer->readTemps();
    printer->heatBed->thermistor->read();
    printer->heat();
    micros2 = micros() - micros1;
    Serial.println("Code duration:\t"+String(printer->heatBed->thermistor->x_est)+"μS");
  });
  // micros2 = micros() - micros1;
  // printer->xMotor->step();
  // Serial.write(27);       // ESC command
  // Serial.print("[2J");    // clear screen command
  // Serial.write(27);
  // Serial.print("[H");     // cursor to home command
  // Serial.println("Code duration:\t"+"μS");

  if (operation >= maxOperation){
    operation = 0;
  }
  operation++;
}
