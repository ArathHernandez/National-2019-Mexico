 #ifndef Rampa_h
#define Rampa_h

#include "arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include <Adafruit_MotorShield.h>
#define BNO055_SAMPLERATE_DELAY_MS 10

class Rampa {

public:
    Rampa();
    Adafruit_BNO055 bno;
    void setup();
    int detectaRampa();
    const int motorIzqAde1  = 8;
    const int motorIzqAde2  = 9;
    const int motorIzqAtras1  = 11;//l
    const int motorIzqAtras2  = 10;//l
    const int motorDerAde1  = 5;//L
    const int motorDerAde2  = 4;//L
    const int motorDerAtras1  = 7;//
    const int motorDerAtras2  = 6;//

};


#endif
