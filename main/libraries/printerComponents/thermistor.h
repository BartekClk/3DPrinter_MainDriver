#ifndef thermistor_h
#define thermistor_h

    #include <Arduino.h>
    #include "../VPpin.h"
    #include "../VPBubleSort.h"

    class Thermistor
    {
    private:
    Pin  *thermistorPin;

    // int thermistorSamples[10]; // Tablica przechowująca próbki

    // Stałe do konwersji napięcia na temperaturę
    float serialResistance = 100000;     // Wartość rezystora w szeregu z termistorem (Ohm)
    float nominalResistance = 100000; // Nominalna wartość rezystancji termistora (Ohm)
    float Vcc = 5.0;      // Napięcie zasilania (V)
    float adcResolution = 1023.0;  // Rozdzielczość przetwornika analogowo-cyfrowego

    // Parametry termistora
    float bCoef = 3950; 

    float x_est = 0; // Estimated state
    double P_est = 1; // Estimated error covariance
    double Q = 0.01;  // Process noise covariance
    double R = 4;     // Measurement noise covariance
    bool firstTemp = true;

    public:
    // Loop *loop = new Loop(100);

    Thermistor(int pin, float nomRes, float bCoef, float serialRes){
        this->thermistorPin = new Pin(pin, 0);
        this->nominalResistance = nomRes;
        this->bCoef = bCoef;
        this->serialResistance = serialRes;
    }

    float read(){
        int sensorValue = thermistorPin->readPinAnalog();

        // Obliczanie oporu termistora
        float resistance = serialResistance * (adcResolution / sensorValue - 1);

        // Obliczanie temperatury w stopniach Celsjusza
        float temperature = 1.0 / ((1.0 / 298.15) + (1.0 / bCoef) * log(resistance / nominalResistance)) - 273.15;

        return temperature;
    }

    int setAvg(){
        float z = read();
        if (z > x_est-2 || z < x_est+2 || firstTemp == true){
            firstTemp = false;
            double x_pred = x_est;
            double P_pred = P_est + Q;

            // Measurement update
            double K = P_pred / (P_pred + R);
            x_est = x_pred + K * (z - x_pred);
            P_est = (1 - K) * P_pred;
            return x_est*100;
        }else{
            return x_est*100;
        }
    }
    
    int getTemp(){
        return x_est;
    }

    void def(){
        thermistorPin->defPin();
    }
    int getPin(){
        return thermistorPin->getPin();
    }
    };

#endif