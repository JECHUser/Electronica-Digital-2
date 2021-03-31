/*
 Electrónica Digital 2
 Laboratorio 5
 Jonatan E. Chocón Gómez, 18420

 Parte del código pertenece a: David A. Mellis
 
 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

#include <SPI.h>
#include <SD.h>

String name;
File myFile;
File root;

void setup()
{
  Serial.begin(9600);
  SPI.setModule(0);   // selección del módulo SPI de la Tiva C
  while (!Serial) {}  // espera que se abra la terminal...
  
  Serial.print("Inicialización de la tarjeta SD... ");
  pinMode(PA_3, OUTPUT);    // CS PIN como sálida

  if (!SD.begin(PA_3)) {   
    Serial.println("¡Inicialización fallida!");
    return;
  }
  Serial.println("Inicialización realizada con éxito.");

  root = SD.open("/");      // selección carpeta raíz de SD
  Serial.println("A continuación se muestra el contenido de la tarjeta SD: ");
  Serial.println("*********************************************************");
  printDirectory(root, 0);  // llamada función de impresión de directorio raíz
  Serial.println("*********************************************************");
  Serial.println("");
  Serial.println("Listo, ingrese el nombre de un archivo con su extensión: ");
}

void loop()
{
  readArchive();
}

void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       break;   //
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());  // imprime el nombre
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // para los directorios no se despliegan los tamaños de archivo
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}

void readArchive(){
  if (Serial.available()){
    name = Serial.readString();     // guardamos la entrada de texto en el monitor

    myFile = SD.open(name.c_str()); // abrimos el archivo
    if (myFile) {
      Serial.println("A continuación se muestra el contenido: ");
      while (myFile.available()) {    // archivo disponible para apertura?
        Serial.write(myFile.read());  // lectura del contenido
      }
    // close the file:
    myFile.close();                 // cerramos archivo
    } else {
      // en caso de errores
      Serial.print("Error al abrir el archivo, revise que el nombre sea correcto."); 
      Serial.print("Además, recuerde agregar la externsión del archivo.");
    }
  }
}
