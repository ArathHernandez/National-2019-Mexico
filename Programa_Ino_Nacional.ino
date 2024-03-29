//PROGRAMA INO. ROBORREGOS.
//RESCUE MAZE JR.
//CREADO POR ROBORREGOS CHARLIE 2019.
//Version OFICIAL 2.5 FINAL DAY -
//INCLUIR LIBRERIAS CORRECTAS: MotoresPuentes | Rampa

#include <MotoresPuentes.h>
#include <NewPing.h>
#include <Servo.h>
#include <Wire.h>
#include <Rampa.h>
#include <Adafruit_TCS34725.h>
#include <StackArray.h>
#include <QueueArray.h>
#include <i2cmaster.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

#define RH_ENCODER_A 3
#define RH_ENCODER_B 2
#define LH_ENCODER_A 18
#define LH_ENCODER_B 19
#define MAX_DISTANCE 220
#define TRIGGER_PIN_E 33
#define ECHO_PIN_E 34
#define TRIGGER_PIN_A 43
#define ECHO_PIN_A 44
#define TRIGGER_PIN_DE    37
#define ECHO_PIN_DE       38
#define TRIGGER_PIN_DA 35
#define ECHO_PIN_DA 36
#define TRIGGER_PIN_IE   39
#define ECHO_PIN_IE     40
#define TRIGGER_PIN_IA 41
#define ECHO_PIN_IA 42

struct point{
  byte a, b, cost;
};

struct direcciones
{
  byte px, py;
};

MotoresPuentes robot;
Rampa subir;
NewPing sonarE(TRIGGER_PIN_E, ECHO_PIN_E, MAX_DISTANCE); 
NewPing sonarA(TRIGGER_PIN_A, ECHO_PIN_A, MAX_DISTANCE); 
NewPing sonarDE(TRIGGER_PIN_DE, ECHO_PIN_DE, MAX_DISTANCE); 
NewPing sonarDA(TRIGGER_PIN_DA, ECHO_PIN_DA, MAX_DISTANCE); 
NewPing sonarIE(TRIGGER_PIN_IE, ECHO_PIN_IE, MAX_DISTANCE);  
NewPing sonarIA(TRIGGER_PIN_IA, ECHO_PIN_IA, MAX_DISTANCE);  
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);
Servo myservo;
direcciones d[15][15];
LiquidCrystal_I2C lcd2(0x3F,16,2);
SoftwareSerial camara(12, 50);
SoftwareSerial camaraNO(13, 51);

volatile unsigned long leftCount = 0;
volatile unsigned long rightCount = 0;
byte menor = 100;
char moves[100];
byte BFSx;
byte BFSy;
bool pasado = false;
char orientacion = 'N';
char pasados[15][15][2];
bool blackTile[15][15][2];
bool victimas[15][15][2];
bool negro = false;
byte x = 7;
byte y = 7; 
byte z = 0;
int distanciaE;
int distanciaA;
int distanciaEback;
int distanciaAback;
int distanciaDE;
int distanciaDA;
int distanciaIE;
int distanciaIA;
byte contador = 0;
byte ledAzul= 22;
byte ledAmarillo= 23;
int mapa[15][15];
bool pN[15][15][2];
bool pE[15][15][2];
bool pO[15][15][2];
bool pS[15][15][2];
bool visitados[15][15];
byte objX,rampaX, lastX = -1, lastlastX = -1, lastlastlastX = -1, lastlastlastlastX = -1, lastlastlastlastlastX = -1, lastlastlastlastlastlastX = -1;
byte objY, rampaY, lastY = -1, lastlastY = -1, lastlastlastY = -1, lastlastlastlastY = -1, lastlastlastlastlastY = -1, lastlastlastlastlastlastY = -1,  objZ;
byte minisq = 0;
byte contAlg = 0;
int conter;
int device1Address = 0x55<<1;    
int device2Address = 0x2A<<1; 
float celcius1 = 0;  
float celcius2 = 0;  
int auxEncoder;
char inByte;
long objectiveA;
long newInterval=5000;
unsigned long timePassed;
int iterator=0;
int seguidoAdelante = 0;

void clear(){
  lcd2.display();
  lcd2.print("CREANDO MAPA");
  direcciones aux;
  aux.px = -1;
  aux.py = -1;
  for(byte i = 0;  i < 15; i++){
    for(byte j = 0; j < 15; j++){
      mapa[i][j] = -1;
      d[i][j] = aux;
      visitados[i][j] = false;
    }
  }

  lcd2.clear();
  

  for(byte i = 0; i < 100; i++)
    {
      moves[i] = 'z';
    }
  

  for(byte i = 0;  i < 15; i++){
    for(byte j = 0; j < 15; j++){
      if(pasados[i][j][z] == 'V' && blackTile[i][j][z] == false)
        mapa[i][j] = 0;
        //Serial.print("pasados");
        //Serial.print("[");
        //Serial.print(i);
        //Serial.print("][");
        //Serial.print(j);
        //Serial.print("][");
        //Serial.print(z);
        //Serial.print("] = ");
        //Serial.print(pasados[i][j][z]);
        //Serial.print("  Cuadro negro = ");
        //Serial.println(blackTile[i][j][z]);
        
    }}

    //Serial.print("z = ");
    //Serial.println(z);

    mapa[objX][objY] = 99;
    mapa[x][y] = 0;

   /* for(byte i=0; i<15; i++){
    for(byte j=0;j<15;j++){
      if(mapa[i][j] == -1)
        {
          //Serial.print("#");
        }
        /*else if(pE[i][j] == true)
          {
            //Serial.print("/");
          }
          else if(pO[i][j] == true)
          {
            //Serial.print("|");
          }
          else if(pN[i][j] == true)
          {
            //Serial.print("-");
          }
          else if(pS[i][j] == true)
          {
            //Serial.print("~");
          }*******
        else{
      //Serial.print(mapa[i][j]);}
      //Serial.print("      ");
    }
    //Serial.println("");
  }*/
  //Serial.println(" ");
}

void bfs(){
  lcd2.display();
  lcd2.print("CREANDO ALGORITMO");
  QueueArray <point> hijos;
  point aux, aux2;
  aux.a = x;
  aux.b = y;
  aux.cost = 0;
  visitados[x][y] = true;
  hijos.push(aux);

  while(!hijos.isEmpty()){
    aux = hijos.front();
    hijos.pop();
    visitados[aux.a][aux.b] = true;
    //Derecha
    if(!visitados[aux.a][aux.b+1] && mapa[aux.a][aux.b+1] != -1 && mapa[aux.a][aux.b+1] != 99 && aux.a >= 0 && aux.a < 15 && aux.b+1 >= 0 && aux.b+1 < 15 && pE[aux.a][aux.b][z] == false){
      mapa[aux.a][aux.b+1] = aux.cost + 1;
      aux2.cost = aux.cost + 1;

      aux2.a = aux.a;
      aux2.b = aux.b+1;
      d[aux.a][aux.b+1] = {aux.a,aux.b};
      hijos.push(aux2);

    }
    //Izquierda
    if(!visitados[aux.a][aux.b-1] && mapa[aux.a][aux.b-1] != -1 && mapa[aux.a][aux.b-1] != 99 && aux.a >= 0 && aux.a < 15 && aux.b-1 >= 0 && aux.b-1 < 15 && pO[aux.a][aux.b][z] == false){
      mapa[aux.a][aux.b-1] = aux.cost + 1;
      aux2.cost = aux.cost + 1;

      aux2.a = aux.a;
      aux2.b = aux.b-1;
      d[aux.a][aux.b-1] = {aux.a,aux.b};
      hijos.push(aux2);
    }
    //ENFRENTE
    if(!visitados[aux.a+1][aux.b] && mapa[aux.a+1][aux.b] != -1 && mapa[aux.a+1][aux.b] != 99 && aux.a+1 >= 0 && aux.a+1 < 15 && aux.b >= 0 && aux.b < 15 && pS[aux.a][aux.b][z] == false){


      mapa[aux.a+1][aux.b] = aux.cost + 1;
      aux2.cost = aux.cost + 1;
      aux2.a = aux.a+1;
      aux2.b = aux.b;
      d[aux.a+1][aux.b] = {aux.a,aux.b};
      hijos.push(aux2);
    }
    //ATRAS
    if(!visitados[aux.a-1][aux.b] && mapa[aux.a-1][aux.b] != -1 && mapa[aux.a-1][aux.b] != 99 && aux.a-1 >= 0 && aux.a-1 < 15 && aux.b >= 0 && aux.b < 15  && pN[aux.a][aux.b][z] == false){
      if(mapa[aux.a-1][aux.b] == -2){
        mapa[aux.a-1][aux.b] = aux.cost + 10;
        aux2.cost = aux.cost + 10;
      }else{
        mapa[aux.a-1][aux.b] = aux.cost + 1;
        aux2.cost = aux.cost + 1;
      }

      aux2.a = aux.a-1;
      aux2.b = aux.b;
      d[aux.a-1][aux.b] = {aux.a,aux.b};
      hijos.push(aux2);
    }


  }

  /*for(byte i=0; i<15; i++){
    for(byte j=0;j<15;j++){
      if(mapa[i][j] == -1)
        {
          //Serial.print("#");
        }
        /*else if(pE[i][j] == true)
          {
            //Serial.print("/");
          }
          else if(pO[i][j] == true)
          {
            //Serial.print("|");
          }
          else if(pN[i][j] == true)
          {
            //Serial.print("-");
          }
          else if(pS[i][j] == true)
          {
            //Serial.print("~");
          }
        else{
      //Serial.print(mapa[i][j]);}
      //Serial.print("      ");
    }
    //Serial.println("");
  }*/
  //Serial.println(" ");


}

