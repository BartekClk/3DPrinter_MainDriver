#ifndef motor_h
#define motor_h

#include <Arduino.h>
#include <Stepper.h>
#include "../VPpin.h"

class Motor{
  private:
    Pin *dirPin;
    Pin *stepPin;
    Pin *enablePin;
    float mmPerStep;
    int ms;
    Stepper *motor;
    
    public:
      Motor(byte dirPin, byte stepPin, byte enablePin, float mmPerStep, int ms=0){
        this->dirPin = new Pin(dirPin, 1);
        this->stepPin = new Pin(stepPin, 1);
        this->enablePin = new Pin(enablePin, 1);
        this->mmPerStep = mmPerStep;
        this->ms = ms;
      }

    void def(byte dir = 0, byte enable = 0, byte ms = 0){
      dirPin->defPin();
      dirPin->setPin(dir);

      stepPin->defPin();

      enablePin->defPin();
      enablePin->setPin(enable);
    }

    void move(int steps){
      
    }
};

#endif