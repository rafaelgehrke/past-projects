#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <SPI.h>
#include <Wire.h>
#include <primbib.h>
#include <DualVNH5019MotorShield.h>

Servo servoesq1;
Servo servodir1;
Servo servoesq2;  
Servo servodir2;
Servo cacamba;

#define Register_ID 0
#define Register_2D 0x2D
#define Register_X0 0x32
#define Register_X1 0x33

// Endereco I2C do sensor : 83 em decimal ou 0x53
int ADXAddress = 0x53;  // the default 7-bit slave address
int reading = 0;
int val = 0;
int X0,X1,X_out;
int Y0,Y1,Y_out;
int Z1,Z0,Z_out;
double Xg,Yg,Zg;


Adafruit_PCD8544 display = Adafruit_PCD8544(36, 37, 38, 39, 40);
DualVNH5019MotorShield md;
//Endereco I2C do L3G4200D

const byte encEsq1 = 23, encDir1 = 22, encDir2 = 24, encEsq2 = 25;
//          azul             azul

bool patLeft = 0;
bool patCenter = 0;
bool patRight = 0;

int pot = 120;
int basePot = 120;
int erro = 0;
int lasterro = 0;
int valPotRampa = 0;
int valNormal = 0;

bool prefEsq = 0;
const int brancoD = 514;

const int brancoE = 438;

const int brancoD2 = 698;

const int brancoE2 = 649;

const int pretoD = 857;

const int pretoE2 = 879;

const int pretoD2 = 906;

const int pretoE = 894;

const int prataD = 758;

const int prataE = 755;

const int verdeDir = -9;

const int verdeEsq = 8;



float kp = 3;    //1.5       2       4   5.4     3
float baseKp = 3;
float baseKd = 35;
float kd = 35;      //15     30     150     6    50

const byte corD = A7;//parmas2
const byte corD2 = A2;
const byte corE2 = A3;
const byte corE = A8;//cenzo2
const byte corC = A4;
const byte verdeE = A6;
const byte verdeD = A5;
int absDir = 0;
int absEsq = 0;
int absEsq2 = 0;
int absDir2 = 0;

byte buz = 5;
byte trigFre = 31;
byte echoFre = 30;
byte trigEsq = 33;
byte echoEsq = 32;
byte echoDir = 34;
byte trigDir = 35;
byte echoWall = 3;
byte trigWall = 11;

byte redLeft = 47;
byte greenLeft = 48;
byte blueLeft = 46;

byte redRight = 26;
byte greenRight = 27;
byte blueRight = 28;

bool stopSensors = 0, stopFollow = 0, stopRampa = 0, stopObst = 0, stopThird = 0, stopDebug = 0, stopVerde = 0, stopSalvaGap = 0, stopMenor = 1;
long timeSensors = 2, timeFollow = 2, timeRampa = 10, timeObst = 50, timeThird = 100, timeDebug = 500, timeVerde = 20, timeSalvaGap = 10, timeMenor = 10;
long lastSensors = 0, lastFollow = 0, lastRampa = 0, lastObst = 0, lastThird = 0, lastDebug = 0, lastVerde = 0, lastSalvaGap = 0, lastMenor = 0;

int potM1;
int potM2;

/*****************************************************************************************************************************/
int greenValEsq = 0, blueValEsq = 0;
  int greenValDir = 0, blueValDir = 0;
  int offDir = 0, offEsq = 0;
  int onDir = 0, onEsq = 0;
/***************************************************************************************************************************/
void checkRapido(){
  if(getUltra(trigFre, echoFre, 8) <= 5 ){
          int contGarraDebug = 0;
          breakMotors();
          servoesq1.write(175);//grau mais baixo, garra mais alta
          servodir1.write(8);//grau mais pra baixo , garra mais pra baixo                     em baixo segurando bolinha : esq1 170  ;   dir1  20    ;   esq2  180  ;  dir2   0;
          for(contGarraDebug = 70; contGarraDebug >= 10; contGarraDebug--){
          servoesq2.write(185 - contGarraDebug);//maior grau fecha  
          servodir2.write(contGarraDebug);//menor grau fecha 
          delay(10);
          }
          servoesq1.write(10);//grau mais baixo, garra mais alta
          servodir1.write(172);//grau mais pra baixo , garra mais pra baixo                     em baixo segurando bolinha : esq1 170  ;   dir1  20    ;   esq2  180  ;  dir2   0;
          servoesq2.write(175);//maior grau fecha  
          servodir2.write(10);//menor grau fecha   
          delay(2000);
          servoesq1.write(10);//grau mais baixo, garra mais alta
          servodir1.write(172);//grau mais pra baixo , garra mais pra baixo                     em baixo segurando bolinha : esq1 170  ;   dir1  20    ;   esq2  180  ;  dir2   0;
          servoesq2.write(90);//maior grau fecha  
          servodir2.write(90);//menor grau fecha 
          delay(1500);
          servoesq1.write(175);//grau mais baixo, garra mais alta
          servodir1.write(7);//grau mais pra baixo , garra mais pra baixo                     em baixo segurando bolinha : esq1 170  ;   dir1  20    ;   esq2  180  ;  dir2   0;
          servoesq2.write(130);//maior grau fecha  
          servodir2.write(70);//menor grau fecha
          delay(2000);
        }
}

void moveForBac(int degree, int pot, int timeOff) {
  byte encVal = digitalRead(encEsq1);
  byte encVal2 = digitalRead(encDir1);
  int contPasq = 0, contPasq2 = 0;
  if (degree > 0) {
    while (contPasq < degree) {
      if (digitalRead(encEsq1) != encVal) {
        encVal = digitalRead(encEsq1);
        contPasq++;
      }
      if (digitalRead(encDir1) != encVal2) {
        encVal2 = digitalRead(encDir1);
        contPasq2++;
      }
      md.setM1Speed(pot*0.95);
      md.setM2Speed(pot);
    }
  }
  else {
    while (contPasq < -degree){
      if (digitalRead(encEsq1) != encVal){
        encVal = digitalRead(encEsq1);
        contPasq++;
        //Serial.println(contPasq);
      }
      md.setM1Speed(pot*0.95);
      md.setM2Speed(pot);
    }
  }
  breakMotors();
  delay(timeOff);
}

void moveForBacEsp(int degree, int pot, int timeOff) {
  byte encVal = digitalRead(encEsq1);
  byte encVal2 = digitalRead(encDir1);
  int contPasq = 0, contPasq2 = 0;
  if (degree > 0) {
    while (contPasq < degree) {
      if (digitalRead(encEsq1) != encVal) {
        encVal = digitalRead(encEsq1);
        contPasq++;
      }
      if (digitalRead(encDir1) != encVal2) {
        encVal2 = digitalRead(encDir1);
        contPasq2++;
      }
      md.setM1Speed(pot*0.82);
      md.setM2Speed(pot);
    }
  }
  else {
    while (contPasq < -degree) {
      if (digitalRead(encEsq1) != encVal) {
        encVal = digitalRead(encEsq1);
        contPasq++;
        //Serial.println(contPasq);
      }
      md.setM1Speed(pot*0.98);
      md.setM2Speed(pot);
    }
  }
  breakMotors();
  delay(timeOff);
}