void findp(){
  lcd2.display();
  lcd2.print("BUSCANDO SALIDA");
  byte contador = 0;
  menor = 100;
  BFSx = objX;
  BFSy = objY;
  while(mapa[BFSx][BFSy] != 0){

    if((mapa[BFSx][BFSy+1] < menor) && (mapa[BFSx][BFSy+1] != -1) && (visitados[BFSx][BFSy+1] == true) && BFSx >= 0 && BFSy+1 >= 0 && BFSx < 15 && BFSy+1 < 15 && (pE[BFSx][BFSy][z] == false)){
      menor = mapa[BFSx][BFSy+1];
      moves[contador] = 'i';}

    if((mapa[BFSx+1][BFSy] < menor) && (mapa[BFSx+1][BFSy] != -1) && (visitados[BFSx+1][BFSy] == true) && BFSx+1 >= 0 && BFSy >= 0 && BFSx+1 < 15 && BFSy < 15 && (pS[BFSx][BFSy][z] == false)){
      menor = mapa[BFSx+1][BFSy];
      moves[contador] = 'f';}

    if((mapa[BFSx-1][BFSy] < menor) && (mapa[BFSx-1][BFSy] != -1) && (visitados[BFSx-1][BFSy] == true) && BFSx-1 >= 0 && BFSy >= 0 && BFSx-1 < 15 && BFSy < 15 && (pN[BFSx][BFSy][z] == false)){
      menor = mapa[BFSx-1][BFSy];
      moves[contador] = 'a';}

    if((mapa[BFSx][BFSy-1] < menor) && (mapa[BFSx][BFSy-1] != -1) && (visitados[BFSx][BFSy-1] == true) && BFSx >= 0 && BFSy-1 >= 0 && BFSx < 15 && BFSy-1 < 15 && (pO[BFSx][BFSy][z] == false)){
      menor = mapa[BFSx][BFSy-1];
      moves[contador] = 'd';}

    if(moves[contador] == 'i')
      {
        BFSy++;
      }
      else if(moves[contador] == 'f')
        {
          BFSx++;
        }
        else if(moves[contador] == 'a')
          {
            BFSx--;
          }
          else if(moves[contador] == 'd')
            {
              BFSy--;
            }

     contador++;

      lcd2.clear();
  }
  
    char movesOf[contador];
    
    for(byte i = 0, j = contador-1; i < contador; i++, j--)
      {
        movesOf[i] = moves[j];
      }

    for(byte i = 0; i < contador; i++)
      {
        if(i == contador - 1)
        {
          if(movesOf[i] == 'a')
          {
            switch(orientacion)
              {
                case 'N':
                {
                  derechaAlg();
                  celcius1 = temperatureCelcius(device1Address);                 
                   if(celcius1 > 34)
                   {
                    auxEncoder = rightCount;
                    robot.detenerse();
                    unaVictimaIzquierda();
                    pasado = true;
                    rightCount = auxEncoder;
                   }
                  derechaAlg();
                  orientacion = 'S';
                } break;
                case 'E':
                {
                  derechaAlg();
                  orientacion = 'S';
                } break;
                case 'O':
                {
                  izquierdaAlg();
                  orientacion = 'S';
                } break;
                case 'S':
                {
                  ignore();
                  orientacion = 'S';
                } break;
              }
          }
          else if(movesOf[i] == 'd')
            {
              switch(orientacion)
              {
                case 'N':
                {
                  derechaAlg();
                  orientacion = 'E';
                } break;
                case 'E':
                {
                  ignore();
                  orientacion = 'E';
                } break;
                case 'O':
                {
                  derechaAlg();
                  celcius1 = temperatureCelcius(device1Address);                 
                   if(celcius1 > 34)
                   {
                    auxEncoder = rightCount;
                    robot.detenerse();
                    unaVictimaIzquierda();
                    pasado = true;
                    rightCount = auxEncoder;
                   }
                  derechaAlg();
                  orientacion = 'E';
                } break;
                case 'S':
                {
                  izquierdaAlg();
                  orientacion = 'E';
                } break;
              }
            }
            else if(movesOf[i] == 'i')
              {
                switch(orientacion)
                  {
                    case 'N':
                    {
                      izquierdaAlg();
                      orientacion = 'O';
                    } break;
                    case 'E':
                    {
                      derechaAlg();
                      celcius1 = temperatureCelcius(device1Address);                 
                       if(celcius1 > 34)
                       {
                        auxEncoder = rightCount;
                        robot.detenerse();
                        unaVictimaIzquierda();
                        pasado = true;
                        rightCount = auxEncoder;
                       }
                      derechaAlg();
                      orientacion = 'O';
                    } break;
                    case 'O':
                    {
                      ignore();
                      orientacion = 'O';
                    } break;
                    case 'S':
                    {
                      derechaAlg();
                      orientacion = 'O';
                    } break;
                  }
              }
              else if(movesOf[i] == 'f')
                {
                  switch(orientacion)
                    {
                      case 'N':
                      {
                        ignore();
                        orientacion = 'N';
                      } break;
                      case 'E':
                      {
                        izquierdaAlg();
                        orientacion = 'N';
                      } break;
                      case 'O':
                      {
                        derechaAlg();
                        orientacion = 'N';
                      } break;
                      case 'S':
                      {
                        derechaAlg();
                        celcius1 = temperatureCelcius(device1Address);                 
                         if(celcius1 > 34)
                         {
                          auxEncoder = rightCount;
                          robot.detenerse();
                          unaVictimaIzquierda();
                          pasado = true;
                          rightCount = auxEncoder;
                         }
                        derechaAlg();
                        orientacion = 'N';
                      } break;
                    }
                }
        }
        else
        {
          if(movesOf[i] == 'a')
            {
              switch(orientacion)
                {
                  case 'N':
                  {
                    derechaAlg();
                    celcius1 = temperatureCelcius(device1Address);                 
                   if(celcius1 > 34)
                   {
                    auxEncoder = rightCount;
                    robot.detenerse();
                    unaVictimaIzquierda();
                    pasado = true;
                    rightCount = auxEncoder;
                   }
                    derechaAlg();
                    adelanteAlg();
                    orientacion = 'S';
                  } break;
                  case 'E':
                  {
                    derechaAlg();
                    adelanteAlg();
                    orientacion = 'S';
                  } break;
                  case 'O':
                  {
                    izquierdaAlg();
                    adelanteAlg();
                    orientacion = 'S';
                  } break;
                  case 'S':
                  {
                    adelanteAlg();
                    orientacion = 'S';
                  } break;
                }
            }
            else if(movesOf[i] == 'd')
              {
                switch(orientacion)
                {
                  case 'N':
                  {
                    derechaAlg();
                    adelanteAlg();
                    orientacion = 'E';
                  } break;
                  case 'E':
                  {
                    adelanteAlg();
                    orientacion = 'E';
                  } break;
                  case 'O':
                  {
                    derechaAlg();
                    celcius1 = temperatureCelcius(device1Address);                 
                   if(celcius1 > 34)
                   {
                    auxEncoder = rightCount;
                    robot.detenerse();
                    unaVictimaIzquierda();
                    pasado = true;
                    rightCount = auxEncoder;
                   }
                    derechaAlg();
                    adelanteAlg();
                    orientacion = 'E';
                  } break;
                  case 'S':
                  {
                    izquierdaAlg();
                    adelanteAlg();
                    orientacion = 'E';
                  } break;
                }
              }
              else if(movesOf[i] == 'i')
                {
                  switch(orientacion)
                    {
                      case 'N':
                      {
                        izquierdaAlg();
                        adelanteAlg();
                        orientacion = 'O';
                      } break;
                      case 'E':
                      {
                        derechaAlg();
                        celcius1 = temperatureCelcius(device1Address);                 
                         if(celcius1 > 34)
                         {
                          auxEncoder = rightCount;
                          robot.detenerse();
                          unaVictimaIzquierda();
                          pasado = true;
                          rightCount = auxEncoder;
                         }
                        derechaAlg();
                        adelanteAlg();
                        orientacion = 'O';
                      } break;
                      case 'O':
                      {
                        adelanteAlg();
                        orientacion = 'O';
                      } break;
                      case 'S':
                      {
                        derechaAlg();
                        adelanteAlg();
                        orientacion = 'O';
                      } break;
                    }
                }
                else if(movesOf[i] == 'f')
                  {
                    switch(orientacion)
                      {
                        case 'N':
                        {
                          adelanteAlg();
                          orientacion = 'N';
                        } break;
                        case 'E':
                        {
                          izquierdaAlg();
                          adelanteAlg();
                          orientacion = 'N';
                        } break;
                        case 'O':
                        {
                          derechaAlg();
                          adelanteAlg();
                          orientacion = 'N';
                        } break;
                        case 'S':
                        {                           
                          derechaAlg();
                          celcius1 = temperatureCelcius(device1Address);                 
                           if(celcius1 > 34)
                           {
                            auxEncoder = rightCount;
                            robot.detenerse();
                            unaVictimaIzquierda();
                            pasado = true;
                            rightCount = auxEncoder;
                           }
                          derechaAlg();
                          adelanteAlg();
                          orientacion = 'N';
                        } break;
                      }
                  }
      }
      robot.actualizaSetpoint();
}}

