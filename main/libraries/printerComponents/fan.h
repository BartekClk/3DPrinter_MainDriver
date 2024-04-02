#ifndef fan_h
#define fan_h

#include <Arduino.h>
#include "../VPpin.h"

class Fan{ //Class for defining fans
  protected:
    int speed = 0;
    Pin *pin;

  public:
    Fan(byte pin){
      this->pin = new Pin(pin, 1, LOW);
    }
    // void setSpeed(int speed){
    //   this->speed = speed;
    // }
    void setSpeed(int speed){ // 0 = off, and steps from 1 to 5;
      if(speed <= 0) this->speed = 0;
      else this->speed = speed*37+70;
      if(this->speed > 255) this->speed = 255;
    }
    void def(){ 
      pin->defPin();
    }
    void writeSpeed(){
      // if(speed > 1) pin->setPin(int(speed));
      // else pin->setPin(byte(speed));
      pin->setPin(speed);
    }
};

#endif