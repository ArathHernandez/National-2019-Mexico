 #ifndef MotoresPuentes_h
#define MotoresPuentes_h

#include "arduino.h"
#include <PID_v1.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>


#define BNO055_SAMPLERATE_DELAY_MS 10

class MotoresPuentesDist {

public:
    MotoresPuentesDist();
    Adafruit_BNO055 bno;
    void moveAdelante(int, int);
    void moveAtras();
    void moveIzq();
    void moveDer();
    void detenerse();
    void setup();
    void moveAtrasLento();
    void acomodoI();
    void acomodoD();
    void moveDerAcomodo();
    void moveIzqAcomodo();
    void actualizaSetpoint();
    const int motorIzqAde1  = 4;
    const int motorIzqAde2  = 5;
    const int motorIzqAtras1  = 6;
    const int motorIzqAtras2  = 7;
    const int motorDerAde1  = 8;
    const int motorDerAde2  = 9;
    const int motorDerAtras1  = 10;
    const int motorDerAtras2  = 11;

   // Adafruit_BNO055 bno;
};


#endif