void derechaAlg()
{
  contAlg++;  
  robot.moveDer();
  robot.detenerse();
  //delay(10);

  distanciaA = distanciaAtras();

  if(distanciaA < 20)
  {
    contAlg = 0;
    robot.moveAtras();
    delay(775);
    robot.actualizaSetpoint();
    robot.moveAdelante();
    delay(200);
    robot.detenerse();
    //delay(10);
  }
  robot.actualizaSetpoint();
}

void izquierdaAlg()
{
  contAlg++;
  robot.moveIzq();
  robot.detenerse();
  //delay(10);

  distanciaA = distanciaAtras();

  if(distanciaA < 20)
  {
    contAlg = 0;
    robot.moveAtras();
    delay(775);
    robot.actualizaSetpoint();
    robot.moveAdelante();
    delay(200);
    robot.detenerse();
    //delay(10);
  }
  robot.actualizaSetpoint();
}

void adelanteAlg()
{
  rightCount = 0;

  robot.actualizaSetpoint();
  while(rightCount < 2100)
  { 
    if(digitalRead(30) == LOW && rightCount < 1400)
   {
    auxEncoder = rightCount;
    robot.detenerse();
    //delay(10);
    robot.acomodarseChoqueIzquierda1();
    robot.detenerse();
    //delay(30);
    robot.acomodarseChoqueIzquierda2();
    robot.detenerse();
    //delay(50);
    robot.actualizaSetpoint();
    //delay(50);
    robot.moveAdelante();
    delay(280);
    robot.detenerse();
    //delay(10);
    rightCount = auxEncoder;
   }

   if(digitalRead(29) == LOW && rightCount < 1400)
   {
    auxEncoder = rightCount;
    robot.detenerse();
    //delay(10);
    robot.acomodarseChoqueDerecha1();
    robot.detenerse();
    //delay(30);
    robot.acomodarseChoqueDerecha2();
    robot.detenerse();
    //delay(50);
    robot.actualizaSetpoint();
    //delay(50);
    robot.moveAdelante();
    delay(280);
    robot.detenerse();
    //delay(10);
    rightCount = auxEncoder;
   }
   robot.moveAdelante();
  }

  robot.detenerse();
  //delay(10);
}

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

int distanciaEnfrente()
{
  int uSDE = sonarE.ping_median();
  int distancia;
  byte turns = 0;
  distancia = uSDE / US_ROUNDTRIP_CM;
  
  if(distancia == 0)
  {
    distancia = uSDE / US_ROUNDTRIP_CM;
  }

  if(distancia == 0)
  {
    distancia = uSDE / US_ROUNDTRIP_CM;
  }
  //Serial.print("Distancia Enfrente= ");
  //Serial.println(distancia);
  return distancia;
}

int distanciaAtras()
{
  int uSDE = sonarA.ping_median();
  int distancia;
  distancia = uSDE / US_ROUNDTRIP_CM;

  if(distancia == 0)
  {
    distancia = uSDE / US_ROUNDTRIP_CM;
  }

  if(distancia == 0)
  {
    distancia = uSDE / US_ROUNDTRIP_CM;
  }
  //Serial.print("Distancia Atras= ");
  //Serial.println(distancia);
  return distancia;
}

int distanciaDerechaEnfrente()
{
  int uSDE = sonarDE.ping_median();
  int distancia;
  distancia = uSDE / US_ROUNDTRIP_CM;

  if(distancia == 0)
  {
    distancia = uSDE / US_ROUNDTRIP_CM;
  }

  if(distancia == 0)
  {
    distancia = uSDE / US_ROUNDTRIP_CM;
  }
  //Serial.print("Distancia Derecha Enfrente= ");
  //Serial.println(distancia);
  
  return distancia;
}

int distanciaDerechaAtras()
{
  int uSDE = sonarDA.ping_median();
  int distancia;
  byte turns = 0;
  distancia = uSDE / US_ROUNDTRIP_CM;
  
  if(distancia == 0)
  {
    distancia = uSDE / US_ROUNDTRIP_CM;
  }

  if(distancia == 0)
  {
    distancia = uSDE / US_ROUNDTRIP_CM;
  }


  //Serial.print("Distancia Derecha Atras= ");
  //Serial.println(distancia);
  return distancia;
}

int distanciaIzquierdaEnfrente()
{
  int uSDE = sonarIE.ping_median();
  int distancia;
  byte turns = 0;
  distancia = uSDE / US_ROUNDTRIP_CM;
  
 if(distancia == 0)
  {
    distancia = uSDE / US_ROUNDTRIP_CM;
  }

  if(distancia == 0)
  {
    distancia = uSDE / US_ROUNDTRIP_CM;
  }

  //Serial.print("Distancia Izquierda Enfrente= ");
  //Serial.println(distancia);
  return distancia;
}

int distanciaIzquierdaAtras()
{
  int uSDE = sonarIA.ping_median();
  int distancia;
  byte turns = 0;
  distancia = uSDE / US_ROUNDTRIP_CM;
  
  if(distancia == 0)
  {
    distancia = uSDE / US_ROUNDTRIP_CM;
  }

  if(distancia == 0)
  {
    distancia = uSDE / US_ROUNDTRIP_CM;
  }

  //Serial.print("Distancia Izquierda Atras= ");
  //Serial.println(distancia);
  //Serial.println("*****");
  return distancia;
}

void ignore()
{
  
}

bool isBlack()
{
  int r, g, b, c, colorTemp, lux;
  
  tcs.getRawData(&r, &g, &b, &c);
  tcs.getRawData(&r, &g, &b, &c);
  if(r < 800 && g < 800 && b < 800){
    lcd2.display();
    lcd2.print("CUADRO NEGRO DETECTADO");
    lcd2.display();
  blackTile[x][y][z] = true;
  return true;
  }
  else{
  return false;}
}

void unaVictimaIzquierda()
{
  lcd2.clear();
  lcd2.display();
  lcd2.print("VICTIMA IZQ");

 robot.moveIzq();
 robot.detenerse();
  
 for(int i = 0;  i < 12; i ++)
 {
  digitalWrite(23, HIGH);
  delay(300);
  digitalWrite(23, LOW);
  delay(300);
 }

  myservo.write(180);
  delay(1000);
  myservo.write(40);
  delay(1000);
  myservo.write(120);
  delay(1000);
  myservo.write(79);
  delay(500);
    
  lcd2.clear();

  robot.moveDer();
  robot.detenerse();
  delay(50);

  return;
}

void unaVictimaDerecha()
{
  lcd2.clear();
  lcd2.display();
  lcd2.print("VICTIMA DERECHA");

  robot.moveDer();
  
  for(int i = 0;  i < 12; i ++)
 {
  digitalWrite(23, HIGH);
  delay(300);
  digitalWrite(23, LOW);
  delay(300);
 }

  myservo.write(0);
  delay(1000);
  myservo.write(120);
  delay(1000);
  myservo.write(40);
  delay(1000);
  myservo.write(79);
  delay(500);
    
  lcd2.clear();

  robot.moveIzq();

  //delay(50);

  return;
}

