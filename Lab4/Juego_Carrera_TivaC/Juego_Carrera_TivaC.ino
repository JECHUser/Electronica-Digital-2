// Universidad del Valle de Guatemala
// Electrónica Digital 2, Sección 30
// Jonatan E. Chocón Gómez, 18420

// Juego de carrera (Tiva C)

#define LED1 RED_LED
#define LED2 BLUE_LED
#define LED3 GREEN_LED
#define PB1 PUSH1
#define PB2 PUSH2
#define PBS PE_3

uint8_t PJ1 = 0;
uint8_t PJ2 = 0;

int PBSState = 0;
int PB1State = 0;
int PB2State = 0;
int pressPB1 = 0;
int pressPB2 = 0;

void setup(){
  Serial.begin(9600);       // baudaje -> 9600
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(PB1, INPUT_PULLUP);
  pinMode(PB2, INPUT_PULLUP);
  pinMode(PBS, INPUT_PULLUP);
}

//----------------------------------------------------------------------------------------

void loop(){
  // lectura digital de pushbutton principal
  PBSState = digitalRead(PBS);

  if (PBSState == 0){
    // secuencia de semáforo
    digitalWrite(LED1, HIGH); // rojo encendido - delay 1s entre para cada luz
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    delay(1000);
    digitalWrite(LED1, LOW);  // amarillo (azul) encendido
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
    delay(1000);
    digitalWrite(LED1, LOW);  // verde encendido
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
    delay(1000);
    digitalWrite(LED1, LOW);  // todos apagados
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    }
  //----------------------------------------------------------------------------------------
  // lectura digital de pushbutton SW1 
  PB1State = digitalRead(PB1);

  // se presionó el botón? (progra de antirebote)
  if(PB1State == 0){
    pressPB1 = 1;
  }
  if(PB1State == LOW && pressPB1 == 1){
    if (PJ1 == 0) {
      PJ1 = 1;
      }
    PJ1 = PJ1<<1;   // corrimiento de bits, para aumento en decadas
    pressPB1 = 0;
    delay(200);
  }
  //----------------------------------------------------------------------------------------
  // lectura digital de pushbutton SW2
  PB2State = digitalRead(PB2);
  
  // se presionó el botón? (progra de antirebote)
  if(PB2State == 0){
    pressPB2 = 1;
  }
  if(PB2State == LOW && pressPB2 == 1){
    if (PJ2 == 0) {
      PJ2 = 1;
      }
    PJ2 = PJ2<<1;   // corrimiento de bits, para aumento en decadas
    pressPB2 = 0;
    delay(200);
  }
  //----------------------------------------------------------------------------------------
  // se enciende bit final?
  if (PJ1 == 128){
    digitalWrite(LED3, HIGH);   // gana jugador 1
    }
  if (PJ2 == 128){
    digitalWrite(LED2, HIGH);   // gana jugador 2
    }
}