void moveForBacBolEsp(int degreee, int pot, int timeOff) {
  long lastUltraBolCheck = 0;
  byte encVale = digitalRead(encEsq1);
  byte encVale2 = digitalRead(encDir1);
  int contPasqe = 0, contPasqe2 = 0;
  if (degreee > 0) {
    while (contPasqe < degreee) {
      if(contPasqe%20 > 10){
        servoesq1.write(175);//grau mais baixo, garra mais alta
        servodir1.write(7);//grau mais pra baixo , garra mais pra baixo                     em baixo segurando bolinha : esq1 170  ;   dir1  20    ;   esq2  180  ;  dir2   0;
        servoesq2.write(123);//maior grau fecha  
        servodir2.write(67);//menor grau fecha
      }
      else{
        servoesq1.write(175);//grau mais baixo, garra mais alta
        servodir1.write(7);//grau mais pra baixo , garra mais pra baixo                     em baixo segurando bolinha : esq1 170  ;   dir1  20    ;   esq2  180  ;  dir2   0;
        servoesq2.write(120);//maior grau fecha  
        servodir2.write(70);//menor grau fecha
      }
      if (digitalRead(encEsq1) != encVale) {
        encVale = digitalRead(encEsq1);
        contPasqe++;
      }
      if (digitalRead(encDir1) != encVale2) {
        encVale2 = digitalRead(encDir1);
        contPasqe2++;
      }
      md.setM1Speed(pot*0.8);
      md.setM2Speed(pot);
      if(millis() - lastUltraBolCheck > 10){
        lastUltraBolCheck = millis();
        if(getUltra(trigFre, echoFre, 8) <= 5 ){
          int contGarraDebug = 0;
          breakMotors();
          for(contGarraDebug = 70; contGarraDebug >= 10; contGarraDebug--){
          servoesq2.write(185 - contGarraDebug);//maior grau fecha  
          servodir2.write(contGarraDebug);//menor grau fecha 
          delay(10);
          }
          servoesq1.write(10);//grau mais baixo, garra mais alta
          servodir1.write(172);//grau mais pra baixo , garra mais pra baixo                     em baixo segurando bolinha : esq1 170  ;   dir1  20    ;   esq2  180  ;  dir2   0;
          servoesq2.write(175);//maior grau fecha  
          servodir2.write(10);//menor grau fecha   
          delay(2000);
          servoesq1.write(10);//grau mais baixo, garra mais alta
          servodir1.write(172);//grau mais pra baixo , garra mais pra baixo                     em baixo segurando bolinha : esq1 170  ;   dir1  20    ;   esq2  180  ;  dir2   0;
          servoesq2.write(90);//maior grau fecha  
          servodir2.write(90);//menor grau fecha 
          delay(1500);
          servoesq1.write(175);//grau mais baixo, garra mais alta
          servodir1.write(7);//grau mais pra baixo , garra mais pra baixo                     em baixo segurando bolinha : esq1 170  ;   dir1  20    ;   esq2  180  ;  dir2   0;
          servoesq2.write(110);//maior grau fecha  
          servodir2.write(70);//menor grau fecha
          delay(2000);
        }
      }
    }
  }
  else {
    while (contPasqe < -degreee) {
      if (digitalRead(encEsq1) != encVale) {
        encVale = digitalRead(encEsq1);
        contPasqe++;
        //Serial.println(contPasq);
      }
      md.setM1Speed(pot*0.9);
      md.setM2Speed(pot);
    }
  }
  breakMotors();
  delay(timeOff);
}

void moveForBacBol(int degreee, int pot, int timeOff) {
  long lastUltraBolCheck = 0;
  byte encVale = digitalRead(encEsq1);
  byte encVale2 = digitalRead(encDir1);
  int contPasqe = 0, contPasqe2 = 0;
  if (degreee > 0) {
    while (contPasqe < degreee) {
      if(contPasqe%20 > 10){
        servoesq1.write(175);//grau mais baixo, garra mais alta
        servodir1.write(7);//grau mais pra baixo , garra mais pra baixo                     em baixo segurando bolinha : esq1 170  ;   dir1  20    ;   esq2  180  ;  dir2   0;
        servoesq2.write(135);//maior grau fecha  
        servodir2.write(65);//menor grau fecha
      }
      else{
        servoesq1.write(175);//grau mais baixo, garra mais alta
        servodir1.write(7);//grau mais pra baixo , garra mais pra baixo                     em baixo segurando bolinha : esq1 170  ;   dir1  20    ;   esq2  180  ;  dir2   0;
        servoesq2.write(130);//maior grau fecha  
        servodir2.write(70);//menor grau fecha
      }
      if (digitalRead(encEsq1) != encVale) {
        encVale = digitalRead(encEsq1);
        contPasqe++;
      }
      if (digitalRead(encDir1) != encVale2) {
        encVale2 = digitalRead(encDir1);
        contPasqe2++;
      }
      md.setM1Speed(pot*1);
      md.setM2Speed(pot);
      if(millis() - lastUltraBolCheck > 10){
        lastUltraBolCheck = millis();
        if(getUltra(trigFre, echoFre, 8) <= 5 ){
          int contGarraDebug = 0;
          breakMotors();
          servoesq1.write(175);//grau mais baixo, garra mais alta
          servodir1.write(8);//grau mais pra baixo , garra mais pra baixo                     em baixo segurando bolinha : esq1 170  ;   dir1  20    ;   esq2  180  ;  dir2   0;
          for(contGarraDebug = 70; contGarraDebug >= 10; contGarraDebug--){
          servoesq2.write(185 - contGarraDebug);//maior grau fecha  
          servodir2.write(contGarraDebug);//menor grau fecha 
          delay(10);
          }
          servoesq1.write(10);//grau mais baixo, garra mais alta
          servodir1.write(172);//grau mais pra baixo , garra mais pra baixo                     em baixo segurando bolinha : esq1 170  ;   dir1  20    ;   esq2  180  ;  dir2   0;
          servoesq2.write(175);//maior grau fecha  
          servodir2.write(10);//menor grau fecha   
          delay(2000);
          servoesq1.write(10);//grau mais baixo, garra mais alta
          servodir1.write(172);//grau mais pra baixo , garra mais pra baixo                     em baixo segurando bolinha : esq1 170  ;   dir1  20    ;   esq2  180  ;  dir2   0;
          servoesq2.write(90);//maior grau fecha  
          servodir2.write(90);//menor grau fecha 
          delay(1500);
          servoesq1.write(175);//grau mais baixo, garra mais alta
          servodir1.write(7);//grau mais pra baixo , garra mais pra baixo                     em baixo segurando bolinha : esq1 170  ;   dir1  20    ;   esq2  180  ;  dir2   0;
          servoesq2.write(130);//maior grau fecha  
          servodir2.write(70);//menor grau fecha
          delay(2000);
        }
      }
    }
  }
  else {
    while (contPasqe < -degreee) {
      if (digitalRead(encEsq1) != encVale) {
        encVale = digitalRead(encEsq1);
        contPasqe++;
        //Serial.println(contPasq);
      }
      md.setM1Speed(pot*0.70);
      md.setM2Speed(pot);
    }
  }
  breakMotors();
  delay(timeOff);
}


void andarCM(int cm, int pot, int timeOff){
  if(cm > 0){
    moveForBac(20*cm, pot, timeOff);
  }
  else{
    moveForBac(-20*cm, pot, timeOff);
  }
}

void andarCMEsp(int cm, int pot, int timeOff){
  if(cm > 0){
    moveForBacEsp(20*cm, pot, timeOff);
  }
  else{
    moveForBacEsp(20*cm, pot, timeOff);
  }
}

void andarCMBol(int cm, int pot, int timeOff){
  if(cm > 0){
    moveForBacBol(20*cm, pot, timeOff);
  }
  else{
    moveForBacBol(-20*cm, pot, timeOff);
  }
}

void andarCMBolEsp(int cm, int pot, int timeOff){
  if(cm > 0){
    moveForBacBolEsp(20*cm, pot, timeOff);
  }
  else{
    moveForBacBolEsp(-20*cm, pot, timeOff);
  }
}
void turnRobot(int degree, int pot, int timeOff) {
  byte encVal = digitalRead(encEsq1);
  byte encVal2 = digitalRead(encDir1);
  int contPasq = 0, contPasq2 = 0;
  if (degree > 0) {
    while (contPasq < degree) {
      if (digitalRead(encEsq1) != encVal) {
        encVal = digitalRead(encEsq1);
        contPasq++;
      }
      md.setM1Speed(-pot*0.95);
      md.setM2Speed(pot);
    }
  }
  else {
    while (contPasq < -degree) {
      if (digitalRead(encEsq1) != encVal) {
        encVal = digitalRead(encEsq1);
        contPasq++;
      }
      md.setM1Speed(pot*0.95);
      md.setM2Speed(-pot);
    }
  }
  breakMotors();
  delay(timeOff);
}

