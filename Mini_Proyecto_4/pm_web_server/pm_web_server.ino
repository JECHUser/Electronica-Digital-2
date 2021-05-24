/*************************************************************************************************
  ESP32 Web Server
  Ejemplo de creación de Web server 
  Basándose en los ejemplos de: 
  https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
  https://electropeak.com/learn
**************************************************************************************************/
//************************************************************************************************
// Librerías
//************************************************************************************************
#include <WiFi.h>
#include <WebServer.h>

#define RXD2 16
#define TXD2 17
//************************************************************************************************
// Variables globales
//************************************************************************************************
// SSID & Password
const char* ssid = "LAPTOP-AFK50DFS 2653";  // Enter your SSID here
const char* password = "queso123";  //Enter your Password here

uint8_t dataIn;

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

//************************************************************************************************
// Configuración
//************************************************************************************************
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

  Serial.print("Try Connecting to ");
  Serial.println(ssid);

  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  server.on("/init", handle_OnConnect); // Directamente desde e.g. 192.168.0.8
  
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  delay(100);
}
//************************************************************************************************
// loop principal
//************************************************************************************************
void loop() {
  server.handleClient();
  while(Serial2.available()){
    dataIn = Serial2.read(); 
  }
  Serial.println(dataIn);
}
//************************************************************************************************
// Handler de Inicio página
//************************************************************************************************
void handle_OnConnect() {
  Serial.println("Parking Status");
  server.send(200, "text/html", SendHTML(dataIn));
}
//************************************************************************************************
// Procesador de HTML
//************************************************************************************************
String SendHTML(uint8_t occupied_spaces) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>Parqueo-matic - Web Server</title>\n";
  ptr += "<style>\n";
  ptr += "body {background-color: black; font-family: sans-serif; }\n";
  ptr += "h1 {color: white; font-family: sans-serif; text-align:center; }\n";
  ptr += "h2 {color: white; font-family: sans-serif; }\n";
  ptr += "h3 {color: white; font-family: sans-serif; }\n";
  ptr += "p {color: white; font-family: sans-serif; }\n";
  ptr += ".boxes{text-align: center; background: #242424; height: 170px; position: relative; border-radius: 10px 10px 10px 10px; -moz-border-radius: 10px 10px 10px 10px; -webkit-border-radius: 10px 10px 10px 10px; border: 2px solid #5878ca;}\n";
  ptr += ".greenbox{margin-top: 50px; background-color: green; width: 100px; height: 25px; font-family: sans-serif; font-size: 18px; font-weight: 400; color: #ffffff; padding: 20px; display: inline-block; border-radius: 10px 10px 10px 10px; -moz-border-radius: 10px 10px 10px 10px; -webkit-border-radius: 10px 10px 10px 10px; border: 2px solid #42A41B;}\n";
  ptr += ".redbox{margin-top: 50px; background-color: #91160E ; width: 100px; height: 25px; font-family: sans-serif; font-size: 18px; font-weight: 400; color: #ffffff; padding: 20px;display: inline-block; border-radius: 10px 10px 10px 10px; -moz-border-radius: 10px 10px 10px 10px; -webkit-border-radius: 10px 10px 10px 10px; border: 2px solid #D81A0E;}\n";
  ptr += ".button {display: block;width: 80px;background-color: grey;border: none;color: white;padding: 20px 20px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #3498db;}\n";
  ptr += ".button-on:active {background-color: #2980b9;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  
  ptr += "<body>\n";
  ptr += "<hr>\n";
  ptr += "<h1>Parqueo-matic Web Server</h1>\n";
  ptr += "<hr>\n";
  ptr += "<br>\n";
  ptr += "<h3>Estado de los parqueos</h3>\n";
  ptr += "<a class=\"button button-on\" href=\"/init\">Refresh</a>\n";
  ptr += "<div class=\"boxes\">\n";

  if (occupied_spaces == 49){
    ptr += "<div class=\"redbox\">Parqueo 1</div>\n";
  }
  else{
    ptr += "<div class=\"greenbox\">Parqueo 1</div>\n";
  }
  
  if (occupied_spaces == '1'){
    ptr += "<div class=\"redbox\">Parqueo 2</div>\n";
  }
  else{
    ptr += "<div class=\"greenbox\">Parqueo 2</div>\n";
  }
  
  if (occupied_spaces == 3){
    ptr += "<div class=\"redbox\">Parqueo 3</div>\n";
  }
  else{
    ptr += "<div class=\"greenbox\">Parqueo 3</div>\n";
  }
  
  if (occupied_spaces == 4){
    ptr += "<div class=\"redbox\">Parqueo 4</div>\n";
  }
  else{
    ptr += "<div class=\"greenbox\">Parqueo 4</div>\n";
  }
  
  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
//************************************************************************************************
// Handler de not found
//************************************************************************************************
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
