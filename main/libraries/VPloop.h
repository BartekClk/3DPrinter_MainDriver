/*
    VPLoop.h - Library for creating millis() based loops
    Created by V-Projects - vprojects.pl, 2024
    Coded by Bartłomiej Celak, 2024
    GitHub: https://github.com/bartekclk
*/
#ifndef VPloop_h
#define VPloop_h

#include <stdint.h>

class Loop{
  private:
    static unsigned long actualTime;
  protected:
    unsigned long deltaTime = 0;
    unsigned long lastTime = 0;
    unsigned long time = 0;
    int repeatFor;

  public:
    static void startLoop(){
      actualTime = millis();
    }
    void action(void (*func)()){
      deltaTime = actualTime - lastTime;
      if (deltaTime>=time){
        func();
        lastTime = actualTime;
      }
    }
    Loop(unsigned long time, int repeatFor = 0){
      this->time = time;
      this->repeatFor = repeatFor;
    }
    static unsigned long getActualTime(){return actualTime;}
    static void setActualTime(unsigned long time){actualTime = time;}
};
unsigned long Loop::actualTime = 0;

#endif