void breakMotors() {
  int encVal = 0;
  int encVal2 = 0;
  long timeElap = millis();
  bool encState1 = digitalRead(encEsq1);
  bool encState2 = digitalRead(encEsq2);
  bool encState3 = digitalRead(encDir1);
  bool encState4 = digitalRead(encDir2);
  bool lastEncState1 = digitalRead(encEsq1);
  bool lastEncState2 = digitalRead(encEsq2);
  bool lastEncState3 = digitalRead(encDir1);
  bool lastEncState4 = digitalRead(encDir2);
  while (millis() - timeElap < 20) {
    encState1 = digitalRead(encEsq1);
    encState2 = digitalRead(encEsq2);
    encState3 = digitalRead(encDir1);
    encState4 = digitalRead(encDir2);
    if (encState1 == true && (encState2 == true && (lastEncState1 == false && lastEncState2 == true))) {
      encVal++;
      lastEncState1 = encState1;
      lastEncState2 = encState2;
      //Serial.println(encVal);
    }

    else if (encState1 == false && (encState2 == true && (lastEncState1 == false && lastEncState2 == false))) {
      encVal++;
      lastEncState1 = encState1;
      lastEncState2 = encState2;
      //Serial.println(encVal);
    }

    else if (encState1 == false && (encState2 == false && (lastEncState1 == true && lastEncState2 == false))) {
      encVal++;
      lastEncState1 = encState1;
      lastEncState2 = encState2;
      //Serial.println(encVal);
    }

    else if (encState1 == true && (encState2 == false && (lastEncState1 == true && lastEncState2 == true))) {
      encVal++;
      lastEncState1 = encState1;
      lastEncState2 = encState2;
      //Serial.println(encVal);
    }

    else if (encState1 == true && (encState2 == true && (lastEncState1 == true && lastEncState2 == false))) {
      encVal--;
      lastEncState1 = encState1;
      lastEncState2 = encState2;
      //Serial.println(encVal);
    }

    else if (encState1 == true && (encState2 == false && (lastEncState1 == false && lastEncState2 == false))) {
      encVal--;
      lastEncState1 = encState1;
      lastEncState2 = encState2;
      //Serial.println(encVal);
    }

    else if (encState1 == false && (encState2 == false && (lastEncState1 == false && lastEncState2 == true))) {
      encVal--;
      lastEncState1 = encState1;
      lastEncState2 = encState2;
      // Serial.println(encVal);
    }

    else if (encState1 == false && (encState2 == true && (lastEncState1 == true && lastEncState2 == true))) {
      encVal--;
      lastEncState1 = encState1;
      lastEncState2 = encState2;
      //Serial.println(encVal);
    }

    if (encState3 == true && (encState4 == true && (lastEncState3 == false && lastEncState4 == true))) {
      encVal2++;
      lastEncState3 = encState3;
      lastEncState4 = encState4;
      //Serial.println(encVal);
    }

    else if (encState3 == false && (encState4 == true && (lastEncState3 == false && lastEncState4 == false))) {
      encVal2++;
      lastEncState3 = encState3;
      lastEncState4 = encState4;
      //Serial.println(encVal);
    }

    else if (encState3 == false && (encState4 == false && (lastEncState3 == true && lastEncState4 == false))) {
      encVal2++;
      lastEncState3 = encState3;
      lastEncState4 = encState4;
      //Serial.println(encVal);
    }

    else if (encState3 == true && (encState4 == false && (lastEncState3 == true && lastEncState4 == true))) {
      encVal2++;
      lastEncState3 = encState3;
      lastEncState4 = encState4;
      //Serial.println(encVal);
    }

    else if (encState3 == true && (encState4 == true && (lastEncState3 == true && lastEncState4 == false))) {
      encVal2--;
      lastEncState3 = encState3;
      lastEncState4 = encState4;
      //Serial.println(encVal);
    }

    else if (encState3 == true && (encState4 == false && (lastEncState3 == false && lastEncState4 == false))) {
      encVal2--;
      lastEncState3 = encState3;
      lastEncState4 = encState4;
      //Serial.println(encVal);
    }

    else if (encState3 == false && (encState4 == false && (lastEncState3 == false && lastEncState4 == true))) {
      encVal2--;
      lastEncState3 = encState3;
      lastEncState4 = encState4;
      // Serial.println(encVal);
    }

    else if (encState3 == false && (encState4 == true && (lastEncState3 == true && lastEncState4 == true))) {
      encVal2--;
      lastEncState3 = encState3;
      lastEncState4 = encState4;
      //Serial.println(encVal);
    }

  }
  Serial.print("val encs: ");
  Serial.println(encVal2);
  Serial.println(encVal);
  if (encVal2 > 0) {
    if (encVal > 0) {
      md.setM1Speed(0.95*(-encVal * 8));
      md.setM2Speed(-encVal * 8);
      delay(70);
    }
    else {
      md.setM1Speed(0.95*(-encVal2 * 4));
      md.setM2Speed(encVal * 4);
      delay(70);
    }
  }
  else {
    if (encVal > 0) {
      md.setM1Speed(0.95*(-encVal2) * 4);
      md.setM2Speed(-encVal * 4);
      delay(70);
    }
    else {
      Serial.println("cheguei");
      md.setM1Speed(0.95 * (-encVal2) * 8);
      md.setM2Speed(-encVal * 8);
      delay(70);
    }
  }
  md.setM1Speed(0);
  md.setM2Speed(0);
  delay(200);
  digitalWrite(buz, HIGH);
  delay(100);
  digitalWrite(buz, LOW);
  delay(100);
}

void breakMotorsSoft() {
  int encVal = 0;
  int encVal2 = 0;
  long timeElap = millis();
  bool encState1 = digitalRead(encEsq1);
  bool encState2 = digitalRead(encEsq2);
  bool encState3 = digitalRead(encDir1);
  bool encState4 = digitalRead(encDir2);
  bool lastEncState1 = digitalRead(encEsq1);
  bool lastEncState2 = digitalRead(encEsq2);
  bool lastEncState3 = digitalRead(encDir1);
  bool lastEncState4 = digitalRead(encDir2);
  while (millis() - timeElap < 20) {
    encState1 = digitalRead(encEsq1);
    encState2 = digitalRead(encEsq2);
    encState3 = digitalRead(encDir1);
    encState4 = digitalRead(encDir2);
    if (encState1 == true && (encState2 == true && (lastEncState1 == false && lastEncState2 == true))) {
      encVal++;
      lastEncState1 = encState1;
      lastEncState2 = encState2;
      //Serial.println(encVal);
    }

    else if (encState1 == false && (encState2 == true && (lastEncState1 == false && lastEncState2 == false))) {
      encVal++;
      lastEncState1 = encState1;
      lastEncState2 = encState2;
      //Serial.println(encVal);
    }

    else if (encState1 == false && (encState2 == false && (lastEncState1 == true && lastEncState2 == false))) {
      encVal++;
      lastEncState1 = encState1;
      lastEncState2 = encState2;
      //Serial.println(encVal);
    }

    else if (encState1 == true && (encState2 == false && (lastEncState1 == true && lastEncState2 == true))) {
      encVal++;
      lastEncState1 = encState1;
      lastEncState2 = encState2;
      //Serial.println(encVal);
    }

    else if (encState1 == true && (encState2 == true && (lastEncState1 == true && lastEncState2 == false))) {
      encVal--;
      lastEncState1 = encState1;
      lastEncState2 = encState2;
      //Serial.println(encVal);
    }

    else if (encState1 == true && (encState2 == false && (lastEncState1 == false && lastEncState2 == false))) {
      encVal--;
      lastEncState1 = encState1;
      lastEncState2 = encState2;
      //Serial.println(encVal);
    }

    else if (encState1 == false && (encState2 == false && (lastEncState1 == false && lastEncState2 == true))) {
      encVal--;
      lastEncState1 = encState1;
      lastEncState2 = encState2;
      // Serial.println(encVal);
    }

    else if (encState1 == false && (encState2 == true && (lastEncState1 == true && lastEncState2 == true))) {
      encVal--;
      lastEncState1 = encState1;
      lastEncState2 = encState2;
      //Serial.println(encVal);
    }

    if (encState3 == true && (encState4 == true && (lastEncState3 == false && lastEncState4 == true))) {
      encVal2++;
      lastEncState3 = encState3;
      lastEncState4 = encState4;
      //Serial.println(encVal);
    }

    else if (encState3 == false && (encState4 == true && (lastEncState3 == false && lastEncState4 == false))) {
      encVal2++;
      lastEncState3 = encState3;
      lastEncState4 = encState4;
      //Serial.println(encVal);
    }

    else if (encState3 == false && (encState4 == false && (lastEncState3 == true && lastEncState4 == false))) {
      encVal2++;
      lastEncState3 = encState3;
      lastEncState4 = encState4;
      //Serial.println(encVal);
    }

    else if (encState3 == true && (encState4 == false && (lastEncState3 == true && lastEncState4 == true))) {
      encVal2++;
      lastEncState3 = encState3;
      lastEncState4 = encState4;
      //Serial.println(encVal);
    }

    else if (encState3 == true && (encState4 == true && (lastEncState3 == true && lastEncState4 == false))) {
      encVal2--;
      lastEncState3 = encState3;
      lastEncState4 = encState4;
      //Serial.println(encVal);
    }

    else if (encState3 == true && (encState4 == false && (lastEncState3 == false && lastEncState4 == false))) {
      encVal2--;
      lastEncState3 = encState3;
      lastEncState4 = encState4;
      //Serial.println(encVal);
    }

    else if (encState3 == false && (encState4 == false && (lastEncState3 == false && lastEncState4 == true))) {
      encVal2--;
      lastEncState3 = encState3;
      lastEncState4 = encState4;
      // Serial.println(encVal);
    }

    else if (encState3 == false && (encState4 == true && (lastEncState3 == true && lastEncState4 == true))) {
      encVal2--;
      lastEncState3 = encState3;
      lastEncState4 = encState4;
      //Serial.println(encVal);
    }

  }
  Serial.print("val encs: ");
  Serial.println(encVal2);
  Serial.println(encVal);
  if (encVal2 > 0) {
    if (encVal > 0) {
      md.setM1Speed(0.95*(-encVal * 4));
      md.setM2Speed(-encVal * 4);
      delay(70);
    }
    else {
      md.setM1Speed(0.95*(-encVal2 * 2));
      md.setM2Speed(encVal * 2);
      delay(70);
    }
  }
  else {
    if (encVal > 0) {
      md.setM1Speed(0.95*(-encVal2) * 2);
      md.setM2Speed(-encVal * 4);
      delay(70);
    }
    else {
      Serial.println("cheguei");
      md.setM1Speed(0.95*(-encVal2) * 4);
      md.setM2Speed(-encVal * 4);
      delay(70);
    }
  }
  md.setM1Speed(0);
  md.setM2Speed(0);
}

