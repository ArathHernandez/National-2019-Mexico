#include "MotoresB.h"




MotoresB::MotoresB(){
bno = Adafruit_BNO055();
AFMS = Adafruit_MotorShield();
MotorAdeIzq = AFMS.getMotor(4);
MotorAdeDer = AFMS.getMotor(1);
MotorAtrasIzq = AFMS.getMotor(3);
MotorAtrasDer = AFMS.getMotor(2);

}

void MotoresB::setup(){
    AFMS.begin();
    if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
   delay(1000);
  bno.setExtCrystalUse(true);
}

void MotoresB::moveAdelante(){
    uint8_t i;

    MotorAtrasDer->setSpeed(250);
    MotorAtrasDer->run(FORWARD);
    MotorAtrasIzq->setSpeed(250);
    MotorAtrasIzq->run(FORWARD);
    MotorAdeIzq->setSpeed(250);
    MotorAdeIzq->run(BACKWARD);
    MotorAdeDer->setSpeed(250);
    MotorAdeDer->run(BACKWARD);
}
void MotoresB::moveAdelanteLento(){
    uint8_t i;

    MotorAtrasDer->setSpeed(50);
    MotorAtrasDer->run(FORWARD);
    MotorAtrasIzq->setSpeed(50);
    MotorAtrasIzq->run(FORWARD);
    MotorAdeIzq->setSpeed(50);
    MotorAdeIzq->run(BACKWARD);
    MotorAdeDer->setSpeed(50);
    MotorAdeDer->run(BACKWARD);
}
void MotoresB::acomodoD()
{
  uint8_t i;

    MotorAtrasDer->setSpeed(170);
    MotorAtrasDer->run(FORWARD);
    MotorAtrasIzq->setSpeed(50);
    MotorAtrasIzq->run(FORWARD);
    MotorAdeIzq->setSpeed(50);
    MotorAdeIzq->run(BACKWARD);
    MotorAdeDer->setSpeed(170);
    MotorAdeDer->run(BACKWARD);
}
void MotoresB::acomodoI()
{
  uint8_t i;

  MotorAtrasDer->setSpeed(50);
    MotorAtrasDer->run(FORWARD);
    MotorAtrasIzq->setSpeed(170);
    MotorAtrasIzq->run(FORWARD);
    MotorAdeIzq->setSpeed(170);
    MotorAdeIzq->run(BACKWARD);
    MotorAdeDer->setSpeed(50);
    MotorAdeDer->run(BACKWARD);
}
void MotoresB::detenerse(){
  MotorAdeIzq->run(RELEASE);
  MotorAdeDer->run(RELEASE);
  MotorAtrasIzq->run(RELEASE);
  MotorAtrasDer->run(RELEASE);
}
void MotoresB::moveAtras(){
uint8_t i;

    MotorAtrasDer->setSpeed(250);
    MotorAtrasDer->run(BACKWARD);
    MotorAtrasIzq->setSpeed(250);
    MotorAtrasIzq->run(BACKWARD);
    MotorAdeIzq->setSpeed(250);
    MotorAdeIzq->run(FORWARD);
    MotorAdeDer->setSpeed(250);
    MotorAdeDer->run(FORWARD);
}
void MotoresB::moveAtrasLento(){
uint8_t i;

    MotorAtrasDer->setSpeed(50);
    MotorAtrasDer->run(BACKWARD);
    MotorAtrasIzq->setSpeed(50);
    MotorAtrasIzq->run(BACKWARD);
    MotorAdeIzq->setSpeed(50);
    MotorAdeIzq->run(FORWARD);
    MotorAdeDer->setSpeed(50);
    MotorAdeDer->run(FORWARD);
}
void MotoresB::moveIzq(){
uint8_t i;
double med=0, newMed=0, punto=0, newPunto=0;

    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    Serial.print("X: ");
    med=euler.x();
    Serial.println(med);
    delay(BNO055_SAMPLERATE_DELAY_MS);

if(med<90){

    punto=med-74;
    newPunto=360+punto;
    do{
    MotorAtrasDer->setSpeed(250);
    MotorAtrasDer->run(BACKWARD);
    MotorAtrasIzq->setSpeed(250);
    MotorAtrasIzq->run(FORWARD);
    MotorAdeIzq->setSpeed(250);
    MotorAdeIzq->run(BACKWARD);
    MotorAdeDer->setSpeed(250);
    MotorAdeDer->run(FORWARD);
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    Serial.print("X: ");
    newMed=euler.x();
    Serial.println(newMed);
    delay(BNO055_SAMPLERATE_DELAY_MS);
    }while(newMed<=med || newMed>newPunto);
  }
  else{
    punto=med-74;
    do{
    MotorAtrasDer->setSpeed(250);
    MotorAtrasDer->run(BACKWARD);
    MotorAtrasIzq->setSpeed(250);
    MotorAtrasIzq->run(FORWARD);
    MotorAdeIzq->setSpeed(250);
    MotorAdeIzq->run(BACKWARD);
    MotorAdeDer->setSpeed(250);
    MotorAdeDer->run(FORWARD);
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    Serial.print("X: ");
    newMed=euler.x();
    Serial.println(newMed);
    delay(BNO055_SAMPLERATE_DELAY_MS);
    }while(newMed>punto);
  }
  Serial.println("YA SALIO********");
  MotorAdeIzq->run(RELEASE);
  MotorAdeDer->run(RELEASE);
  MotorAtrasIzq->run(RELEASE);
  MotorAtrasDer->run(RELEASE);
}

