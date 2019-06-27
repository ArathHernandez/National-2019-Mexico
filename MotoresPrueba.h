  #ifndef MotoresB_h
#define MotoresB_h

#include "arduino.h"
#include <PID_v1.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include <Adafruit_MotorShield.h>
#define BNO055_SAMPLERATE_DELAY_MS 10

class MotoresPrueba {

public:
    MotoresPrueba();
    Adafruit_DCMotor *MotorAdeIzq;
    Adafruit_DCMotor *MotorAdeDer;
    Adafruit_DCMotor *MotorAtrasIzq;
    Adafruit_DCMotor *MotorAtrasDer;
    Adafruit_MotorShield AFMS;
    Adafruit_BNO055 bno;
    void moveAdelante();
    void moveAtras();
    void moveIzq();
    void moveDer();
    void detenerse();
    void setup();
    void moveAdelanteLento();
    void moveAtrasLento();
    void acomodoI();
    void acomodoD();
    void moveDerAcomodo();
    void moveIzqAcomodo();
   // Adafruit_BNO055 bno;
};


#endif
