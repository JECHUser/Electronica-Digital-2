#define RXD2 16
#define TXD2 17

char Read_pic;
char Write_pic;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop() {
  Serial2.print(Write_pic);
  while (Serial2.available()){
    Read_pic = Serial2.read();
    }
}
