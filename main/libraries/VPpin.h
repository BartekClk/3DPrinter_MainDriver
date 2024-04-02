/*
    VPpin.h - Library for creating pin objects
    Created by V-Projects - vprojects.pl, 2024
    Coded by Bartłomiej Celak, 2024
    GitHub: https://github.com/bartekclk
*/
#ifndef VPpin_h
#define VPpin_h

#include <Arduino.h>

class Pin{ //Class for defining pins
  protected:
    byte pin;
    int mode; // 0 = INPUT, 1 = OUTPUT, 2 = INPUT_PULLUP
    byte defState;
  public:
    Pin(byte pin, int mode, byte defState=0){
      this->pin = pin;
      this->mode = mode;
      this->defState = defState;
    }

    byte getPin(){return pin;}

    void defPin(){
      switch (mode)
      {
      case 0:
        pinMode(pin, INPUT);
        break;
      case 1:
        pinMode(pin, OUTPUT);
        digitalWrite(pin, defState);
        break;
      case 2:
        pinMode(pin, INPUT_PULLUP);
        break;
      }
      if(defState == 1) digitalWrite(pin, HIGH);
      else digitalWrite(pin, LOW);
    }

    int readPin(){
      return digitalRead(pin);
    }

    int readPinAnalog(){
      return analogRead(pin);
    }

    bool readPinState(){
      return digitalRead(pin);
    }

    void setPin(int state){
      if (state <= 1) digitalWrite(pin, state);
      else analogWrite(pin, state);
    }

    // void setPin(int state){
    //   analogWrite(pin, state);
    // }
};

#endif