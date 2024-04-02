#ifndef limiter_h
#define limiter_h

#include <Arduino.h>
#include "../VPpin.h"

class Limiter{
  private:
    String name;
    bool status = 0;
    Pin *pin;
  public:
    Limiter(byte pin, String name = "unnamed"){
      this->pin = new Pin(pin, 2);

      if (name=="unnamed") this->name = "unnamed";
      else this->name = name;
    }

    bool getStatus(){
        status = pin->readPin();
        return status;
    }

    String getName(){return name;}
};

#endif