void contornoDir(){
  breakMotors();
  int distGhost = 0;
  moveForBac(-100,-150,500);
  while(distGhost < 10){
    distGhost = getUltra(trigFre, echoFre, 100);
    md.setM1Speed(-150);
    md.setM2Speed(150);
    if(distGhost >= 10){
      breakMotors();
      distGhost = getUltra(trigFre, echoFre, 100);
    }
   }
   breakMotors();
   turnRobot(155,150,500);
   moveForBac(100,100,500);
   digitalWrite(buz, HIGH);
   delay(100);
   digitalWrite(buz, LOW);
   int contParmaContObst = 0;
   bool encoderContObst = digitalRead(encEsq1);
   while(contParmaContObst < 700){
    md.setM1Speed(145);
    md.setM2Speed(78);
    if(digitalRead(encEsq1) != encoderContObst){
      encoderContObst = digitalRead(encEsq1);
      contParmaContObst++;
    }
   }
   breakMotors();
   while(analogRead(corD) < brancoD + 200 && analogRead(corE) < brancoE + 200){
     md.setM1Speed(145);
     md.setM2Speed(78);
   }
   breakMotors();
   contParmaContObst = 0;
   encoderContObst = digitalRead(encEsq1);
   while(contParmaContObst < 300){
     if(analogRead(corD) < brancoD + 50){
       md.setM1Speed(200);
       md.setM2Speed(0);
     }
     else{
       md.setM1Speed(-200);
       md.setM2Speed(200);
     }
     if(digitalRead(encEsq1) != encoderContObst){
      encoderContObst = digitalRead(encEsq1);
      contParmaContObst++;
    }
   }
   breakMotors();
}
void contornoEsq(){
  breakMotors();
  int distGhost = 0;
  moveForBac(-100,-150,500);
  while(distGhost < 10){
    distGhost = getUltra(trigFre, echoFre, 100);
    md.setM1Speed(150);
    md.setM2Speed(-150);
    if(distGhost >= 10){
      breakMotors();
      distGhost = getUltra(trigFre, echoFre, 100);
    }
   }
   breakMotors();
   turnRobot(-170,150,500);
   moveForBac(100,150,500);
   digitalWrite(buz, HIGH);
   delay(100);
   digitalWrite(buz, LOW);
   int contParmaContObst = 0;
   bool encoderContObst = digitalRead(encEsq1);
   while(contParmaContObst < 700){
    md.setM1Speed(83);
    md.setM2Speed(215);
    if(digitalRead(encDir1) != encoderContObst){
      encoderContObst = digitalRead(encDir1);
      contParmaContObst++;
    }
   }
   breakMotors();
   while(analogRead(corD) < brancoD + 200 && analogRead(corE) < brancoE + 200){
     md.setM1Speed(78);
     md.setM2Speed(215);
   }
   breakMotors();
   contParmaContObst = 0;
   encoderContObst = digitalRead(encDir1);
   while(contParmaContObst < 200){
     if(analogRead(corE) < brancoD + 50){
       md.setM1Speed(0);
       md.setM2Speed(200);
     }
     else{
       md.setM1Speed(200);
       md.setM2Speed(-200);
     }
     if(digitalRead(encDir1) != encoderContObst){
      encoderContObst = digitalRead(encDir1);
      contParmaContObst++;
    }
   }
   breakMotors();
}
/*****************************************************************************************************************************/
void setup() {   //direita eh m1
  pinMode(redLeft, OUTPUT);       pinMode(greenLeft, OUTPUT);     pinMode(blueLeft, OUTPUT);
  pinMode(redRight, OUTPUT);      pinMode(greenRight, OUTPUT);    pinMode(blueRight, OUTPUT);
  pinMode(encEsq1, INPUT);        pinMode(encEsq2, INPUT);
  pinMode(encDir1, INPUT);        pinMode(encDir2, INPUT);
  pinMode(trigFre, OUTPUT);       pinMode(echoFre, INPUT);
  pinMode(trigEsq, OUTPUT);       pinMode(echoEsq, INPUT);
  pinMode(trigDir, OUTPUT);       pinMode(echoDir, INPUT);
  pinMode(trigWall, OUTPUT);       pinMode(echoWall, INPUT);
  pinMode(buz, OUTPUT);
  cacamba.attach(28);
  servoesq1.attach(16);
  servodir1.attach(17);
  servoesq2.attach(14);  
  servodir2.attach(15);
  cacamba.write(77);
  servoesq1.write(10);//grau mais baixo, garra mais alta
  servodir1.write(163);//grau mais pra baixo , garra mais pra baixo                     em baixo segurando bolinha : esq1 170  ;   dir1  20    ;   esq2  180  ;  dir2   0;
  servoesq2.write(175);//maior grau fecha  
  servodir2.write(15);//menor grau fecha   
  Serial.begin(115200);
  display.begin();
  display.setRotation(2);
  display.setContrast(18);
  Wire.begin();
  delay(100);
  // enable to measute g data
  //andarCMEsp(70, 150, 500);
  //digitalWrite(5, HIGH);
  Wire.beginTransmission(ADXAddress);
  Wire.write(Register_2D);
  Wire.write(8);                //measuring enable
  Wire.endTransmission();     // stop transmitting
}
bool condition = false;
void taskSensors() {
  absDir = map(analogRead(corD), brancoD, pretoD, 0, 200);
  absEsq = map(analogRead(corE), brancoE, pretoE, 0, 200);
  absDir = constrain(absDir, 0, 200);
  absEsq = constrain(absEsq, 0, 200);
  absDir2 = map(analogRead(corD2), brancoD2, pretoD2, 0, 200);
  absEsq2 = map(analogRead(corE2), brancoE2, pretoE2, 0, 200);
  absDir2 = constrain(absDir2, 0, 200);
  absEsq2 = constrain(absEsq2, 0, 200);
  erro = absDir - absEsq;
  if(condition == true){
    potM1 = constrain(0.80*(pot - (kp * (erro) + kd * (erro - lasterro))), -400, 400);
    potM2 = constrain(pot + (kp * (erro) + kd * (erro - lasterro)), -400, 400);
  }
  else{
    potM1 = constrain(0.80*(pot - (kp * (erro) + kd * (erro - lasterro))), -250, 250);
    potM2 = constrain(pot + (kp * (erro) + kd * (erro - lasterro)), -250, 250);
  }
  //exibir("direita: ", analogRead(corD2));
  //exibir("esquerda: ", analogRead(corE2));
}