void dosVictimasIzquierda()
{
  lcd2.clear();
  lcd2.display();
  lcd2.print("VICTIMA IZQ H");

  robot.moveIzq();
 robot.detenerse();
  
  for(int i = 0; i < 15; i ++)
 {
  digitalWrite(23, HIGH);
  delay(300);
  digitalWrite(23, LOW);
  delay(300);
 }

  myservo.write(180);
  delay(1000);
  myservo.write(40);
  delay(1000);
  myservo.write(120);
  delay(1000);
  myservo.write(79);
  delay(500);
  delay(1000);
  myservo.write(180);
  delay(1000);
  myservo.write(40);
  delay(1000);
  myservo.write(120);
  delay(1000);
  myservo.write(79);
  delay(500);

  robot.moveDer();
  robot.detenerse();
  //delay(50);
    
  lcd2.clear();

  return;
}

void dosVictimasDerecha()
{
  lcd2.clear();
  lcd2.display();
  lcd2.print("VICTIMAS DERECHA");

  robot.moveDer();
 robot.detenerse();
  
  for(int i = 0;  i < 12; i ++)
 {
  digitalWrite(23, HIGH);
  delay(300);
  digitalWrite(23, LOW);
  delay(300);
 }

  myservo.write(0);
  delay(1000);
  myservo.write(120);
  delay(1000);
  myservo.write(40);
  delay(1000);
  myservo.write(79);
  delay(500);
  delay(1000);
  myservo.write(0);
  delay(1000);
  myservo.write(120);
  delay(1000);
  myservo.write(40);
  delay(1000);
  myservo.write(79);
  delay(500);

  robot.moveIzq();
  robot.detenerse();
  //delay(50);
    
  lcd2.clear();

  return;
}

void buscarObjetivo()
{
  clear();
  bfs();
  lcd2.clear();
  findp();

  switch(orientacion)
  {
    case 'N':{
      x = objX + 1;
      y = objY;
    } break;
    case 'E':
    {
      x = objX;
      y = objY - 1;
    } break;
    case 'O':
    {
      x = objX;
      y = objY + 1;
    } break;
    case 'S':
    {
      x = objX - 1;
      y = objY;
    }
  }
}

void alineaRobot(){
  /*//lcd2.display();
  //lcd2.print("ALINEANDO ROBOT");
    distanciaE=distanciaEnfrente();
    distanciaA=distanciaAtras();
    distanciaDE=distanciaDerechaEnfrente();
    distanciaDA=distanciaDerechaAtras();
    distanciaIE=distanciaIzquierdaEnfrente();
    distanciaIA=distanciaIzquierdaAtras();
    if((distanciaDE!=distanciaDA||distanciaIE!=distanciaIA)&&((distanciaDE<20 && distanciaDA<20)||(distanciaIE<20 && distanciaIA<20))){
  //Serial.println("Desalineado******************");
  if(distanciaDE>distanciaDA && distanciaIE<distanciaIA){
    while(distanciaDE>distanciaDA && distanciaIE<distanciaIA){
       robot.moveDerAcomodo();
       digitalWrite(ledAzul, HIGH); 
       distanciaDE=distanciaDerechaEnfrente();
       distanciaDA=distanciaDerechaAtras();
       delay(50);
    }  
    robot.detenerse();
    digitalWrite(ledAzul, LOW);    
  }
   
  if(distanciaDE<distanciaDA && distanciaIE>distanciaIA){
    while(distanciaDE<distanciaDA && distanciaIE>distanciaIA){
     robot.moveIzqAcomodo();  
   digitalWrite(ledAmarillo, HIGH);
   distanciaIE=distanciaIzquierdaEnfrente();
   distanciaIA=distanciaIzquierdaAtras();
   delay(50);
    } 
   robot.detenerse();
   digitalWrite(ledAmarillo, LOW);  
}
}
delay(500);

//lcd2.clear();*/
}
float temperatureCelcius(int address) {
  int dev = address;
  int data_low = 0;
  int data_high = 0;
  int pec = 0;

  // Write
  i2c_start_wait(dev+I2C_WRITE);
  i2c_write(0x07);

  // Read
  i2c_rep_start(dev+I2C_READ);
  data_low = i2c_readAck();       // Read 1 byte and then send ack.
  data_high = i2c_readAck();      // Read 1 byte and then send ack.
  pec = i2c_readNak();
  i2c_stop();

  // This converts high and low bytes together and processes temperature, 
  // MSB is a error bit and is ignored for temps.
  double tempFactor = 0.02;       // 0.02 degrees per LSB (measurement 
                                  // resolution of the MLX90614).
  double tempData = 0x0000;       // Zero out the data
  int frac;                       // Data past the decimal point

  // This masks off the error bit of the high byte, then moves it left 
  // 8 bits and adds the low byte.
  tempData = (double)(((data_high & 0x007F) << 8) + data_low);
  tempData = (tempData * tempFactor)-0.01;
  float celcius = tempData - 273.15;
  
  // Returns temperature un Celcius.
  return celcius;
}

void setup() {
  Serial.begin(9600);
  Serial.begin(115200);
  camara.begin(115200);
  i2c_init();                               // Initialise the i2c bus.
  lcd2.init();
  lcd2.setBacklight(10);
  lcd2.backlight();
  lcd2.clear();
  lcd2.setCursor(0,0);
  PORTC = (1 << PORTC4) | (1 << PORTC5);    // Enable pullups.
  lcd2.display();
  lcd2.print("INICIANDO");
  lcd2.clear();
  
  myservo.attach(32);
  myservo.write(81);
  
  pinMode(29, INPUT_PULLUP);
  pinMode(30, INPUT_PULLUP);
  pinMode(23, OUTPUT);
  pinMode(LH_ENCODER_A, INPUT);
  pinMode(LH_ENCODER_B, INPUT);
  pinMode(RH_ENCODER_A, INPUT);
  pinMode(RH_ENCODER_B, INPUT);
 
  attachInterrupt(1,rightEncoderEvent, CHANGE);

  lcd2.display();
  lcd2.print("INICIANDO MAPA");
  for(byte i = 0; i < 15; i++)
    for(byte j = 0; j < 15; j++)
      for(byte k = 0; k < 2; k++)
        {
          pasados[i][j][k] = 'I';
          victimas[i][j][k] = false;
          blackTile[i][j][k] = false;
          pN[i][j][k] = false;
          pE[i][j][k] = false;
          pO[i][j][k] = false;
          pS[i][j][k] = false;
        }

  delay(10);
  lcd2.clear();

  lcd2.display();
  lcd2.print("INICIANDO MOTORES");
  robot.setup();
  delay(10);
  lcd2.clear();

  lcd2.display();
  lcd2.print("INICIANDO RAMPA");
  subir.setup();
  delay(10);
  lcd2.clear();

  lcd2.display();
  lcd2.print("DISTANCIAS INICIALES");
  
  pasados[x][y][z] = 'V';

   distanciaDE = distanciaDerechaEnfrente();
   distanciaIE = distanciaIzquierdaEnfrente();
   distanciaA = distanciaAtras();

  if(distanciaDE < 20)
    {
      pE[x][y][z] = true;
      pO[x][y+1][z] = true;
    }
    else
      {
        pasados[x][y+1][z] = 'P';
      }

  if(distanciaIE < 20)
    {
      pO[x][y][z] = true;
      pE[x][y-1][z] = true;
    }
    else
    {
      pasados[x][y-1][z] = 'P';
    }

  if(distanciaA < 20)
    {
      pS[x][y][z] = true;
      pN[x+1][y][z] = true;
    }
    else
      {
        pasados[x+1][y][z] = 'P';
      }

  if(distanciaDE > 15 || distanciaIE > 15 || distanciaA >15)
    {
      lastX = 7;
      lastY = 7;
    }

  delay(10);

  lcd2.clear();

  delay(1000);
  robot.actualizaSetpoint();
}

