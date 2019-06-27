
#include <SoftwareSerial.h>
// software serial #1: RX = digital pin 10, TX = digital pin 11
SoftwareSerial camDer(13, 51); // EL 13 VA AL P4 DE LA CÁMARA DERECHA

// software serial #2: RX = digital pin 8, TX = digital pin 9
// on the Mega, use other pins instead, since 8 and 9 don't work on the Mega
SoftwareSerial camIzq(12, 50); // EL 12 VA AL P4 DE LA CÁMARA IZQUIERDA

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  camDer.begin(9600); // INICIALIZAMOS LA COMUNICACION DE LA CAMARA DERECHA
  camIzq.begin(9600); // INICIALIZAMOS LA CMOUNICACION DE LA CAMARA IZQUIERDA
}

void loop() {

  camDer.listen();  // ATENDER SOLO CAMARA DERECHA
  Serial.println("Data from port one:");

  while (camDer.available() > 0) {
    char inByte = camDer.read();
    Serial.write(inByte);
    if(inByte == '6'){ // 6 ES PARA LAS VICTIMAS H DEL LADO DERECHO
      Serial.println();
      Serial.println("Harmed victim from the Right Side");
      delay(2000);
      break;
    }
    if(inByte == '5'){ // 5 ES PARA LAS VICTIMAS S DE LADO DERECHO
      Serial.println();
      Serial.println("Stable victim from the Right Side");
      delay(2000);
      break;
    }
    if(inByte == '4'){ // 4 ES PARA LAS VICTIMAS U DEL LADO DERECHO
      Serial.println();
      Serial.println("Unharmed victim from the Right Side");
      delay(2000);
      break;
    }
  }

  Serial.println();

  camIzq.listen();

  Serial.println("Data from port two:");
  while (camIzq.available() > 0) {
    char inByte = camIzq.read();
    Serial.write(inByte);
    if(inByte == '3'){ // 3 ES PARA LAS VICTIMAS H DEL LADO IZQUIERDO
      Serial.println();
      Serial.println("Harmed victim from the Left Side");
      delay(2000);
      break;
    }
    if(inByte == '2'){ // 2 ES PARA LAS VICTIMAS S DEL LADO IZQUIERDO
      Serial.println();
      Serial.println("Stable victim from the Left Side");
      delay(2000);
       break;
    }
    if(inByte == '1'){ // 1 ES PARA LAS VICTIMAS U DEL LADO IZQUIERDO
      Serial.println();
      Serial.println("Unharmed victim from the Left Side");
      delay(2000);
      break;
    }
  }

  // blank line to separate data from the two ports:
  Serial.println();
  
}