void taskFollow(){
  md.setM1Speed(potM1);
  md.setM2Speed(potM2);
  /*exibir("pot M2: ", potM2);
  exibir("pot M1: ", potM1);
  exibir("rampa?: ", condition);
  exibir("x: ", Xg);*/
  lasterro = erro;
  //exibir("verdeE: ", analogRead(verdeE)); 
}

void taskThird(){
  int ale = 0;
  int stateUltimo = 0;
  breakMotors();
  andarCM(13, 130, 500);
  cacamba.write(77);
  servoesq1.write(175);//grau mais baixo, garra mais alta
  servodir1.write(7);//grau mais pra baixo , garra mais pra baixo                     em baixo segurando bolinha : esq1 170  ;   dir1  20    ;   esq2  180  ;  dir2   0;
  servoesq2.write(130);//maior grau fecha  
  servodir2.write(70);//menor grau fecha
  turnRobot(-50, 130, 100);
  andarCMBol(38, 130, 500);
  ale = 0;
  while(getUltra(trigWall, echoWall, 100) > 48){
    md.setM1Speed(150);
    md.setM2Speed(150);
  }
  breakMotors();
  analogWrite(buz, 200);
  delay(100);
  analogWrite(buz, 0);
  stateUltimo = digitalRead(encEsq1);
  while(ale < 400){               //pra frente ate patamar(ou nao)
    if(digitalRead(encEsq1) != stateUltimo){
       stateUltimo = digitalRead(encEsq1);
       ale++;
    }
    md.setM1Speed(150);
    md.setM2Speed(150);
    patLeft = 0;
    if(digitalRead(46) == HIGH){             //se encostar em algo, eh patamar!
      patLeft = 1;
      ale = 0;
      while(ale < 350){                            
        if(digitalRead(encEsq1) != stateUltimo){
          stateUltimo = digitalRead(encEsq1);
          ale++;
        }
        md.setM1Speed(0);
        md.setM2Speed(300);
      }
      while(digitalRead(46) == LOW){          //percorre lateral do patamar ate encontrar parede
        md.setM1Speed(143);
        md.setM2Speed(150);
      }
      md.setM1Speed(0);
      md.setM2Speed(0);
      delay(100);
      ale = 0;
      stateUltimo = digitalRead(encEsq1);
      while(digitalRead(46) == HIGH){               //vira ate desencostar da parede
        if(digitalRead(encEsq1) != stateUltimo){
          stateUltimo = digitalRead(encEsq1);
        }
        md.setM1Speed(0);
        md.setM2Speed(350);
      }
      breakMotors();
      break;
    }
    if(ale == 399){
      checkRapido();
      analogWrite(buz, 200);
      delay(100);
      analogWrite(buz, 0);
      while(getUltra(trigWall, echoWall, 40) > 25){        
         md.setM1Speed(150);
         md.setM2Speed(150);
         delay(100);
        }
        breakMotors();
        for(ale = 130; ale < 153; ale++){
          servoesq2.write(ale);//maior grau fecha
          delay(50);
        }
        for(ale = 70; ale > 35; ale--){
          servodir2.write(ale);//maior grau fecha
          delay(50);
        }
        ale = 0;
        stateUltimo = digitalRead(encEsq1);
        while(ale < 650){                            //fim da parede. Vira 90 graus para percorrer outra parede
          if(digitalRead(encEsq1) != stateUltimo){
            stateUltimo = digitalRead(encEsq1);
            ale++;
          }
          md.setM1Speed(0);
          md.setM2Speed(200);
        }
        breakMotors();
        for(ale = 153; ale >130; ale--){
          servoesq2.write(ale);//maior grau fecha
          delay(50);
        }
        for(ale = 35; ale > 70; ale++){
          servodir2.write(ale);//maior grau fecha
          delay(50);
        }
        andarCMBol(15, 130, 100);
        break;
    }
  }
  md.setM1Speed(0);
  md.setM2Speed(0);
  delay(100);
  ale = 0;
  andarCMBol(35,130,100);
  ale = 0;
  while(getUltra(trigWall, echoWall, 100) > 48){
    md.setM1Speed(150);
    md.setM2Speed(150);
  }
  breakMotors();
  checkRapido();
  stateUltimo = digitalRead(encEsq1);
  ale = 0;
  analogWrite(buz, 200);
  delay(100);
  analogWrite(buz, 0);
  while(ale < 400){               //pra frente ate patamar2(ou nao)
    if(digitalRead(encEsq1) != stateUltimo){
       stateUltimo = digitalRead(encEsq1);
       ale++;
    }
    md.setM1Speed(150);
    md.setM2Speed(150);
    patCenter = 0;
    
    if(digitalRead(46) == HIGH){             //se encostar em algo, eh patamar!
      patCenter = 1;
      ale = 0;
      while(ale < 350){                            
        if(digitalRead(encEsq1) != stateUltimo){
          stateUltimo = digitalRead(encEsq1);
          ale++;
        }
        md.setM1Speed(0);
        md.setM2Speed(300);
      }
      delay(100);
      while(digitalRead(46) == LOW){          //percorre lateral do patamar ate encontrar parede
        md.setM1Speed(150);
        md.setM2Speed(150);
      }
      md.setM1Speed(0);
      md.setM2Speed(0);
      delay(100);
      ale = 0;
      stateUltimo = digitalRead(encEsq1);
      while(digitalRead(46) == HIGH){               //vira ate desencostar da parede
        if(digitalRead(encEsq1) != stateUltimo){
          stateUltimo = digitalRead(encEsq1);
        }
        md.setM1Speed(0);
        md.setM2Speed(200);
      }
      breakMotors();
      break;
    }
    if(ale == 399){
      checkRapido();
      analogWrite(buz, 200);
      delay(100);
      analogWrite(buz, 0);
      while(getUltra(trigWall, echoWall, 40) > 25){        
         md.setM1Speed(150);
         md.setM2Speed(150);
         delay(100);
        }
        breakMotors();
        for(ale = 130; ale < 153; ale++){
          servoesq2.write(ale);//maior grau fecha
          delay(50);
        }
        for(ale = 70; ale > 35; ale--){
          servodir2.write(ale);//maior grau fecha
          delay(50);
        }
        ale = 0;
        stateUltimo = digitalRead(encEsq1);
        while(ale < 650){                            //fim da parede. Vira 90 graus para percorrer outra parede
          if(digitalRead(encEsq1) != stateUltimo){
            stateUltimo = digitalRead(encEsq1);
            ale++;
          }
          md.setM1Speed(0);
          md.setM2Speed(200);
        }
        breakMotors();
        for(ale = 153; ale >130; ale--){
          servoesq2.write(ale);//maior grau fecha
          delay(50);
        }
        for(ale = 35; ale > 70; ale++){
          servodir2.write(ale);//maior grau fecha
          delay(50);
        }
        andarCMBol(15, 130, 100);
        break;
    }
  }
  checkRapido();
  while(getUltra(trigWall, echoWall, 100) > 48){
    md.setM1Speed(143);
    md.setM2Speed(150);
  }
  breakMotors();
  stateUltimo = digitalRead(encEsq1);
  ale = 0;
  //andarCMBol(10,130,100);                          //percorre a outra parede
  ale = 0;
  analogWrite(buz, 200);
  delay(100);
  analogWrite(buz, 0);
  breakMotors();
  while(ale < 400){               //pra frente ate patamar3(ou nao)
    if(patCenter == true){
      goto atalho;
    }
    if(digitalRead(encEsq1) != stateUltimo){
       stateUltimo = digitalRead(encEsq1);
       ale++;
    }
    md.setM1Speed(143);
    md.setM2Speed(150);
    patCenter = 0;
    if(digitalRead(46) == HIGH){             //se encostar em algo, eh patamar!
      //digitalWrite(buz, HIGH);
      breakMotors();
      delay(2000);
      patRight = 1;
      int ale2 = 0;
      while(ale2 < 350){                            
        if(digitalRead(encEsq1) != stateUltimo){
          stateUltimo = digitalRead(encEsq1);
          ale2++;
        }
        md.setM1Speed(0);
        md.setM2Speed(300);
      }
      delay(200);
      while(digitalRead(46) == LOW){          //percorre lateral do patamar ate encontrar parede
        md.setM1Speed(143);
        md.setM2Speed(150);
      }
      md.setM1Speed(0);
      md.setM2Speed(0);
      delay(100);
      ale2 = 0;
      stateUltimo = digitalRead(encEsq1);
      while(digitalRead(46) == HIGH){               //vira ate desencostar da parede
        if(digitalRead(encEsq1) != stateUltimo){
          stateUltimo = digitalRead(encEsq1);
        }
        md.setM1Speed(0);
        md.setM2Speed(250);
      }
      breakMotors();
      andarCM(5, 150, 100);
      break;
    }
    atalho:
    if(ale == 399 || patCenter == true){
      breakMotors(); 
      analogWrite(buz, 100);
      delay(500);
      analogWrite(buz, 0);
      checkRapido();
      analogWrite(buz, 200);
      delay(100);
      analogWrite(buz, 0);
      while(getUltra(trigWall, echoWall, 40) > 25){        
         md.setM1Speed(150);
         md.setM2Speed(150);
         delay(100);
        }
        breakMotors();
        checkRapido();
        for(ale = 130; ale < 153; ale++){
          servoesq2.write(ale);//maior grau fecha
          delay(50);
        }
        for(ale = 70; ale > 35; ale--){
          servodir2.write(ale);//maior grau fecha
          delay(50);
        }
        ale = 0;
        stateUltimo = digitalRead(encEsq1);
        while(ale < 700){                            //fim da parede. Vira 90 graus para percorrer outra parede
          if(digitalRead(encEsq1) != stateUltimo){
            stateUltimo = digitalRead(encEsq1);
            ale++;
          }
          md.setM1Speed(0);
          md.setM2Speed(200);
        }
        breakMotors();
        for(ale = 153; ale >130; ale--){
          servoesq2.write(ale);//maior grau fecha
          delay(50);
        }
        for(ale = 35; ale > 70; ale++){
          servodir2.write(ale);//maior grau fecha
          delay(50);
        }
        andarCMBol(15, 130, 100);
        break;
    }
  }
  breakMotors();
  checkRapido();
  analogWrite(buz, 200);
  delay(100);
  analogWrite(buz, 0);
  delay(100);
  analogWrite(buz, 200);
  delay(100);
  analogWrite(buz, 0);
  delay(100);
  analogWrite(buz, 200);
  delay(100);
  analogWrite(buz, 0);
  andarCMBol(35,130,100);                                             //percorre a ultima parede
  md.setM1Speed(0);
  md.setM2Speed(0);
  delay(100);
  ale = 0;
  for(ale = 0; ale < 28; ale++){
    servoesq1.write(175 - ale);//grau mais baixo, garra mais alta
    servodir1.write(8 + ale);//grau mais pra baixo , garra mais pra baixo
    delay(15);
  }
  stateUltimo = digitalRead(encEsq1);                                //vira 90 graus
  while(ale < 700){
    if(digitalRead(encEsq1) != stateUltimo){
      stateUltimo = digitalRead(encEsq1);
      ale++;
    }
    md.setM1Speed(0);
    md.setM2Speed(200);
  }
  breakMotors();
  for(ale = 0; ale < 28; ale++){
    servoesq1.write(147 + ale);//grau mais baixo, garra mais alta
    servodir1.write(126 - ale);//grau mais pra baixo , garra mais pra baixo
    delay(15);
  }
  md.setM1Speed(-300);                                               //re para emparelhar c/ parede
  md.setM2Speed(-300);
  delay(2000);
  if(patLeft == true){
    md.setM1Speed(0);
    md.setM2Speed(0);
    analogWrite(buz, 200);
    delay(100);
    analogWrite(buz, 0);
  }
  else if(patCenter == true){
    md.setM1Speed(0);
    md.setM2Speed(0);
    analogWrite(buz, 200);
    delay(100);
    analogWrite(buz, 0);
    delay(100);
    analogWrite(buz, 200);
    delay(100);
    analogWrite(buz, 0);
  }
  else{
    md.setM1Speed(0);
    md.setM2Speed(0);
    analogWrite(buz, 200);
    delay(100);
    analogWrite(buz, 0);
    delay(100);
    analogWrite(buz, 200);
    delay(100);
    analogWrite(buz, 0);
    delay(100);
    analogWrite(buz, 200);
    delay(100);
    analogWrite(buz, 0);
  }
  servoesq2.write(110);//maior grau fecha  
  servodir2.write(70);//menor grau fecha
  md.setM1Speed(80);
  md.setM2Speed(100);
  delay(1000);
  md.setM1Speed(-100);
  md.setM2Speed(-100);
  delay(2000);
  andarCMBolEsp(40, 150, 200);                                       //reto
  ale = 0;             
  while(ale < 480){                                                       //primeira virada
    if(digitalRead(encEsq1) != stateUltimo){
      stateUltimo = digitalRead(encEsq1);
      ale++;
    }
    md.setM1Speed(0);
    md.setM2Speed(130);
  }
  breakMotors();
  andarCMBolEsp(25, 150, 200);
  ale = 0;
  while(ale < 480){                                              //segunda virada
    if(digitalRead(encEsq1) != stateUltimo){
      stateUltimo = digitalRead(encEsq1);
      ale++;
    }
    md.setM1Speed(0);
    md.setM2Speed(130);
  }
  breakMotors();
  for(ale = 0; ale < 68; ale++){
    servoesq1.write(175 - ale);//grau mais baixo, garra mais alta
    servodir1.write(8 + ale);//grau mais pra baixo , garra mais pra baixo
    delay(15);
  }
  andarCMEsp(-5, -150, 100);
  for(ale = 0; ale < 68; ale++){
    servoesq1.write(107 + ale);//grau mais baixo, garra mais alta
    servodir1.write(76 - ale);//grau mais pra baixo , garra mais pra baixo
    delay(15);
  }
  andarCMBolEsp(18, 130, 100);
  ale = 0;
  while(ale < 480){                                              //terceira virada
    if(digitalRead(encEsq1) != stateUltimo){
      stateUltimo = digitalRead(encEsq1);
      ale++;
    }
    md.setM1Speed(0);
    md.setM2Speed(130);
  }
  breakMotors();
  for(ale = 0; ale < 68; ale++){
    servoesq1.write(175 - ale);//grau mais baixo, garra mais alta
    servodir1.write(8 + ale);//grau mais pra baixo , garra mais pra baixo
    delay(15);
  }
  andarCMEsp(-5, -150, 100);
  for(ale = 0; ale < 68; ale++){
    servoesq1.write(107 + ale);//grau mais baixo, garra mais alta
    servodir1.write(76 - ale);//grau mais pra baixo , garra mais pra baixo
    delay(15);
  }
  andarCMBolEsp(50, 150, 100);
  breakMotors();
  ale = 0;
  while(ale < 930){
    if(digitalRead(encEsq1) != stateUltimo){
      stateUltimo = digitalRead(encEsq1);
      ale++;
    }
    md.setM1Speed(0);
    md.setM2Speed(200);
  }
  md.setM1Speed(-300);
  md.setM2Speed(-300);
  delay(3000);
  
  if(patLeft == true){
    andarCMEsp(20,130,100);
    ale=0;
    while(ale < 480){
      if(digitalRead(encDir1) != stateUltimo){
        stateUltimo = digitalRead(encDir1);
        ale++;
      }
      md.setM1Speed(200);
      md.setM2Speed(0);
    }
    while(getUltra(trigWall, echoWall, 30) > 20){
      md.setM1Speed(200*0.80);
      md.setM2Speed(200);
    }
    breakMotors();
    ale = 0;
    while(ale < 720){
      if(digitalRead(encEsq1) != stateUltimo){
        stateUltimo = digitalRead(encEsq1);
        ale++;
      }
      md.setM1Speed(-200);
      md.setM2Speed(-200);
    }
    ale=0;
    while(ale < 700){
      if(digitalRead(encEsq1) != stateUltimo){
        stateUltimo = digitalRead(encEsq1);
        ale++;
      }
      md.setM1Speed(0);
      md.setM2Speed(200);
    }
    md.setM1Speed(-200);
    md.setM2Speed(-200);
    delay(3000);
    cacamba.write(130);
    delay(1000);
    md.setM1Speed(200);
    md.setM2Speed(200);
    delay(300);
    md.setM1Speed(-200);
    md.setM2Speed(-200);
    delay(300);
    delay(200000);
  }
  else if(patCenter == true){
    andarCMEsp(60, 150, 100);
    ale=0;
    while(ale < 480){
      if(digitalRead(encEsq1) != stateUltimo){
        stateUltimo = digitalRead(encEsq1);
        ale++;
      }
      md.setM1Speed(0);
      md.setM2Speed(-150);
    }
    md.setM1Speed(-200);
    md.setM2Speed(-200);
    delay(3000);
    andarCMEsp(25, 150, 500);
    
    ale=0;
    while(ale < 480){
      if(digitalRead(encEsq1) != stateUltimo){
        stateUltimo = digitalRead(encEsq1);
        ale++;
      }
      md.setM1Speed(0);
      md.setM2Speed(150);
    }
    while(getUltra(trigWall, echoWall, 30) > 20){
      md.setM1Speed(200*0.85);
      md.setM2Speed(200);
    }
    andarCMEsp(-13, -150, 100);
    ale = 0;
    while(ale < 720){
      if(digitalRead(encDir1) != stateUltimo){
        stateUltimo = digitalRead(encDir1);
        ale++;
      }
      md.setM1Speed(-200);
      md.setM2Speed(0);
    }
    md.setM1Speed(-200);
    md.setM2Speed(-200);
    delay(3000);
    cacamba.write(130);
    md.setM1Speed(0);
    md.setM2Speed(0);
    delay(1000);
    md.setM1Speed(200);
    md.setM2Speed(200);
    delay(300);
    md.setM1Speed(-200);
    md.setM2Speed(-200);
    delay(300);
    delay(200000);
  }
  else{
    andarCMBolEsp(60, 150, 100);
    ale=0;
    while(ale < 480){
      if(digitalRead(encEsq1) != stateUltimo){
        stateUltimo = digitalRead(encEsq1);
        ale++;
      }
      md.setM1Speed(0);
      md.setM2Speed(-150);
    }
    md.setM1Speed(-200);
    md.setM2Speed(-200);
    delay(3000);
    andarCMBolEsp(25, 150, 100);
    ale=0;
    while(ale < 480){
      if(digitalRead(encEsq1) != stateUltimo){
        stateUltimo = digitalRead(encEsq1);
        ale++;
      }
      md.setM1Speed(0);
      md.setM2Speed(150);
    }
    while(getUltra(trigWall, echoWall, 30) > 20){
      md.setM1Speed(200*0.85);
      md.setM2Speed(200);
    }
    andarCMEsp(-13, -150, 100);
    ale = 0;
    while(ale < 720){
      if(digitalRead(encEsq1) != stateUltimo){
        stateUltimo = digitalRead(encEsq1);
        ale++;
      }
      md.setM1Speed(0);
      md.setM2Speed(-200);
    }
    md.setM1Speed(-200);
    md.setM2Speed(-200);
    delay(3000);
    cacamba.write(130);
    md.setM1Speed(0);
    md.setM2Speed(0);
    delay(1000);
    md.setM1Speed(200);
    md.setM2Speed(200);
    delay(300);
    md.setM1Speed(-200);
    md.setM2Speed(-200);
    delay(300);
    delay(200000);
  }
}