void loop() {
  

byte pos;
    byte valor = 0;
    rightCount=0;
    iterator=0;
    objectiveA=millis()+newInterval;
    lcd2.display();
    lcd2.print("ADELANTE");
    while(rightCount<2100){
    if(iterator==0){
    timePassed=millis();
    }
    else{
      timePassed=0;
    }
    if(timePassed<objectiveA){
       robot.moveAdelante();
      valor = subir.detectaRampa();
    if(valor != 0)
    {
      lcd2.clear();
      lcd2.display();
      lcd2.print("RAMPA DETECTADA");
      //delay(250);
      lcd2.clear();
      if(z == 1){
      z = 0;}
      else{
      z = 1;}
      lastlastlastlastlastlastX = -1;
      lastlastlastlastlastlastY = -1;
      lastlastlastlastlastX = -1;
      lastlastlastlastlastY = -1;
      lastlastlastlastX = -1;
      lastlastlastlastY = -1;
      lastlastlastX = -1;
      lastlastlastY = -1;
      lastlastX = -1;
      lastlastY = -1;
      lastX = -1;
      lastY = -1;
    }
      
   if(digitalRead(30) == LOW && rightCount < 1400)
   {
    auxEncoder = rightCount;
    robot.detenerse();
    //delay(10);
    robot.acomodarseChoqueIzquierda1();
    robot.detenerse();
    //delay(30);
    robot.acomodarseChoqueIzquierda2();
    robot.detenerse();
    //delay(50);
    //delay(50);
    robot.moveAdelante();
    delay(280);
    robot.detenerse();
    //delay(10);
    rightCount = auxEncoder;
   }

   if(digitalRead(29) == LOW && rightCount < 1400)
   {
    auxEncoder = rightCount;
    robot.detenerse();
    //delay(10);
    robot.acomodarseChoqueDerecha1();
    robot.detenerse();
    //delay(30);
    robot.acomodarseChoqueDerecha2();
    robot.detenerse();
    //delay(50);
    //delay(50);
    robot.moveAdelante();
    delay(280);
    robot.detenerse();
    //delay(10);
    rightCount = auxEncoder;
   }
   if(rightCount < 1750){

   if(pasado == false){

   celcius1 = temperatureCelcius(device1Address); 
   celcius2 = temperatureCelcius(device2Address); 

   if(celcius2 > 34)
   {
    auxEncoder = rightCount;
    robot.detenerse();
     unaVictimaDerecha();
     pasado = true;
     rightCount = auxEncoder;
     iterator++;
   }

   if(celcius1 > 34)
   {
    auxEncoder = rightCount;
    robot.detenerse();
    unaVictimaIzquierda();
    pasado = true;
    rightCount = auxEncoder;
    iterator++;
   }
   
  while(camara.available() > 0){
   inByte = camara.read();
    Serial.write(inByte);
    if(inByte == '6'){ // 3 ES PARA LAS VICTIMAS H DEL LADO IZQUIERDO
      //();
      auxEncoder = rightCount;
      Serial.println();
      robot.detenerse();
      //lcd2.display();
      //lcd2.print("VICTIMA H IZQ");
      //lcd2.clear();
      robot.moveAtras();
      delay(100);
      robot.detenerse();
      dosVictimasIzquierda();
      //lcd2.clear();
      pasado = true;
      rightCount = auxEncoder;
      iterator++;
      break;
    }
    else if(inByte == '5'){ // 2 ES PARA LAS VICTIMAS S DEL LADO IZQUIERDO
      auxEncoder = rightCount;
      Serial.println();
      robot.detenerse();
      //lcd2.display();
      //lcd2.print("VICTIMA S IZQ");
      //lcd2.clear();
      robot.moveAtras();
      delay(100);
      unaVictimaIzquierda();
      //lcd2.clear();
      pasado = true;
      rightCount = auxEncoder;
      iterator++;
      break;
    }
  /* else  if(inByte == '4' ){ // 1 ES PARA LAS VICTIMAS U DEL LADO IZQUIERDO
    auxEncoder = rightCount;
      Serial.println();
      robot.detenerse();
      //lcd2.display();
      //lcd2.print("VICTIMA U IZQ");
      for(int i = 0;  i < 12; i++)
      {
        digitalWrite(23, HIGH);
        delay(300);
        digitalWrite(23, LOW);
        delay(300);  
      }
      //lcd2.clear();
      pasado = true;
      rightCount = auxEncoder;
      iterator++;
      break;
    }*/
   else if(inByte == '3'){ // 6 ES PARA LAS VICTIMAS H DEL LADO DERECHO
      auxEncoder = rightCount;
      Serial.println();
      robot.detenerse();
      //lcd2.display();
      //lcd2.print("VICTIMA H DER");
      //lcd2.clear();
      robot.moveAtras();
      delay(100);
      dosVictimasDerecha();
      //lcd2.clear();
      pasado = true;
      rightCount = auxEncoder;
      iterator++;
      break;
    }
   else if(inByte == '2'){ // 5 ES PARA LAS VICTIMAS S DE LADO DERECHO
      auxEncoder = rightCount;
      Serial.println();
      robot.detenerse();
      //lcd2.display();
      //lcd2.print("VICTIMA S DER");
      //lcd2.clear();
      robot.moveAtras();
      delay(100);
      unaVictimaDerecha();
      //lcd2.clear();
      pasado = true;
      rightCount = auxEncoder;
      iterator++;
      break;
    }
   /* else if(inByte == '1'){ // 4 ES PARA LAS VICTIMAS U DEL LADO DERECHO
      auxEncoder = rightCount;
      Serial.println();
      robot.detenerse();
      //lcd2.display();
      //lcd2.print("VICTIMA U DER");

      for(int i = 0;  i < 12; i++)
      {
        digitalWrite(23, HIGH);
        delay(300);
        digitalWrite(23, LOW);
        delay(300);  
      }
      
      //lcd2.clear();
      pasado = true;
      rightCount = auxEncoder;
      iterator++;
      break;
    }
    else
    {
      break;}
    }*/}
    }   
    }
    }
    else{
    lcd2.clear();
    lcd2.print("CONDICION X");
    robot.moveAtras();
    delay(300);
    robot.moveDerechaFast();
    robot.moveAdelanteFast(); 
    robot.detenerse();
    break;
    }
    }
    

  // blank line to separate data from the two ports:
  //Serial.println();

  

  lcd2.clear();

   pasado = false;
   inByte = '0';
  
  robot.detenerse();
  //delay(10);
  alineaRobot();

  if(pasado == false){

   celcius1 = temperatureCelcius(device1Address); 
   celcius2 = temperatureCelcius(device2Address); 

   if(celcius2 > 28)
   {
    distanciaDE = distanciaDerechaEnfrente();
    if(distanciaDE < 20){
    auxEncoder = rightCount;
    robot.detenerse();
     unaVictimaDerecha();
     pasado = true;
     rightCount = auxEncoder;}
   }

   if(celcius1 > 28)
   {
    distanciaIE = distanciaIzquierdaEnfrente();
    if(distanciaIE < 20){
    auxEncoder = rightCount;
    robot.detenerse();
    unaVictimaIzquierda();
    pasado = true;
    rightCount = auxEncoder;}
   }}
  
  distanciaE = distanciaEnfrente();

  if(distanciaE > 10 && distanciaE < 20)
   {
    while(distanciaE > 10){
      distanciaE = distanciaEnfrente();
      robot.moveAdelante();}
   }

   robot.detenerse();

  switch(orientacion)
    {
      case 'N':
        x--;
        break;
      case 'E':
        y++;
        break;
      case 'O':
        y--;
        break;
       case 'S':
       x++;
       break;  
    }

    if(valor == 1 && z == 1)
    {
      rampaX = x;
      rampaY = y;
    }    
    
   pasados[x][y][z] = 'V';

   if(valor == 1)
   {
    if(z == 1)
    {
      pasados[x][y][0] = 'I';
    }
    else if(z == 0)
    {
      pasados[x][y][1] = 'I';
    }
   }

     negro = isBlack();

  minisq = 0;

  if(x == 7 && y == 7 && z == 0){
     for(byte i = 0; i < 15; i++)
      {
        for(byte j = 0; j < 15; j++)
          {
            for(byte k = 0; k < 2; k++)
              {
                if(pasados[i][j][k] == 'P')
                  {
                    minisq++;
                  }
              }
          }
      }

      if(minisq ==0)
        {
          int j = 0;
          lcd2.display();
          lcd2.print("FIN DE RONDA");
          for(int i = 1; i > 0; i++){
           j++;
           }        
        }}

  if(negro == false)
  {
   distanciaDE = distanciaDerechaEnfrente();
   distanciaIE = distanciaIzquierdaEnfrente();
   distanciaE = distanciaEnfrente();

  if(distanciaDE < 20 && distanciaDE != 0)
    {
      switch(orientacion)
        {
          case 'N':{
            pE[x][y][z] = true;
            pO[x][y+1][z] = true;
          }break;
          case 'E':{
            pS[x][y][z] = true;
            pN[x+1][y][z] = true;
          }break;
          case 'O':{
            pN[x][y][z] = true;
            pS[x-1][y][z] = true;
          }break;
          case 'S':{
            pO[x][y][z] = true;
            pE[x][y-1][z] = true;
        } break;
        }
    }

  if(distanciaIE < 20 && distanciaIE != 0)
    {
      switch(orientacion)
        {
          case 'N':{
            pO[x][y][z] = true;
            pE[x][y-1][z] = true;
          }break;
          case 'E':{
            pN[x][y][z] = true;
            pS[x-1][y][z] = true;
          }break;
          case 'O':{
            pS[x][y][z] = true;
            pN[x+1][y][z] = true;
          } break;
          case 'S':{
            pE[x][y][z] = true;
            pO[x][y+1][z] = true;
          }break;
        }
    }

  if(distanciaE < 20 && distanciaE != 0)
    {
      switch(orientacion)
        {
          case 'N':{
            pN[x][y][z] = true;
            pS[x-1][y][z] = true;
          }break;
          case 'E':{
            pE[x][y][z] = true;
            pO[x][y+1][z] = true;
          } break;
          case 'O':{
            pO[x][y][z] = true;
            pE[x][y-1][z] = true;
          }break;
          case 'S':{
            pS[x][y][z] = true;
            pN[x+1][y][z] = true;
          }break;
        }
    }

    switch(orientacion)
    {
      case 'N':
        {
          if(pasados[x][y+1][z] == 'I' && pE[x][y][z] == false)
            {
              pasados[x][y+1][z] = 'P';
            }

          if(pasados[x-1][y][z] == 'I' && pN[x][y][z] == false)
            {
              pasados[x-1][y][z] = 'P';
            }

            if(pasados[x][y-1][z] == 'I' && pO[x][y][z] == false)
              {
                pasados[x][y-1][z] = 'P';
              }
        } break;
      case 'E':
        {
          if(pasados[x+1][y][z] == 'I' && pS[x][y][z] == false)
            {
              pasados[x+1][y][z] = 'P';
            }

          if(pasados[x][y+1][z] == 'I' && pE[x][y][z] == false)
            {
              pasados[x][y+1][z] = 'P';
            }

            if(pasados[x-1][y][z] == 'I' && pN[x][y][z] == false)
              {
                pasados[x-1][y][z] = 'P';
              }
        } break;
      case 'O':
        {
          if(pasados[x-1][y][z] == 'I' && pN[x][y][z] == false)
            {
              pasados[x-1][y][z] = 'P';
            }

          if(pasados[x][y-1][z] == 'I' && pO[x][y][z] == false)
            {
              pasados[x][y-1][z] = 'P';
            }

            if(pasados[x+1][y][z] == 'I' && pS[x][y][z] == false)
              {
                pasados[x+1][y][z] = 'P';
              }
        } break;
      case 'S':
        {
          if(pasados[x][y-1][z] == 'I' && pO[x][y][z] == false)
            {
              pasados[x][y-1][z] = 'P';
            }

          if(pasados[x+1][y][z] == 'I' && pS[x][y][z] == false)
            {
              pasados[x+1][y][z] = 'P';
            }

            if(pasados[x][y+1][z] == 'I' && pE[x][y][z] == false)
              {
                pasados[x][y+1][z] = 'P';
              }
        } break;
    }

    switch(orientacion)
    {
      case 'N':
        {
          if(pasados[x][y+1][z] == 'P' && pasados[x-1][y][z] == 'P' && distanciaDE > 15 && distanciaE > 15)
            {
              lastlastlastlastlastlastX = lastlastlastlastlastX;
              lastlastlastlastlastX = lastlastlastlastX;
              lastlastlastlastX = lastlastlastX;
              lastlastlastX = lastlastX;
              lastlastX = lastX;
              lastX = x;

              lastlastlastlastlastlastY = lastlastlastlastlastY;
              lastlastlastlastlastY = lastlastlastlastY;
              lastlastlastlastY = lastlastlastY;
              lastlastlastY = lastlastY;
              lastlastY = lastY;
              lastY = y;
            }
            else if(pasados[x][y+1][z] == 'P' && pasados[x][y-1][z] == 'P' && distanciaDE > 15 && distanciaIE > 15)
            {
              lastlastlastlastlastlastX = lastlastlastlastlastX;
              lastlastlastlastlastX = lastlastlastlastX;
              lastlastlastlastX = lastlastlastX;
              lastlastlastX = lastlastX;
              lastlastX = lastX;
              lastX = x;

              lastlastlastlastlastlastY = lastlastlastlastlastY;
              lastlastlastlastlastY = lastlastlastlastY;
              lastlastlastlastY = lastlastlastY;
              lastlastlastY = lastlastY;
              lastlastY = lastY;
              lastY = y;
            }
            else if(pasados[x-1][y][z] == 'P' && pasados[x][y-1][z] == 'P' && distanciaE > 15 && distanciaIE > 15)
            {
              lastlastlastlastlastlastX = lastlastlastlastlastX;
              lastlastlastlastlastX = lastlastlastlastX;
              lastlastlastlastX = lastlastlastX;
              lastlastlastX = lastlastX;
              lastlastX = lastX;
              lastX = x;

              lastlastlastlastlastlastY = lastlastlastlastlastY;
              lastlastlastlastlastY = lastlastlastlastY;
              lastlastlastlastY = lastlastlastY;
              lastlastlastY = lastlastY;
              lastlastY = lastY;
              lastY = y;
            }
        } break;
        case 'E':
          {
            if(pasados[x+1][y][z] == 'P' && pasados[x][y+1][z] == 'P' && distanciaDE > 15 && distanciaE > 15)
              {
                lastlastlastlastlastlastX = lastlastlastlastlastX;
              lastlastlastlastlastX = lastlastlastlastX;
              lastlastlastlastX = lastlastlastX;
              lastlastlastX = lastlastX;
              lastlastX = lastX;
              lastX = x;

              lastlastlastlastlastlastY = lastlastlastlastlastY;
              lastlastlastlastlastY = lastlastlastlastY;
              lastlastlastlastY = lastlastlastY;
              lastlastlastY = lastlastY;
              lastlastY = lastY;
              lastY = y;
              }
              else if(pasados[x+1][y][z] == 'P' && pasados[x-1][y][z] == 'P' && distanciaDE > 15 && distanciaIE > 15)
              {
                lastlastlastlastlastlastX = lastlastlastlastlastX;
              lastlastlastlastlastX = lastlastlastlastX;
              lastlastlastlastX = lastlastlastX;
              lastlastlastX = lastlastX;
              lastlastX = lastX;
              lastX = x;

              lastlastlastlastlastlastY = lastlastlastlastlastY;
              lastlastlastlastlastY = lastlastlastlastY;
              lastlastlastlastY = lastlastlastY;
              lastlastlastY = lastlastY;
              lastlastY = lastY;
              lastY = y;
              }
              else if(pasados[x][y+1][z] == 'P' && pasados[x-1][y][z] == 'P' && distanciaE > 15 && distanciaIE > 15)
              {
                lastlastlastlastlastlastX = lastlastlastlastlastX;
              lastlastlastlastlastX = lastlastlastlastX;
              lastlastlastlastX = lastlastlastX;
              lastlastlastX = lastlastX;
              lastlastX = lastX;
              lastX = x;

              lastlastlastlastlastlastY = lastlastlastlastlastY;
              lastlastlastlastlastY = lastlastlastlastY;
              lastlastlastlastY = lastlastlastY;
              lastlastlastY = lastlastY;
              lastlastY = lastY;
              lastY = y;
              }
          } break;
          case 'O':
            {
              if(pasados[x-1][y][z] == 'P' && pasados[x][y-1][z] == 'P' && distanciaDE > 15 && distanciaE > 15)
                {
                  lastlastlastlastlastlastX = lastlastlastlastlastX;
                  lastlastlastlastlastX = lastlastlastlastX;
                  lastlastlastlastX = lastlastlastX;
                  lastlastlastX = lastlastX;
                  lastlastX = lastX;
                  lastX = x;
    
                  lastlastlastlastlastlastY = lastlastlastlastlastY;
                  lastlastlastlastlastY = lastlastlastlastY;
                  lastlastlastlastY = lastlastlastY;
                  lastlastlastY = lastlastY;
                  lastlastY = lastY;
                  lastY = y;
                }
                else if(pasados[x-1][y][z] == 'P' && pasados[x+1][y][z] == 'P' && distanciaDE > 15 && distanciaIE > 15)
                {
                  lastlastlastlastlastX = lastlastlastlastX;
                  lastlastlastlastX = lastlastlastX;
                  lastlastlastX = lastlastX;
                  lastlastX = lastX;
                  lastX = x;
    
                  lastlastlastlastlastlastY = lastlastlastlastlastY;
                  lastlastlastlastlastY = lastlastlastlastY;
                  lastlastlastlastY = lastlastlastY;
                  lastlastlastY = lastlastY;
                  lastlastY = lastY;
                  lastY = y;
                }
                else if(pasados[x][y-1][z] == 'P' && pasados[x+1][y][z] == 'P' && distanciaE > 15 && distanciaIE > 15)
                {
                  lastlastlastlastlastX = lastlastlastlastX;
                  lastlastlastlastX = lastlastlastX;
                  lastlastlastX = lastlastX;
                  lastlastX = lastX;
                  lastX = x;
    
                  lastlastlastlastlastlastY = lastlastlastlastlastY;
                  lastlastlastlastlastY = lastlastlastlastY;
                  lastlastlastlastY = lastlastlastY;
                  lastlastlastY = lastlastY;
                  lastlastY = lastY;
                  lastY = y;
                }
            } break;
            case 'S':
              {
                if(pasados[x][y-1][z] == 'P' && pasados[x+1][y][z] == 'P' && distanciaDE > 15 && distanciaE > 15)
                  {
                    lastlastlastlastlastX = lastlastlastlastX;
                    lastlastlastlastX = lastlastlastX;
                    lastlastlastX = lastlastX;
                    lastlastX = lastX;
                    lastX = x;
      
                    lastlastlastlastlastlastY = lastlastlastlastlastY;
                    lastlastlastlastlastY = lastlastlastlastY;
                    lastlastlastlastY = lastlastlastY;
                    lastlastlastY = lastlastY;
                    lastlastY = lastY;
                    lastY = y;
                  }
                  else if(pasados[x][y-1][z] == 'P' && pasados[x][y+1][z] == 'P' && distanciaDE > 15 && distanciaIE > 15)
                  {
                    lastlastlastlastX = lastlastlastX;
                    lastlastlastX = lastlastX;
                    lastlastX = lastX;
                    lastX = x;
      
                    lastlastlastlastlastlastY = lastlastlastlastlastY;
                    lastlastlastlastlastY = lastlastlastlastY;
                    lastlastlastlastY = lastlastlastY;
                    lastlastlastY = lastlastY;
                    lastlastY = lastY;
                    lastY = y;
                  }
                  else if(pasados[x+1][y][z] == 'P' && pasados[x][y+1][z] == 'P' && distanciaE > 15 && distanciaIE > 15)
                  {
                    lastlastlastlastX = lastlastlastX;
                    lastlastlastX = lastlastX;
                    lastlastX = lastX;
                    lastX = x;
      
                    lastlastlastlastlastlastY = lastlastlastlastlastY;
                    lastlastlastlastlastY = lastlastlastlastY;
                    lastlastlastlastY = lastlastlastY;
                    lastlastlastY = lastlastY;
                    lastlastY = lastY;
                    lastY = y;
                  }
              } break;
    }
    
    ///// CONDICION 1 ///////

    if(seguidoAdelante < 9){
    
    if((distanciaDE==0 || distanciaDE > 20) && orientacion == 'N' && pasados[x][y+1][z] == 'P' && pasados[x][y+1][z] != 'V' && blackTile[x][y+1][z] != true)
      {
        seguidoAdelante = 0;
        lcd2.display();
        lcd2.print("DERECHA");
        robot.moveDer();
        robot.detenerse();
        //delay(10);
        lcd2.clear();
        alineaRobot();
        orientacion = 'E';
        
contador++;
        distanciaA=distanciaAtras();
        if(distanciaA < 20)
          {
            contador = 0;
            robot.moveAtras();
            delay(775);
            robot.actualizaSetpoint();
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            //delay(10);
             
          }
          robot.actualizaSetpoint();
      }
      //////////// CONDICION 1.5 ////////////
      else if((distanciaDE==0 || distanciaDE > 20) && orientacion == 'E' && pasados[x+1][y][z] == 'P' && pasados[x+1][y][z] != 'V' && blackTile[x+1][y][z] != true)
      {
        seguidoAdelante = 0;
        lcd2.display();
        lcd2.print("DERECHA");
        robot.moveDer();
        robot.detenerse();
        //delay(10);
        lcd2.clear();
        alineaRobot();
        orientacion = 'S';
contador++;
        distanciaA=distanciaAtras();
        lcd2.clear();
        if(distanciaA < 20)
          {
            robot.moveAtras();
            delay(775);
            robot.actualizaSetpoint();
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            //delay(10);
             
          }
          robot.actualizaSetpoint();
      }
      ///////////// CONDICION 2 //////////////
      else if((distanciaDE==0 || distanciaDE > 20) && orientacion == 'S' && pasados[x][y-1][z] == 'P' && pasados[x][y-1][z] != 'V' && blackTile[x][y-1][z] != true)
      {
        seguidoAdelante = 0;
        lcd2.display();
        lcd2.print("DERECHA");
        robot.moveDer();
        robot.detenerse();
        //delay(10);
        lcd2.clear();
        alineaRobot();
        orientacion = 'O';
contador++;
distanciaA=distanciaAtras();
        if(distanciaA < 20)
          {
            robot.moveAtras();
            delay(775);
            robot.actualizaSetpoint();
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            //delay(10);
             
          }
          robot.actualizaSetpoint();
      }
      //////////// CONDICION 3 ////////////
      else if((distanciaDE==0 || distanciaDE > 20) && orientacion == 'O' && pasados[x-1][y][z] == 'P' && pasados[x-1][y][z] != 'V' && blackTile[x-1][y][z] != true)
      {
        seguidoAdelante = 0;
        lcd2.display();
        lcd2.print("DERECHA");
        robot.moveDer();
        robot.detenerse();
        //delay(10);
        lcd2.clear();
        alineaRobot();
        orientacion = 'N';
contador++;
distanciaA=distanciaAtras();
        if(distanciaA < 20)
          {
            robot.moveAtras();
            delay(775);
            robot.actualizaSetpoint();
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            //delay(10);
             
          }
          robot.actualizaSetpoint();
      }
      /////////// CONDICION 4 /////////
      else if(distanciaE > 20 && orientacion == 'N' && pasados[x-1][y][z] == 'P' && pasados[x-1][y][z] != 'V' && blackTile[x-1][y][z] != true)
      {
        seguidoAdelante++;
        ignore();
        robot.actualizaSetpoint();
      }
      /////////// CONDICION 5 ///////////////
      else if(distanciaE > 20 && orientacion == 'E' && pasados[x][y+1][z] == 'P' && pasados[x][y+1][z] != 'V' && blackTile[x][y+1][z] != true)
      {
        seguidoAdelante++;
        ignore();
        robot.actualizaSetpoint();
      }
      ///////// CONDICION 6 //////////
      else if(distanciaE > 20 && orientacion == 'S' && pasados[x+1][y][z] == 'P' && pasados[x+1][y][z] != 'V' && blackTile[x+1][y][z] != true)
      {
        seguidoAdelante++;
        ignore();
        robot.actualizaSetpoint();
      }
      ////////// CONDICION 7 /////////////
      else if(distanciaE > 20 && orientacion == 'O' && pasados[x][y-1][z] == 'P' && pasados[x][y-1][z] != 'V' && blackTile[x][y-1][z] != true)
      {
        seguidoAdelante++;
        ignore();
        robot.actualizaSetpoint();
      }
      //////// CONDICION 8 ///////////
      else if((distanciaIE==0 || distanciaIE > 20) && orientacion == 'N' && pasados[x][y-1][z] == 'P' && pasados[x][y-1][z] != 'V' && blackTile[x][y-1][z] != true)
      {
        seguidoAdelante = 0;
        lcd2.display();
        lcd2.print("IZQUIERDA");
        robot.moveIzq();
        robot.detenerse();
        //delay(10);
        lcd2.clear();
        alineaRobot();
        orientacion = 'O';
contador++;
distanciaA=distanciaAtras();
        if(distanciaA < 20)
          {
            robot.moveAtras();
            delay(775);
            robot.actualizaSetpoint();
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            //delay(10);
             
          }
          robot.actualizaSetpoint();
      }
      ////////////// CONDICION 9 ///////////////
      else if((distanciaIE==0 || distanciaIE > 20) && orientacion == 'E' && pasados[x-1][y][z] == 'P' && pasados[x-1][y][z] != 'V' && blackTile[x-1][y][z] != true)
      {
        seguidoAdelante = 0;
        lcd2.display();
        lcd2.print("IZQUIERDA");
        robot.moveIzq();
        robot.detenerse();
        //delay(10);
        lcd2.clear();
        alineaRobot();
        orientacion = 'N';
contador++;
distanciaA=distanciaAtras();
        if(distanciaA < 20)
          {
            robot.moveAtras();
            delay(775);
            robot.actualizaSetpoint();
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            //delay(10);
             
          }
          robot.actualizaSetpoint();
      }
      ////////// CONDICION 10//////////////
      else if((distanciaIE==0 || distanciaIE > 20) && orientacion == 'S' && pasados[x][y+1][z] == 'P' && pasados[x][y+1][z] != 'V' && blackTile[x][y+1][z] != true)
      {
        seguidoAdelante = 0;
        lcd2.display();
        lcd2.print("IZQUIERDA");
        robot.moveIzq();
        robot.detenerse();
        //delay(10);
        lcd2.clear();
        alineaRobot();
        orientacion = 'E';
contador++;
distanciaA=distanciaAtras();
        if(distanciaA < 20)
          {
            robot.moveAtras();
            delay(775);
            robot.actualizaSetpoint();
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            //delay(10);
             
          }
          robot.actualizaSetpoint();
      }
      ///////////// CONDICION 11 ///////////7
      else if((distanciaIE==0 || distanciaIE > 20) && orientacion == 'O' && pasados[x+1][y][z] == 'P' && pasados[x+1][y][z] != 'V' && blackTile[x+1][y][z] != true)
      {
        seguidoAdelante = 0;
        lcd2.display();
        lcd2.print("IZQUIERDA");
        robot.moveIzq();
        robot.detenerse();
        //delay(10);
        lcd2.clear();
        alineaRobot();
        orientacion = 'S';
contador++;
distanciaA=distanciaAtras();
        if(distanciaA < 20)
          {
            robot.moveAtras();
            delay(775);
            robot.actualizaSetpoint();
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            //delay(10);
             
          }
          robot.actualizaSetpoint();
      }
      ///////////// CONDICION 12 ////////////
      else
      {
        seguidoAdelante = 0;       
                for(int i = 0; i < 15; i++){
                  for(int j = 0; j < 15; j++)
                    {
                      if(pasados[i][j][z] == 'P')
                        {
                          objX = lastX;
                          objY = lastY;

                          if((pasados[lastX+1][lastY][z] == 'V' || pasados[lastX+1][lastY][z] == 'I') && (pasados[lastX-1][lastY][z] == 'V' || pasados[lastX-1][lastY][z] == 'I') && (pasados[lastX][lastY+1][z] == 'V' || pasados[lastX][lastY+1][z] == 'I') && (pasados[lastX][lastY-1][z] == 'V' || pasados[lastX][lastY-1][z] == 'I'))
                            {
                              objX = i;
                              objY = j;
                            }

                          lastX = lastlastX;
                          lastlastX = lastlastlastX;
                          lastlastlastX = lastlastlastlastX;
                          lastlastlastlastX = lastlastlastlastlastX;
                          lastlastlastlastlastX = lastlastlastlastlastlastX;

                          lastY = lastlastY;
                          lastlastY = lastlastlastY;
                          lastlastlastY = lastlastlastlastY;
                          lastlastlastlastY = lastlastlastlastlastY;
                          lastlastlastlastlastY = lastlastlastlastlastlastY;
                            
                          i = 16;
                          j = 16;
                        }
                        else
                          {
                            objX = 7;
                            objY = 7;
                            objZ = 0;

                            if(z > objZ)
                            {
                              objX = rampaX;
                              objY = rampaY;
                            }
                          }
                    }
              }
              
        buscarObjetivo();
      }
  }
  else
  {
    distanciaIE = distanciaIzquierdaEnfrente();
    if(distanciaIE < 20)
    {
      robot.moveIzq();
    }
    else
    {
      robot.moveDer();
      robot.moveDer();
    }

    seguidoAdelante = 0;
  }
  }
  else
    {
      blackTile[x][y][z] = true;
      robot.moveAtras();
      delay(890);
      robot.detenerse();
      //delay(10);

      switch(orientacion)
       {
      case 'N':
        x++;
        break;
      case 'E':
        y--;
        break;
      case 'O':
        y++;
        break;
       case 'S':
       x--;
       break;  
      }

      distanciaDE = distanciaDerechaEnfrente();
      distanciaIE = distanciaIzquierdaEnfrente();

      if((distanciaDE > 20 || distanciaDE == 0) && orientacion == 'N' && pasados[x][y+1][z] == 'P' && blackTile[x][y+1][z] != true)
      {
        contador++;
        robot.moveDer();
        robot.detenerse();
        //delay(10);
        alineaRobot();
        orientacion = 'E';
distanciaA = distanciaAtras();
        if(distanciaA <20)
          {
            robot.moveAtras();
            delay(775);
            robot.actualizaSetpoint();
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            //delay(10);
             
          }
          robot.actualizaSetpoint();
      }
      else if((distanciaDE > 20 || distanciaDE == 0) && orientacion == 'E' && pasados[x+1][y][z] == 'P' && blackTile[x+1][y][z] != true)
      {
        contador++;
        robot.moveDer();
        robot.detenerse();
        //delay(10);
        alineaRobot();
        orientacion = 'S';
distanciaA = distanciaAtras();
        if(distanciaA <20)
          {
            robot.moveAtras();
            delay(775);
            robot.actualizaSetpoint();
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            //delay(10);
             
          }
          robot.actualizaSetpoint();
      }
      else if((distanciaDE > 20 || distanciaDE == 0) && orientacion == 'S' && pasados[x][y-1][z] == 'P' && blackTile[x][y-1][z] != true)
      {
        contador++;
        robot.moveDer();
        robot.detenerse();
        //delay(10);
        alineaRobot();
        orientacion = 'O';
distanciaA = distanciaAtras();
        if(distanciaA <20)
          {
            robot.moveAtras();
            delay(775);
            robot.actualizaSetpoint();
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            //delay(10);
             
          }
          robot.actualizaSetpoint();
      }
      else if((distanciaDE > 20 || distanciaDE == 0) && orientacion == 'O' && pasados[x-1][y][z] == 'P' && blackTile[x-1][y][z] != true)
      {
        contador++;
        robot.moveDer();
        robot.detenerse();
        //delay(10);
        alineaRobot();
        orientacion = 'N';
distanciaA = distanciaAtras();
        if(distanciaA <20)
          {
            robot.moveAtras();
            delay(775);
            robot.actualizaSetpoint();
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            //delay(10);
             
          }
          robot.actualizaSetpoint();
      }
      else if((distanciaIE > 20 || distanciaIE == 0) && orientacion == 'N' && pasados[x][y-1][z] == 'P' && blackTile[x][y-1][z] != true)
      {
        contador++;
        robot.moveIzq();
        robot.detenerse();
        //delay(10);
        alineaRobot();
        orientacion = 'O';
distanciaA = distanciaAtras();
        if(distanciaA <20)
          {
            robot.moveAtras();
            delay(775);
            robot.actualizaSetpoint();
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            //delay(10);
             
          }
          robot.actualizaSetpoint();
      }
      else if((distanciaIE > 20 || distanciaIE == 0) && orientacion == 'E' && pasados[x-1][y][z] == 'P' && blackTile[x-1][y][z] != true)
      {
        contador++;
        robot.moveIzq();
        robot.detenerse();
        //delay(10);
        alineaRobot();
        orientacion = 'N';
distanciaA = distanciaAtras();
        if(distanciaA <20)
          {
            robot.moveAtras();
            delay(775);
            robot.actualizaSetpoint();
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            //delay(10);
             
          }
          robot.actualizaSetpoint();
      }
      else if((distanciaIE > 20 || distanciaIE == 0) && orientacion == 'S' && pasados[x][y+1][z] == 'P' && blackTile[x][y+1][z] != true)
      {
        contador++;
        robot.moveIzq();
        robot.detenerse();
        //delay(10);
        alineaRobot();
        orientacion = 'E';
distanciaA = distanciaAtras();
        if(distanciaA <20)
          {
            robot.moveAtras();
            delay(775);
            robot.actualizaSetpoint();
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            //delay(10);
             
          }
          robot.actualizaSetpoint();
      }
      else if((distanciaIE > 20 || distanciaIE == 0) && orientacion == 'O' && pasados[x+1][y][z] == 'P' && blackTile[x+1][y][z] != true)
      {
        contador++;
        robot.moveIzq();
        robot.detenerse();
        //delay(10);
        alineaRobot();
        orientacion = 'S';
distanciaA = distanciaAtras();
        if(distanciaA <20)
          {
            robot.moveAtras();
            delay(775);
            robot.actualizaSetpoint();
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            //delay(10);
             
          }
          robot.actualizaSetpoint();
      }
      else{
        contador = contador +2;;
        robot.moveDer();
        robot.detenerse();
        //delay(10);
        alineaRobot();
        robot.moveDer();
        robot.detenerse();
        //delay(10);
        alineaRobot();
        if(orientacion == 'N')
          orientacion = 'S';
          else if(orientacion == 'E')
            orientacion = 'O';
            else if(orientacion == 'S')
              orientacion = 'N';
                else if(orientacion == 'O')
                  orientacion = 'E';

robot.actualizaSetpoint();
       distanciaE = distanciaEnfrente();
       

       if(distanciaE > 20)
        {
          ignore();
        }
        else
        {
          if(distanciaE > 10 && distanciaE < 20)
           {
            contador++;
            while(distanciaE > 10){
              distanciaE = distanciaEnfrente();
              robot.moveAdelante();}
           }
          robot.moveDer();
          robot.actualizaSetpoint();
          robot.detenerse();
          //delay(10);
          alineaRobot();
          switch(orientacion)
          {
            case 'N':
              orientacion = 'E';
              break;
             case 'E':
              orientacion = 'S';
              break;
             case 'S':
              orientacion = 'O';
              break;
             case 'O':
              orientacion = 'N';
              break;
          }
        }
      }
    }
  
}
