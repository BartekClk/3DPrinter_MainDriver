#ifndef stepper_h
#define stepper_h

#include <Arduino.h>
#include "../VPpin.h"

class Stepper{
  private:
    static unsigned long actualTime;

  public:
    Pin* dirPin;
    Pin* stepPin;
    Pin* enablePin;

    uint8_t stepPulseDelay;

    uint8_t microstepMode;

    byte direction = 1;
    byte reversedDirection;

    float mmPerSecond; 
    float mmPerStep;
    float stepsPerMillimeter;

    uint32_t stepsRemaining = 0;
    uint32_t stepsToDo = 0;

    float stepInterval = 1000;
    uint32_t targetStepInterval = 1000;
    float accelerationFactor = 3; // Acceleration factor (adjust as needed)
    int targetAccelerationSteps = 10; // n of steps that will be smoothing start and stop of a motor   
    int accelerationSteps = 0; // n of steps that will be smoothing start and stop of a motor
    float intervalModifier = 0;

    unsigned long previousStepTime = 0;

    static void update(){
      actualTime = micros();
    }

    Stepper(uint8_t dirPin, 
            uint8_t stepPin, 
            uint8_t enablePin, 
            float mmPerRev, 
            int stepsPerRev, 
            uint8_t microstepMode = 1, 
            float accelerationFactor = 1.5, 
            int targetAccelerationSteps = 1000,
            byte reversedDirection = 1,
            uint8_t stepPulseDelay = 2){

      this->dirPin = new Pin(dirPin, 1);
      this->stepPin = new Pin(stepPin, 1);
      this->enablePin = new Pin(enablePin, 1, HIGH);

      this->stepPulseDelay = stepPulseDelay;

      this->reversedDirection = reversedDirection;

      this->microstepMode = microstepMode;

      this->mmPerStep = mmPerRev/float(stepsPerRev);
      this->stepsPerMillimeter = 1/mmPerStep;

      this->accelerationFactor = accelerationFactor;
      this->targetAccelerationSteps = targetAccelerationSteps;

      this->setup();
    }

    void setup(){
      this->stepPin->defPin();
      this->enablePin->defPin();
      this->enablePin->defPin();
      this->setDir(1);
    }

    void enable(){
      this->enablePin->setPin(LOW);
    }

    void disable(){
      this->enablePin->setPin(HIGH);
    }

    void setDir(byte dir){ // -1 = backward, 1 = forward
      this->direction = dir*reversedDirection;
      switch (this->direction)
      {
      case 1:
        this->dirPin->setPin(HIGH);
        break;
      case -1:
        this->dirPin->setPin(LOW);
        break;
      default:
        this->dirPin->setPin(HIGH);
        break;
      }
    }

    void setSpeed(float mmPerSecond){ // speed in mm/s
      targetStepInterval = 1000000 / abs(mmPerSecond*stepsPerMillimeter);
      stepInterval = targetStepInterval*(accelerationFactor);
      // Serial.println("stepPulseDelay: "+String(stepPulseDelay));
      // Serial.println("mmPerSecond: "+String(mmPerSecond));
      // Serial.println("mmPerStep: "+String(mmPerStep));
      // Serial.println("stepsPerMillimeter: "+String(stepsPerMillimeter));
      // Serial.println("stepsRemaining: "+String(stepsRemaining));
      // Serial.println("stepsToDo: "+String(stepsToDo));
      // Serial.println("stepInterval: "+String(stepInterval));
      // Serial.println("targetStepInterval: "+String(targetStepInterval));
      // Serial.println("accelerationFactor: "+String(accelerationFactor));
      // Serial.println("targetAccelerationSteps: "+String(targetAccelerationSteps));
      // Serial.println("accelerationSteps: "+String(accelerationSteps));
      // Serial.println("intervalModifier: "+String(intervalModifier));
    }

    void move(float mm){
      stepsRemaining = mm*stepsPerMillimeter*microstepMode;
      stepsToDo = stepsRemaining;

      if(accelerationSteps>(stepsToDo/2)) accelerationSteps = stepsToDo/2;
      else accelerationSteps = targetAccelerationSteps;

      intervalModifier = ((long(stepInterval)-long(targetStepInterval))/float(accelerationSteps));
    }

    void asyncStep(){
      if (actualTime - previousStepTime >= stepInterval && stepsRemaining > 0) {
        
        if (accelerationSteps>(stepsToDo-stepsRemaining)){
          if(stepInterval>targetStepInterval) stepInterval -= intervalModifier;
        }
        else if((stepsToDo-(stepsToDo-stepsRemaining))<=accelerationSteps && stepsRemaining!=0){
          stepInterval += intervalModifier;
        }

        previousStepTime = actualTime;
        stepPin->setPin(HIGH);
        delayMicroseconds(stepPulseDelay);
        stepPin->setPin(LOW);
        stepsRemaining--;
      }
    }


  // protected:
  //   unsigned long deltaTime = 0;
  //   unsigned long lastTime = 0;
  //   unsigned long time = 0;
  //   int repeatFor;

  // private:
  //   Pin *dirPin;
  //   Pin *stepPin;
  //   Pin *enablePin;

  //   float mmPerStep;
  //   uint64_t steps = 0;
  //   uint64_t stepsRemaining = 0;
  //   bool stepsDone = 1;
  //   bool reverseDir = 0;
    
  // public:
  //   static void update(){
  //     actualTime = micros();
  //   }
  //   Motor(byte dirPin, byte stepPin, byte enablePin, float mmPerStep){
  //     this->dirPin = new Pin(dirPin, 1);
  //     this->stepPin = new Pin(stepPin, 1);
  //     this->enablePin = new Pin(enablePin, 1);
  //     this->mmPerStep = mmPerStep;
  //   }
  //   void def(){
  //     dirPin->defPin();
  //     stepPin->defPin();
  //     enablePin->defPin();
  //     enablePin->setPin(0);
  //   }
  //   void setDir(byte dir){
  //     if(reverseDir) dirPin->setPin(!int(dir));
  //     else dirPin->setPin(dir);
  //   }
  //   void move(uint64_t steps){
  //     this->stepsRemaining = steps;
  //   }
  //   void setSpeed(int time){
  //     this->time = time;
  //   }
  //   void step(){
  //     if (stepsRemaining > 0){
  //     deltaTime = actualTime - lastTime;
  //     if (deltaTime>=time){
  //       setDir(0);      
  //         steps++;
  //         stepsRemaining--;
  //         stepPin->setPin(1);
  //         delayMicroseconds(5);
  //         stepPin->setPin(0);
  //       lastTime = actualTime;
  //     }
  //     }else if (stepsRemaining < 0){
  //     }
  //   }
};
unsigned long Stepper::actualTime = 0;

#endif