void taskObst() {
  breakMotors();
  if(prefEsq == true){
    if(getUltra(trigEsq, echoEsq, 100) < 14){
      contornoDir();
    }
    else{
      contornoEsq();
    }
  }
  else{
    if(getUltra(trigDir, echoDir, 100) < 14){
      contornoEsq();
    }
    else{
      contornoDir();
    }
  }
}

void taskVerde(){
  offDir = analogRead(verdeD);
  offEsq = analogRead(verdeE);
  delayMicroseconds(500);
  digitalWrite(greenLeft, HIGH);
  digitalWrite(redLeft, HIGH);
  digitalWrite(greenRight, HIGH);
  digitalWrite(redRight, HIGH);
  onDir = analogRead(verdeD);
  onEsq = analogRead(verdeE);
  delayMicroseconds(500);
  if(onDir < offDir){
    offDir = onDir;
  }
  if(onEsq < offEsq){
    offEsq = onEsq;
  }
  digitalWrite(greenLeft, HIGH);
  digitalWrite(redLeft, LOW);
  digitalWrite(greenRight, HIGH);
  digitalWrite(redRight, LOW);
  delayMicroseconds(1000);
  greenValEsq = analogRead(verdeE);
  greenValDir = analogRead(verdeD);
  delayMicroseconds(1000);
  digitalWrite(greenLeft, LOW);
  digitalWrite(redLeft, HIGH);
  digitalWrite(greenRight, LOW);
  digitalWrite(redRight, HIGH);
  delayMicroseconds(1000);
  blueValEsq = analogRead(verdeE);
  blueValDir = analogRead(verdeD);
  delayMicroseconds(1000);
  digitalWrite(redLeft, LOW);
  digitalWrite(redRight, LOW);
  //exibir("esquerda",(greenValEsq-blueValEsq));
  if(((greenValEsq-blueValEsq) > verdeEsq) && (erro > -150 && erro < 150) && (analogRead(corD) < brancoD + 150 && analogRead(corE) < brancoE + 150) && analogRead(corC) > 450){
    breakMotors();
    andarCMEsp(0.5,150,100);
    int contParmaContObst = 0;
     bool encoderContObst = digitalRead(encDir1);
     while(contParmaContObst < 600){
       if(analogRead(corE) < brancoE + 50){
         md.setM1Speed(0);
         md.setM2Speed(190);
       }
       else{
         md.setM1Speed(190);
         md.setM2Speed(-190);
       }
       if(digitalRead(encDir1) != encoderContObst){
        encoderContObst = digitalRead(encDir1);
        contParmaContObst++;
      }
     }
     lastMenor = millis();
     timeMenor = 1000;
  }
  else{
    //exibir("direita",(greenValDir-blueValDir));
    //exibir("corC", (analogRead(corC)));
    //exibir("corrente", md.getM1CurrentMilliamps());
    if(((greenValDir-blueValDir) > verdeDir) && (erro > -150 && erro < 150) && (analogRead(corD) < brancoD + 150 && analogRead(corE) < brancoE + 150) && analogRead(corC) > 450){
     breakMotors();
     andarCMEsp(0.5,150,100);
     int contParmaContObst = 0;
     bool encoderContObst = digitalRead(encEsq1);
     while(contParmaContObst < 600){
       if(analogRead(corD) < brancoD + 50){
         md.setM1Speed(190);
         md.setM2Speed(0);
       }
       else{
         md.setM1Speed(-190);
         md.setM2Speed(190);
       }
       if(digitalRead(encEsq1) != encoderContObst){
        encoderContObst = digitalRead(encEsq1);
        contParmaContObst++;
      }
     }
     lastMenor = millis();
      timeMenor = 1000;
    }
  }
}

