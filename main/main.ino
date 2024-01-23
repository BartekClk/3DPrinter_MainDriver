#include <Arduino.h>
#include <stdint.h>

class Loop{
  private:
    static unsigned long actualTime;
  protected:
    unsigned long deltaTime = 0;
    unsigned long lastTime = 0;

  public:
    Loop(){
      lastTime = actualTime;
    }


    static unsigned long getActualTime(){return actualTime;}
    static void setActualTime(unsigned long time){actualTime = time;}
};
unsigned long Loop::actualTime = 0;

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

    void setPin(byte state){
      digitalWrite(pin, state);
    }
};

class Fan{ //Class for defining fans
  protected:
    int speed = 0;
    Pin *pin;

  public:
    Fan(byte pin){
      this->pin = new Pin(pin, 1);
    }
    void setSpeed(int speed){
      this->speed = speed;
    }
    void def(){ 
      pin->defPin();
    }
};

class Heater{ //Class for defining heaters
  protected:
    int actualTemp = 10;
    int wantedTemp = 60;
    bool isHeating = 0;
    Pin *pin;

  public:
    Heater(byte pin){
      this->pin = new Pin(pin, 1);
    }
    
  void def(){
      pin->defPin();
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

  bool getIsHeating(){return isHeating;}
};

class HeatBed : public Heater{
  private:
  public:
    HeatBed(byte transistorPin) : Heater(transistorPin){}
};

class HotEnd : public Heater{
  private:
  public:
    HotEnd(byte transistorPin) : Heater(transistorPin){}
};

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

class Motor{
  private:
    Pin *dirPin;
    Pin *stepPin;
    Pin *enablePin;
    float mmPerStep;
    
    public:
      Motor(byte dirPin, byte stepPin, byte enablePin, float mmPerStep){
        this->dirPin = new Pin(dirPin, 1);
        this->stepPin = new Pin(stepPin, 1);
        this->enablePin = new Pin(enablePin, 1);
        this->mmPerStep = mmPerStep;
      }

    void def(byte dir = 0, byte enable = 0, byte ms = 0){
      dirPin->defPin();
      dirPin->setPin(dir);

      stepPin->defPin();

      enablePin->defPin();
      enablePin->setPin(enable);
    }
};

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
};

class Printer{
  private:
    HeatBed *heatBed = new HeatBed(22);
    HotEnd *hotEnd = new HotEnd(23);

    Fan *hotEndFan = new Fan(2);
    Fan *printFan = new Fan(3);



    Motor *xMotor = new Motor(5,6,7, 0.29215);

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
    limiters[0][0] = new Limiter(a[0], "y-front");
    limiters[0][1] = new Limiter(a[1], "y-back");

    limiters[1][0] = new Limiter(a[2], "x-left");
    limiters[1][1] = new Limiter(a[3], "x-right");

    limiters[2][0] = new Limiter(a[4], "z-top");
    limiters[2][1] = new Limiter(a[5], "z-bottom");
  }

  int getBedT(bool a=0){return heatBed->getTemp(a);}
  int getHotEndT(bool a=0){return hotEnd->getTemp(a);}
  int getPrintSpeed(){return printSpeed;}
  bool getendStopstatus(byte row, byte collumn){return limiters[row][collumn]->getStatus();}

  void def(){
    heatBed->def();
    hotEnd->def();
    hotEndFan->def();
    printFan->def();
    setLimiters(new byte[6]{26,27,28,29,30,31});
  }
};

Printer *printer = new Printer();


unsigned long lastTime = 0;
unsigned long deltaTime = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Hello World");
  printer->def();
}

Loop loop1 = Loop();

void loop() {
  // Loop::setActualTime(millis());

  // deltaTime = actualTime - lastTime;

  // if (deltaTime >= 1000UL){
  //   Serial.println(actualTime);
  //   lastTime = actualTime;
  // }
}
