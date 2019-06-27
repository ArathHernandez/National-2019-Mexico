  #include "MotoresPrueba.h"

double Kp=2, Ki=0, Kd=0;
double Input=0, Output=0, Setpoint=0;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, REVERSE);


MotoresPrueba::MotoresPrueba(){
bno = Adafruit_BNO055();
AFMS = Adafruit_MotorShield();
MotorAdeIzq = AFMS.getMotor(4);
MotorAdeDer = AFMS.getMotor(1);
MotorAtrasIzq = AFMS.getMotor(3);
MotorAtrasDer = AFMS.getMotor(2);
}

void MotoresPrueba::setup(){
    AFMS.begin();
    if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
   delay(1000);
  bno.setExtCrystalUse(true);
  Input;
  Setpoint;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void MotoresPrueba::moveAdelante(){
    uint8_t i;

    MotorAtrasDer->setSpeed(210);
    MotorAtrasDer->run(FORWARD);
    MotorAtrasIzq->setSpeed(235);
    MotorAtrasIzq->run(FORWARD);
    MotorAdeIzq->setSpeed(235);
    MotorAdeIzq->run(BACKWARD);
    MotorAdeDer->setSpeed(210);
    MotorAdeDer->run(BACKWARD);


}
void MotoresPrueba::moveAdelanteLento(){
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
void MotoresPrueba::acomodoD()
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
void MotoresPrueba::acomodoI()
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
void MotoresPrueba::detenerse(){
  MotorAdeIzq->run(RELEASE);
  MotorAdeDer->run(RELEASE);
  MotorAtrasIzq->run(RELEASE);
  MotorAtrasDer->run(RELEASE);
}
void MotoresPrueba::moveAtras(){
uint8_t i;

    MotorAtrasDer->setSpeed(210);
    MotorAtrasDer->run(BACKWARD);
    MotorAtrasIzq->setSpeed(235);
    MotorAtrasIzq->run(BACKWARD);
    MotorAdeIzq->setSpeed(235);
    MotorAdeIzq->run(FORWARD);
    MotorAdeDer->setSpeed(210);
    MotorAdeDer->run(FORWARD);
}
void MotoresPrueba::moveAtrasLento(){
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
void MotoresPrueba::moveIzq(){
uint8_t i;
double med=0, newMed=0, punto=0, newPunto=0, nOutput=0;

    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    Serial.print("X: ");
    med=euler.x();
    Serial.println(med);
    delay(BNO055_SAMPLERATE_DELAY_MS);

if(med<90){

    punto=med-85;
    newPunto=360+punto;
    Setpoint=newPunto;
    do{
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    Serial.print("X: ");
    newMed=euler.x();
    Input=newMed;
    Serial.println(newMed);
    delay(BNO055_SAMPLERATE_DELAY_MS);
    myPID.Compute();
    nOutput=Output+150;
    (nOutput>=255)? nOutput=255: nOutput=nOutput;
    Serial.println(nOutput);
    //analogWrite(PIN_OUTPUT, Output);
    MotorAtrasDer->setSpeed(nOutput);
    MotorAtrasDer->run(BACKWARD);
    MotorAtrasIzq->setSpeed(nOutput);
    MotorAtrasIzq->run(FORWARD);
    MotorAdeIzq->setSpeed(nOutput);
    MotorAdeIzq->run(BACKWARD);
    MotorAdeDer->setSpeed(nOutput);
    MotorAdeDer->run(FORWARD);

    }while(newMed<=med || newMed>newPunto);
  }
  else{
    punto=med-85;
    Setpoint=punto;
    do{
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    Serial.print("X: ");
    newMed=euler.x();
    Input=newMed;
    Serial.println(newMed);
    delay(BNO055_SAMPLERATE_DELAY_MS);
    myPID.Compute();
    nOutput=Output+150;
    (nOutput>=255)? nOutput=255: nOutput=nOutput;
    Serial.println(nOutput);
    //analogWrite(PIN_OUTPUT, Output);
    MotorAtrasDer->setSpeed(nOutput);
    MotorAtrasDer->run(BACKWARD);
    MotorAtrasIzq->setSpeed(nOutput);
    MotorAtrasIzq->run(FORWARD);
    MotorAdeIzq->setSpeed(nOutput);
    MotorAdeIzq->run(BACKWARD);
    MotorAdeDer->setSpeed(nOutput);
    MotorAdeDer->run(FORWARD);
    }while(newMed>punto);
  }
  Serial.println("YA SALIO********");
  MotorAdeIzq->run(RELEASE);
  MotorAdeDer->run(RELEASE);
  MotorAtrasIzq->run(RELEASE);
  MotorAtrasDer->run(RELEASE);
}

void MotoresPrueba::moveDer(){
uint8_t i;
double med=0, newMed=0, punto=0, newPunto=0, nOutput=0;

    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    Serial.print("X: ");
    med=euler.x();
    Serial.println(med);
    delay(BNO055_SAMPLERATE_DELAY_MS);

    if(med>=270){

    punto=med+85;
    newPunto=(360-punto)*-1;
    Setpoint=newPunto;
    do{
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    Serial.print("X: ");
    newMed=euler.x();
    Input=newMed;
    Serial.println(newMed);
    delay(BNO055_SAMPLERATE_DELAY_MS);
    myPID.Compute();
    nOutput=Output+150;
    (nOutput>=255)? nOutput=255: nOutput=nOutput;
    Serial.println(nOutput);
    //analogWrite(PIN_OUTPUT, Output);
    MotorAtrasDer->setSpeed(nOutput);
    MotorAtrasDer->run(FORWARD);
    MotorAtrasIzq->setSpeed(nOutput);
    MotorAtrasIzq->run(BACKWARD);
    MotorAdeIzq->setSpeed(nOutput);
    MotorAdeIzq->run(FORWARD);
    MotorAdeDer->setSpeed(nOutput);
    MotorAdeDer->run(BACKWARD);
    }while(newMed>=med || newMed<newPunto);
  }
  else{
    punto=med+85;
    Setpoint=punto;
    do{
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    Serial.print("X: ");
    newMed=euler.x();
    Input=newMed;
    Serial.println(newMed);
    delay(BNO055_SAMPLERATE_DELAY_MS);
    myPID.Compute();
    nOutput=Output+150;
    (nOutput>=255)? nOutput=255: nOutput=nOutput;
    Serial.println(nOutput);
    //analogWrite(PIN_OUTPUT, Output);
    MotorAtrasDer->setSpeed(nOutput);
    MotorAtrasDer->run(FORWARD);
    MotorAtrasIzq->setSpeed(nOutput);
    MotorAtrasIzq->run(BACKWARD);
    MotorAdeIzq->setSpeed(nOutput);
    MotorAdeIzq->run(FORWARD);
    MotorAdeDer->setSpeed(nOutput);
    MotorAdeDer->run(BACKWARD);
    }while(newMed<punto);
  }
  Serial.println("YA SALIO********");
  MotorAdeIzq->run(RELEASE);
  MotorAdeDer->run(RELEASE);
  MotorAtrasIzq->run(RELEASE);
  MotorAtrasDer->run(RELEASE);
}
void MotoresPrueba::moveDerAcomodo(){
   MotorAtrasDer->setSpeed(80);
    MotorAtrasDer->run(FORWARD);
    MotorAtrasIzq->setSpeed(80);
    MotorAtrasIzq->run(BACKWARD);
    MotorAdeIzq->setSpeed(80);
    MotorAdeIzq->run(FORWARD);
    MotorAdeDer->setSpeed(80);
    MotorAdeDer->run(BACKWARD);
}
void MotoresPrueba::moveIzqAcomodo(){
    MotorAtrasDer->setSpeed(80);
    MotorAtrasDer->run(BACKWARD);
    MotorAtrasIzq->setSpeed(80);
    MotorAtrasIzq->run(FORWARD);
    MotorAdeIzq->setSpeed(80);
    MotorAdeIzq->run(BACKWARD);
    MotorAdeDer->setSpeed(80);
    MotorAdeDer->run(FORWARD);
}