void MotoresB::moveDer(){
uint8_t i;
double med=0, newMed=0, punto=0, newPunto=0;

    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    Serial.print("X: ");
    med=euler.x();
    Serial.println(med);
    delay(BNO055_SAMPLERATE_DELAY_MS);

    if(med>=270){

    punto=med+74;
    newPunto=(360-punto)*-1;
    do{
    MotorAtrasDer->setSpeed(250);
    MotorAtrasDer->run(FORWARD);
    MotorAtrasIzq->setSpeed(250);
    MotorAtrasIzq->run(BACKWARD);
    MotorAdeIzq->setSpeed(250);
    MotorAdeIzq->run(FORWARD);
    MotorAdeDer->setSpeed(250);
    MotorAdeDer->run(BACKWARD);
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    Serial.print("X: ");
    newMed=euler.x();
    Serial.println(newMed);
    delay(BNO055_SAMPLERATE_DELAY_MS);
    }while(newMed>=med || newMed<newPunto);
  }
  else{
    punto=med+74;
    do{
    MotorAtrasDer->setSpeed(250);
    MotorAtrasDer->run(FORWARD);
    MotorAtrasIzq->setSpeed(250);
    MotorAtrasIzq->run(BACKWARD);
    MotorAdeIzq->setSpeed(250);
    MotorAdeIzq->run(FORWARD);
    MotorAdeDer->setSpeed(250);
    MotorAdeDer->run(BACKWARD);
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    Serial.print("X: ");
    newMed=euler.x();
    Serial.println(newMed);
    delay(BNO055_SAMPLERATE_DELAY_MS);
    }while(newMed<punto);
  }
  Serial.println("YA SALIO********");
  MotorAdeIzq->run(RELEASE);
  MotorAdeDer->run(RELEASE);
  MotorAtrasIzq->run(RELEASE);
  MotorAtrasDer->run(RELEASE);
}
void MotoresB::moveDerAcomodo(){
   MotorAtrasDer->setSpeed(80);
    MotorAtrasDer->run(FORWARD);
    MotorAtrasIzq->setSpeed(80);
    MotorAtrasIzq->run(BACKWARD);
    MotorAdeIzq->setSpeed(80);
    MotorAdeIzq->run(FORWARD);
    MotorAdeDer->setSpeed(80);
    MotorAdeDer->run(BACKWARD);
}
void MotoresB::moveIzqAcomodo(){
    MotorAtrasDer->setSpeed(80);
    MotorAtrasDer->run(BACKWARD);
    MotorAtrasIzq->setSpeed(80);
    MotorAtrasIzq->run(FORWARD);
    MotorAdeIzq->setSpeed(80);
    MotorAdeIzq->run(BACKWARD);
    MotorAdeDer->setSpeed(80);
    MotorAdeDer->run(FORWARD);
}
