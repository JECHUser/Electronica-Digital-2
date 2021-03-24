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

void setup(){
  Serial.begin(9600);       // baudaje -> 9600
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(PB1, INPUT_PULLUP);
  pinMode(PB2, INPUT_PULLUP);
  pinMode(PBS, INPUT_PULLUP);
}

void loop(){
  PB1State = digitalRead(PB1);
  PB2State = digitalRead(PB2);
  PBSState = digitalRead(PBS);
  digitalWrite(LED1, LOW);

  if (PBSState == 0){
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    delay(1000);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
    delay(1000);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
    delay(1000);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    }
  
  if(PB1State == 0){
    digitalWrite(LED1, HIGH);
    if (PJ1 == 0) {
      PJ1 = 1;
      }
    PJ1 = PJ1<<1;
    delay(200);
  }
  Serial.print("PJ1: ");
  Serial.println(PJ1, BIN);

  if(PB2State == 0){
    digitalWrite(LED1, HIGH);
    if (PJ2 == 0) {
      PJ2 = 1;
      }
    PJ2 = PJ2<<1;
    GPIO Port B () = PJ2;
    delay(200);
  }
  Serial.print("PJ2: ");
  Serial.println(PJ2, BIN);

  if (PJ1 == 128){
    digitalWrite(LED3, HIGH);
    }
  if (PJ2 == 128){
    digitalWrite(LED2, HIGH);
    }
}
