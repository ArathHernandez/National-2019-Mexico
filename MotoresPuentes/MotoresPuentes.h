    #ifndef MotoresPuentes_h
#define MotoresPuentes_h

#include "arduino.h"
#include <PID_v1.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <LiquidCrystal_I2C.h>


#define BNO055_SAMPLERATE_DELAY_MS 10

class MotoresPuentes {

public:
    MotoresPuentes();
    Adafruit_BNO055 bno;
    void moveAdelante();
    void moveAdelanteFast();
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
    void acomodarseChoqueIzquierda1();
    void acomodarseChoqueIzquierda2();
    void acomodarseChoqueDerecha1();
    void acomodarseChoqueDerecha2();
    void actualizaSetpoint();
    const int motorIzqAde1  = 9;
    const int motorIzqAde2  = 8;
    const int motorIzqAtras1  = 11;//l
    const int motorIzqAtras2  = 10;//l
    const int motorDerAde1  = 5;//L
    const int motorDerAde2  = 4;//L
    const int motorDerAtras1  = 7;//
    const int motorDerAtras2  = 6;//
    double getSetpoint(double);

   // Adafruit_BNO055 bno;
};


#endif