void taskDebug() {
  display.clearDisplay();
  display.setCursor(0, 1);
  display.print("absDir = ");
  display.print(absDir);
  display.setCursor(0, 9);
  display.print("absEsq = ");
  display.print(absEsq);
  display.setCursor(0, 17);
  display.print("rawEsq = ");
  display.print(analogRead(corE));
  display.setCursor(0, 25);
  display.print("rawDir = ");
  display.print(analogRead(corD));
  display.setCursor(0, 33);
  display.print("potDir = ");
  display.print(int(pot - (kp * (erro) + kd * (erro - lasterro))));
  display.display();
}
long startCounter = 0;
int subCondition = 0;
int contDescida = 0;
long timeDesc = 0;
bool onlyDesc = false;
void taskRampa(){
  if (contDescida < 0) contDescida = 0;
  if(analogRead(A15) < 500 && onlyDesc == false && erro > -50 && erro < 50){  //SUBIDA
  condition = true;
  subCondition = false;
  contDescida--;
  digitalWrite(buz,LOW);
  pot = 350;
  }
  else if(analogRead(A14) < 500 && onlyDesc == false){  // DESCIDA
  condition = false;
  if(subCondition == true){
    contDescida++;
    /*if(contDescida > 4){                 //150ms identificando descida
    onlyDesc = true;
    //breakMotors();
    timeDesc = millis();
    }*/
  }
  subCondition = true;
  digitalWrite(buz,HIGH);
  pot = 100;
  kd = 100;
  kp = 1;
  }
  else if(onlyDesc == true){
    if(millis() - timeDesc < 5000){
      pot = 100;
      kd = 300;
      kp = 1;
    }
    else{
      condition = false;
      subCondition = false;
      contDescida = 0;
      pot = basePot;
      onlyDesc = false;
      digitalWrite(buz,LOW);
      kd = baseKd;
      kp = baseKp;
    }
  }
  else{
    condition = false;
    subCondition = false;
    contDescida--;
    pot = basePot;
    digitalWrite(buz,LOW);
    kd = baseKd;
    kp = baseKp;
  }
}
int valGap;
int parma;
int parmaEnc;
int startTimeFollow;
int encGapState = digitalRead(encEsq1);
int contEnc = 0;
void taskSalvaGap(){
  if(absDir < 10 && absEsq < 10 && analogRead(corC) < 400){
    valGap++;
  }
  else{
    valGap--;
  }
  if(valGap < 0){
    valGap = 0;
  }
  else if(valGap >= 10){
    analogWrite(buz, 150);
    delay(100);
    analogWrite(buz, 0);
    while(absDir < 50 && absEsq < 50 && absDir2 < 100 && absEsq2 < 100 && analogRead(corC) < 450){
      absDir = map(analogRead(corD), brancoD, pretoD, 0, 200);
      absEsq = map(analogRead(corE), brancoE, pretoE, 0, 200);
      absDir = constrain(absDir, 0, 200);
      absEsq = constrain(absEsq, 0, 200);
      absDir2 = map(analogRead(corD2), brancoD2, pretoD2, 0, 200);
      absEsq2 = map(analogRead(corE2), brancoE2, pretoE2, 0, 200);
      absDir2 = constrain(absDir2, 0, 200);
      absEsq2 = constrain(absEsq2, 0, 200);
      potM1 = constrain(0.80*(pot - (kp * (erro) + kd * (erro - lasterro))), -250, 250);
      potM2 = constrain(pot + (kp * (erro) + kd * (erro - lasterro)), -250, 250);
      md.setM1Speed(potM1);
      md.setM2Speed(potM2);
      if(digitalRead(encEsq1) != encGapState){
        contEnc++;
        encGapState = digitalRead(encEsq1);
      }
    }
    if(absDir2 >= 100){
      breakMotors();
      andarCMEsp(-1.5, -150, 100);
      breakMotors();
      while(analogRead(corE) < brancoE + 100){
        md.setM2Speed(150);
        md.setM1Speed(-75);
      }
      parma = 0;
      while(parma < 200){
       if(analogRead(corE) < brancoE + 100){
         md.setM1Speed(0);
         md.setM2Speed(200);
       }
       else{
         md.setM1Speed(200);
         md.setM2Speed(-200);
       }
       if(digitalRead(encDir1) != parmaEnc){
        parmaEnc = digitalRead(encDir1);
        parma++;
      }
     }
    }
    else if(absEsq2 >= 100){
      breakMotors();
      andarCMEsp(-1.5, -150, 100);
      breakMotors();
      while(analogRead(corD) < brancoD + 100){
        md.setM1Speed(150);
        md.setM2Speed(-75);
      }
      parma = 0;
      while(parma < 200){
       if(analogRead(corD) < brancoD + 100){
         md.setM1Speed(200);
         md.setM2Speed(0);
       }
       else{
         md.setM1Speed(-200);
         md.setM2Speed(200);
       }
       if(digitalRead(encDir1) != parmaEnc){
        parmaEnc = digitalRead(encDir1);
        parma++;
      }
     }
    }
    timeSalvaGap = 5000;
    valGap = 0;
  }
}

