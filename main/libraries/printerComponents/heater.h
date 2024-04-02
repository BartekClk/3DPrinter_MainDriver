#ifndef heater_h
#define heater_h

#include <Arduino.h>
#include "../VPpin.h"
#include "./thermistor.h"


class Heater{ //Class for defining heaters
  protected:
    int actualTemp = 10;
    int wantedTemp = 0;
    bool isHeating = 0;
    bool tempReached = 0;
    bool work = 0;
    Pin *pin;
    Thermistor *thermistor;

  public:
    Heater(byte pin, byte thermistorPin){
      this->pin = new Pin(pin, 1, LOW);
      this->thermistor = new Thermistor(thermistorPin, 100000, 3950, 100000);
    }
    
  void def(){
      pin->defPin();
      thermistor->def();
    }

  int getTemp(bool a=0){ // 0 = actualTemp, 1 = wantedTemp;
    switch(a){
      case 0:
        return actualTemp;
        break;
      case 1:
        return wantedTemp;
        break;
    }
  }

  void setTemp(int temp){
    wantedTemp = temp;
  }

  int heat(){
    if (work == 1){    
      if(actualTemp < wantedTemp*100+20){
        isHeating = 1;
        pin->setPin(HIGH);
      }else{
        isHeating = 0;
        pin->setPin(LOW);
      }
    }else{
      isHeating = 0;
      pin->setPin(LOW);
    }
  }

  void readTemp(){
    actualTemp = thermistor->setAvg();
    if (actualTemp > wantedTemp*100-200 && actualTemp < wantedTemp*100+200){
      tempReached = 1;
    }
  }

  bool getIsHeating(){return isHeating;}

  int getPin(){return pin->getPin();}

  void setWork(bool a){
    work = a;
  }
};

class HeatBed : public Heater{
  private:
  public:
    HeatBed(byte transistorPin, byte thermistorPin) : Heater(transistorPin, thermistorPin){}
};

class HotEnd : public Heater{
  private:
  public:
    HotEnd(byte transistorPin, byte thermistorPin) : Heater(transistorPin, thermistorPin){}
};

#endif