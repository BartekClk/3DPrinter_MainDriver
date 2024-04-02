/*
    VP3Dprinter.h - Library for creating 3D printers on Arduino
    Created by V-Projects - vprojects.pl, 2024
    Coded by Bartłomiej Celak, 2024
    GitHub: https://github.com/bartekclk
*/
#ifndef VP3Dprinter_h
#define VP3Dprinter_h

#include <Arduino.h>

#include "./VPloop.h"
#include "./VPpin.h"

#include "./printerComponents/fan.h"
#include "./printerComponents/heater.h"
#include "./printerComponents/limiter.h"
#include "./printerComponents/microSteps.h"
#include "./printerComponents/motor.h"

class Printer{
  public:
    HotEnd *hotEnd = new HotEnd(11, A1);
    HeatBed *heatBed = new HeatBed(12, A2);

    Fan *hotEndFan = new Fan(10);
    Fan *printFan = new Fan(9);

    // dir, step, enable, mmPerStep, ms
    Motor *xMotor = new Motor(32,31,30, 0.01875, 4);
    Motor *yMotor = new Motor(35,34,33, 0.0025, 4);
    Motor *zMotor1 = new Motor(38,37,36, 0.005, 1);
    Motor *zMotor2 = new Motor(41,40,39, 0.005, 1);
    Motor *eMotor = new Motor(44,43,42, 0.01);

    MicroSteps *Ms = new MicroSteps(new byte[3]{27,28,29});

    Loop *loop = new Loop(1000, 0);

    int printSpeed = 1;
    bool absoluteExtrusion = true;
    bool imperialUnits = false;
    bool absolutePositioning = true;

    float AxisPosition[3] = {0,0,0}; //x,y,z
    float AxisPositionHome[3] = {0,0,0}; //x,y,z
    bool positionTrusted = false;
    bool levelingAfterHome = true;
  public:
    Limiter *limiters[3][2] = {
      {}, //x dimmension
      {}, //y dimmension
      {}  //z dimmension
    };

    Printer(){

    }

  void setLimiters(byte a[]){
    limiters[0][1] = new Limiter(a[1], "y");

    limiters[1][0] = new Limiter(a[2], "x");

    limiters[2][1] = new Limiter(a[5], "z");
  }

  int getBedT(bool a=0){return heatBed->getTemp(a);}
  int getHotEndT(bool a=0){return hotEnd->getTemp(a);}
  int getPrintSpeed(){return printSpeed;}
  bool getLimiterStatus(byte row, byte collumn){return limiters[row][collumn]->getStatus();}

  void def(){
    heatBed->def();
    hotEnd->def();
    hotEndFan->def();
    printFan->def();
    setLimiters(new byte[3]{26,27,28});
  }
  void heat(){
    hotEnd->heat();
    heatBed->heat();
  }
  void readTemps(){
    hotEnd->readTemp();
    heatBed->readTemp();
  }
  void print(){
    Serial.print("HotEnd:\t");
    Serial.print(hotEnd->getTemp()/100);
    Serial.print("°C");
    Serial.print("\tHeatBed: ");
    Serial.print(heatBed->getTemp()/100);
    Serial.print("°C");
    Serial.print("\n");
  }
};

#endif