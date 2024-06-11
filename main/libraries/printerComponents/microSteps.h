#ifndef microSteps_h
#define microSteps_h

#include <Arduino.h>
#include "../VPpin.h"

class MicroSteps{
  private:
    Pin *pin[3];
  public:
    MicroSteps(byte msPins[3]){
      pin[0] = new Pin(msPins[0], 1);
      pin[1] = new Pin(msPins[1], 1);
      pin[2] = new Pin(msPins[2], 1);
    }

    void setMs(byte ms=0){ // 0 = full step, 1 = half step, 2 = 1/4 step, 3 = 1/8 step, 4 = 1/16 step
      switch (ms)
      {
      case 0: // full step
        pin[0]->setPin(0);
        pin[1]->setPin(0);
        pin[2]->setPin(0);
        break;
      case 1: // half step
        pin[0]->setPin(1);
        pin[1]->setPin(0);
        pin[2]->setPin(0);
        break;
      case 2: // 1/4 step
        pin[0]->setPin(0);
        pin[1]->setPin(1);
        pin[2]->setPin(0);
        break;
      case 3: // 1/8 step
        pin[0]->setPin(1);
        pin[1]->setPin(1);
        pin[2]->setPin(0);
        break;
      case 4: // 1/16 step
        pin[0]->setPin(0);
        pin[1]->setPin(0);
        pin[2]->setPin(1);
        break;
      case 5: // 1/32 step
        pin[0]->setPin(1);
        pin[1]->setPin(1);
        pin[2]->setPin(1);
        break;
      default: // full step
        pin[0]->setPin(0);
        pin[1]->setPin(0);
        pin[2]->setPin(0);
        break;
      }
    }
    void def(){
      setMs(1);
    }
};

#endif