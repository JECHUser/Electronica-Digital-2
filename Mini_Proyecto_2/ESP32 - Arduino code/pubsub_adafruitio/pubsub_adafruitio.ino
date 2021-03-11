// Adafruit IO Publish & Subscribe Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

#include "config.h"

/************************ Example Starts Here *******************************/

// definición de algunas valores
#define RXD2 16
#define TXD2 17
#define IO_LOOP_DELAY 5000

// definción de variables
unsigned long lastUpdate = 0;
int count = 0;
char Read_pic;
char Write_pic;
char state_p1;
char state_p2;

// configurar el feed 'P1'
AdafruitIO_Feed *P1 = io.feed("P1");

// configurar el feed 'P2'
AdafruitIO_Feed *P2 = io.feed("P2");

// configurar el feed 'SENSOR'
AdafruitIO_Feed *SENSOR = io.feed("SENSOR");

void setup() {

  // Inicar la comunicación serial
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);  // Serial2 utilizado para comunicación I2C

  // Espra que la terminal se abra
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // Conexión con io.adafruit.com
  io.connect();

  // Recepción de los mensaje de adafruit io
  P1->onMessage(handleP1);
  P2->onMessage(handleP2);
  SENSOR->onMessage(handleSensor);

  // Espera la conexión con adafruit io
  while(io.status() < AIO_CONNECTED) {
    Serial.println(io.statusText());
    delay(500);
  }

  // Indicadores de la conexión con adafruit io
  Serial.println();
  Serial.println(io.statusText());

  // obtener/enviar datos desde adafruit
  P1->get();
  P2->get();
  SENSOR->get();
}

void loop() {
  
  io.run(); // mantener conexión con io.adafruit.com
  
  while (Serial2.available()){
    Read_pic = Serial2.read();        // lee el dato enviado por comunicación UART al Serial2
    Write_pic = state_p1 + state_p2;  // operación del estado de las luces piloto
    Serial2.write(Write_pic);         // escribe el estado de las luces piloto y las envía por comunicación I2C
    }
    
  if (millis() > (lastUpdate + IO_LOOP_DELAY)) {
    // Guarda el valor leído en el feed 'SENSOR' de adafruit IO
    SENSOR->save(Read_pic);
    // Después de almacenar el valor, guarda el tiempo actual
    lastUpdate = millis();
  }
}

// Funciones llamadas cuando un mensaje enviado por Adafruit IO es recibido

void handleSensor(AdafruitIO_Data *data) {    // onMessage del feed 'SENSOR'
  Serial.print("received <- ");
  Serial.println(data->value());
}

void handleP1(AdafruitIO_Data *data) {        // onMessage del feed 'P1'
  Serial.print("received <- ");
 
  if(data->isTrue()){                 
    state_p1 = 1;     // si el botón se presiona, entonces state_p1 = 0b00000001
    Serial.println("P1 - HIGH");
  }
  else{
    state_p1 = 0;     // si el botón se presiona, entonces state_p1 = 0b00000000
    Serial.println("P1 - LOW");
  }
}

void handleP2(AdafruitIO_Data *data) {        // onMessage del feed 'P2'
  Serial.print("received <- ");
 
  if(data->isTrue()){
    state_p2 = 2;   // si el botón se presiona, entonces state_p2 = 0b00000010
    Serial.println("P2 - HIGH");
  }
  else{
    state_p2 = 0;   // si el botón se presiona, entonces state_p2 = 0b00000000
    Serial.println("P2 - LOW");
  }
}