void taskMenor(){
  if(((absEsq2 - absDir2 ) < -180) && (absEsq - absDir > -20 && absEsq - absDir < 20) && analogRead(corC) > 450){
    breakMotors();
    analogWrite(buz, 150);
    delay(100);
    analogWrite(buz, 0);
    delay(100);
    analogWrite(buz, 150);
    delay(100);
    analogWrite(buz, 0);
    delay(100);
    andarCM(-1,-100,100);
    int contParmaContObst = 0;
     bool encoderContObst = digitalRead(encEsq1);
     while(contParmaContObst < 400){
       if(analogRead(corD) < brancoD + 50){
         md.setM1Speed(150);
         md.setM2Speed(0);
       }
       else{
         md.setM1Speed(-150);
         md.setM2Speed(150);
       }
       if(digitalRead(encEsq1) != encoderContObst){
        encoderContObst = digitalRead(encEsq1);
        contParmaContObst++;
      }
     }
     breakMotors();
    //delay(10000000);
  }
  if(((absEsq2 - absDir2 ) > 180) && (absEsq - absDir > -20 && absEsq - absDir < 20) && analogRead(corC) > 450){
    breakMotors();
    analogWrite(buz, 150);
    delay(100);
    analogWrite(buz, 0);
    delay(100);
    int contParmaContObst = 0;
     bool encoderContObst = digitalRead(encDir1);
     while(contParmaContObst < 400){
       if(analogRead(corE) < brancoE + 50){
         md.setM1Speed(0);
         md.setM2Speed(150);
       }
       else{
         md.setM1Speed(150);
         md.setM2Speed(-150);
       }
       if(digitalRead(encDir1) != encoderContObst){
        encoderContObst = digitalRead(encDir1);
        contParmaContObst++;
      }
     }
    breakMotors();
  }
}

void loop() {
  if (millis() - lastSensors >= timeSensors) {
    //exibir("tempoSensors = 2: ",millis() - lastSensors);
    if (stopSensors == false){
      taskSensors();
    }
    lastSensors = millis();
  }
  if (millis() - lastFollow >= (timeFollow)) {
    //exibir("tempoFollow = 5: ",millis() - lastFollow);
    if (stopFollow == false) {
      taskFollow();
    }
    lastFollow = millis();
  }
  
  if (millis() - lastVerde >= timeVerde) {
    //exibir("tempoVerde = 10: ",millis() - lastVerde);
    if (stopVerde == false) {
      taskVerde();
    }
    lastVerde = millis();
  }
  
  if(millis() - lastThird >= timeThird){
    //exibir("tempoThird = 100: ",millis() - lastThird);
    if(stopThird == false){
      if((analogRead(corD) > prataD - 50 && (analogRead(corD) < prataD + 50 && (analogRead(corE) > prataE - 50 && (analogRead(corE) < prataE + 50 && (getUltra(trigDir, echoDir, 50) < 12 && (getUltra(trigEsq, echoEsq, 50) < 12))))))){
        taskThird();
      }
    }
    lastThird = millis();
  }
  
  if (millis() - lastObst >= timeObst) {
    //exibir("tempoObst = 50: ",millis() - lastObst);
    if (stopObst == false) {
      if (getUltra(trigFre, echoFre, 10) < 5 && Xg > -0.25) {
        taskObst();
      }
    }
    lastObst = millis();
  }

  if (millis() - lastDebug >= timeDebug) {
    if (stopDebug == false) {
      taskDebug();
    }
    lastDebug = millis();
  }
  
  if (millis() - lastRampa >= timeRampa) {
    //exibir("tempoRampa = 100: ",millis() - lastRampa);
    if (stopRampa == false) {
      taskRampa();
    }
    lastRampa = millis();
  }ß
  
  if(millis() - lastSalvaGap >= timeSalvaGap){
    timeSalvaGap = 10;
    if (stopSalvaGap == false) {
      taskSalvaGap();
    }
    lastSalvaGap = millis();
  }
  
  if(millis() - lastMenor >= timeMenor){
    timeMenor = 10;
    if (stopMenor == false) {
      taskMenor();
    }
    lastMenor = millis();
  }
  
}
