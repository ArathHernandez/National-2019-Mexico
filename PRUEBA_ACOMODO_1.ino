

#include <MotoresPrueba.h>
#include <NewPing.h>
#include <i2cmaster.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

MotoresPrueba robot;
LiquidCrystal_I2C lcd2(0x3F,16,2);

#define RH_ENCODER_A 2
#define RH_ENCODER_B 3
#define LH_ENCODER_A 18
#define LH_ENCODER_B 19

  
 
// variables to store the number of encoder pulses
// for each motor
volatile unsigned long leftCount = 0;
volatile unsigned long rightCount = 0;

 

void setup()
{
  Serial.begin(9600);
  i2c_init();                               // Initialise the i2c bus.
  lcd2.init();
  lcd2.setBacklight(15);
  lcd2.backlight();
  lcd2.clear();
  lcd2.setCursor(0,0);
  PORTC = (1 << PORTC4) | (1 << PORTC5);    // Enable pullups.
  lcd2.display();
  lcd2.print("INICIANDO");
  delay(500);
  robot.setup();
  lcd2.clear();

  pinMode(LH_ENCODER_A, INPUT);
  pinMode(LH_ENCODER_B, INPUT);
  pinMode(RH_ENCODER_A, INPUT);
  pinMode(RH_ENCODER_B, INPUT);
 
attachInterrupt(1,rightEncoderEvent, CHANGE);
   
  
}

void loop()
{
  rightCount=0;
  while(rightCount<1900){
  robot.moveAdelante();
  }
  robot.detenerse();
  delay(2000);
   
}
 

// encoder event for the interrupt call
void rightEncoderEvent() {
  if (digitalRead(RH_ENCODER_A) == HIGH) {
    if (digitalRead(RH_ENCODER_B) == LOW) {
      rightCount++;
    } else {
      rightCount--;
    }
  } else {
    if (digitalRead(RH_ENCODER_B) == LOW) {
      rightCount--;
    } else {
      rightCount++;
    }
  }
}
