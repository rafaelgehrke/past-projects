#include <Herkulex.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <SPI.h>
#include <Wire.h>
int laser = 252; //motor ID - verify your ID !!!!
int ldr = 253;

Adafruit_PCD8544 display = Adafruit_PCD8544(46, 48, 50, 49, 51);
byte releLaser = 53;
byte confirma = 35;
byte troca = 47;
byte estado = 1;
void setup()  
{
  display.begin();
  display.setContrast(50); //Ajusta o contraste do display
  display.clearDisplay();   //Apaga o buffer e o display
  display.setTextSize(1);  //Seta o tamanho do texto
  display.setTextColor(BLACK); //Seta a cor do texto
  display.setCursor(0,0);  //Seta a posição do cursor
  display.println("Contactless   Calliper");  
  display.display();
  delay(2000);  //a delay to have time for serial monitor opening
  Serial.begin(115200);    // Open serial communications
  Serial.println("Begin");
  pinMode(releLaser, OUTPUT);
  pinMode(troca, INPUT);
  pinMode(confirma, INPUT);
  digitalWrite(releLaser, 1);
  Herkulex.beginSerial3(115200); //open serial port 3
  Herkulex.reboot(ldr); //reboot first motor
  Herkulex.reboot(laser); //reboot first motor
  delay(500); 
  Herkulex.initialize(); //initialize motors
  delay(500);
  
}
float i1, i2;
float absLaser;
float absLdr;
int potLdr[160];
void loop(){ 
  for(int resetLdr = 0; resetLdr < 160; resetLdr++){
    potLdr[resetLdr] = 1000;
  }
  /*Herkulex.moveOneAngle(ldr, 30, 2000, LED_GREEN);    // 40 = 41.5  30 = 45     //ldr           laser
  Herkulex.moveOneAngle(laser, 95, 2000, LED_GREEN); //-25 = 49  95 = 61        //30 = 45       -55 = 45
  delay(2000);                 //                                             //-110 = 61     95 = 61
  Herkulex.moveOneAngle(ldr, -110, 2000, LED_GREEN);    //61
  Herkulex.moveOneAngle(laser, -55, 2000, LED_GREEN);     //45
  delay(2000);
  Herkulex.moveOneAngle(ldr, 40, 2000, LED_GREEN);    //41.5
  Herkulex.moveOneAngle(laser, -20, 2000, LED_GREEN);
 delay(2000); */
  Herkulex.moveOneAngle(ldr, -110, 2000, LED_GREEN);    //61
  Herkulex.moveOneAngle(laser, 95, 2000, LED_GREEN);     //45
  delay(2000);
  //mover com resolução de 0.1 grau cada modulo de 61 graus até encontrar menor valor ou 50 graus
  //laser: converter entrada (45 -> 61) em 2 saídas: ldr: 30 -> -110 e laser: -55 -> 95   porem, ldr oscila +-5 graus e pega valores!
  for(int ang = 610; ang > 450; ang--){
    Herkulex.moveOneAngle(ldr, (14*ang -6780)/(-16), 100,LED_GREEN);
    Herkulex.moveOneAngle(laser, ang - 505, 100,LED_GREEN);
    delay(100);
    Herkulex.moveOneAngle(ldr, (14*(ang - 50) - 6780)/(-16), 100,LED_GREEN);
    delay(100);
    for(int osc = ang - 50; osc < ang + 50; osc++){ //ldr oscilando 5 graus     38
      if(osc > 610){
      }
      else{
        Herkulex.moveOneAngle(ldr, (14*ang -6780)/(-16), 20,LED_GREEN);
        delay(20);
      }
      if(analogRead(A0) < potLdr[ang - 451]){
        potLdr[ang - 451] = analogRead(A0);
      }
    }
  }
  int low = 0;
  for(int i = 1; i < 160; i++){
    if(potLdr[i] < potLdr[low]){
      low = i;
    }
  }
  Serial.println("Low: ");
  Serial.println(potLdr[low]);
  
  while(1){
    display.clearDisplay();   //Apaga o buffer e o display
    display.setTextSize(1);  //Seta o tamanho do texto
    display.setTextColor(BLACK); //Seta a cor do texto
    display.setCursor(0,0);  //Seta a posição do cursor
    display.println("Keep moving   until it      calibrates...");  
    display.display();
    //colocar o loop aki
    display.clearDisplay();   //Apaga o buffer e o display
    display.setTextSize(1);  //Seta o tamanho do texto
    display.setTextColor(BLACK); //Seta a cor do texto
    display.setCursor(0,0);  //Seta a posição do cursor
    display.println("Calibration   completed");  
    display.display();
    delay(100000);
    break;
  }
  if(digitalRead(troca) == 1){
    delay(5);
    while(digitalRead(troca) == 1);
    delay(5);
    estado++;
    if(estado == 4) estado = 1;
  }
  if(estado == 1){
    display.clearDisplay();   //Apaga o buffer e o display
    display.setTextSize(1);  //Seta o tamanho do texto
    display.setTextColor(BLACK); //Seta a cor do texto
    display.setCursor(0,0);  //Seta a posição do cursor
    display.println("Refractive    Index Mode");  
    display.display();
  }
  else if(estado == 2){
    display.clearDisplay();   //Apaga o buffer e o display
    display.setTextSize(1);  //Seta o tamanho do texto
    display.setTextColor(BLACK); //Seta a cor do texto
    display.setCursor(0,0);  //Seta a posição do cursor
    display.println("Thickness Mode");  
    display.display();
  }
  else if(estado == 3){
    display.clearDisplay();   //Apaga o buffer e o display
    display.setTextSize(1);  //Seta o tamanho do texto
    display.setTextColor(BLACK); //Seta a cor do texto
    display.setCursor(0,0);  //Seta a posição do cursor
    display.println("Reflectance   Mode");  
    display.display();
  }
  if(digitalRead(confirma) == 1){
   // display.clearDisplay();   //Apaga o buffer e o display
    display.setTextSize(1);  //Seta o tamanho do texto
    display.setTextColor(BLACK); //Seta a cor do texto
    display.setCursor(0,20);  //Seta a posição do cursor
    display.println("Mode          Selected");  
    display.display();
    //checar calibração
    if(estado == 1){
      Herkulex.moveOneAngle(ldr, -120, 3000, LED_GREEN);
      Herkulex.moveOneAngle(laser, 120, 3000, LED_GREEN);
      delay(3000);
      for(int baseDeg = -120; baseDeg < 120; baseDeg++){
      }
    }
    if(estado == 2){
    }
    if(estado == 3){
    }
    delay(10000);
  }
  //delay(1000);
}


