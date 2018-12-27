//*********************

Program of linefollower robot
Video of robot: https://www.youtube.com/watch?v=ZDBzhmAAq_4

***********************//
int incomingByte;
//pot - LOW - HIGH -> FWD
byte potDir = 5;     
byte auxDir1 = 4;
byte auxDir2 = 2;
byte potEsq = 11;
byte auxEsq1 = 12;
byte auxEsq2 = 13;

byte buz = 9;

float pwr = 70;        //100       130   90       150
float kp = 0.03;         //0.05     0.06  0.035   0.055
float kd = 0.18;        //0.13      0.39   0.22     0.45
double ki = 0;
int error;
float lastError;
float p;
long i;
float d;

int s[8] = {A3, A2, A1, A0, A7, A6, A4, A5};    //da direita para a esquerda
int soma, num = 10;
int maxS[8] = {0};
int minS[8] = {1000};

int nS[8] = {0};

long valueAll = 0L;
bool memLine = false; //false = ultimo contato da linha foi no sensor esquerdo. true = // sensor direito
bool curLost = false; //armazena se o robo perdeu a linha ou não
bool onLine[8] = {true};
int numOnLine = 0;

int leftValues[20] = {1000};
int rightValues[20] = {1000};
int contLeft = 0;
int contRight = 0;

int sEsq = 8;
int sDir = 10;
int contMarkDir = 0;
int contMarkEsq = 0;

void taskCalc();
void taskFollow();

int timeFollow = 1;
int timeCalc = 1;
int timeBlue = 200;
int timeLeft = 5;
int timeRight = 5;

long int lastCalc;
long int lastFollow;
long int lastBlue;
long int lastLeft;
long int lastRight;
int millisCalib = 0;

void andar(byte mot, float pot){
  if(mot == potDir){
    if(pot > 0){
      digitalWrite(auxDir1, LOW);
      digitalWrite(auxDir2, HIGH);
      analogWrite(potDir, pot);
    }
    else{
      digitalWrite(auxDir1, HIGH);
      digitalWrite(auxDir2, LOW);
      analogWrite(potDir, -pot);
    }
  }
  else{
    if(pot > 0){
      digitalWrite(auxEsq1, LOW);
      digitalWrite(auxEsq2, HIGH);
      analogWrite(potEsq, pot);
    }
    else{
      digitalWrite(auxEsq1, HIGH);
      digitalWrite(auxEsq2, LOW);
      analogWrite(potEsq, -pot);
    }
  }
}

bool checkRight(){
  bool mem = false;
  for(int i = 0; i < 10; i++){
      if(rightValues[i] < 500){
        mem = true;
        break;
      }
  }
  if (mem == true){
    return true;
  }
  else{
    return false;
  }
}

bool checkLeft(){
  bool mem = false;
  for(int i = 0; i < 10; i++){
      if(leftValues[i] < 500){
        mem = true;
        break;
      }
  }
  if (mem == true){
    return true;
  }
  else{
    return false;
  }
}

int mediaPorta(byte porta){
  soma = 0;
  for(int i = 0; i < num; i++){
    soma += analogRead(porta);
  }
  return soma/num;
}

void setup() {
  Serial.begin(9600);
  /*while(1){
    Serial.println(analogRead(s[0]));
     Serial.println(analogRead(s[1]));
      Serial.println(analogRead(s[2]));
       Serial.println(analogRead(s[3]));
        Serial.println(analogRead(s[4]));
         Serial.println(analogRead(s[5]));
          Serial.println(analogRead(s[6]));
           Serial.println(analogRead(s[7]));
    Serial.println("-----");
  }*/
  pinMode(buz, OUTPUT);
  analogWrite(buz, 1);
  andar(potDir, 50);
  andar(potEsq, -50);
  millisCalib = millis();
  while(millis() - millisCalib < 4500){
    for(int  i = 0; i < num; i++){
      if(mediaPorta(s[i]) < minS[i]){
        minS[i] = mediaPorta(s[i]);
      }
      if(mediaPorta(s[i]) > maxS[i]){
        maxS[i] = mediaPorta(s[i]);
      }
    }
  }
  while(mediaPorta(s[3]) > 200);
  for(int i = 0; i < 8; i++){
    minS[i] = minS[i]*2;
    maxS[i] = maxS[i]*0.8;
  }
  andar(potDir, 0);
  andar(potEsq, 0);
  analogWrite(buz, 0);
  pinMode(potDir, OUTPUT);
  pinMode(potEsq, OUTPUT);
  pinMode(auxDir1, OUTPUT);
  pinMode(auxDir2, OUTPUT);
  pinMode(auxEsq1, OUTPUT);
  pinMode(auxEsq2, OUTPUT);
  pinMode(sEsq, INPUT);
  pinMode(sDir, INPUT);
  delay(2000);
}


void loop() {
  if(millis() - lastCalc >= timeCalc){
    taskCalc();
    lastCalc = millis();
  }
  if(millis() - lastFollow >= timeFollow){
    taskFollow();
    lastFollow = millis();
  }
  if(millis() - lastBlue >= timeBlue){
    taskBlue();
    lastBlue = millis();
  }
  /*  if(millis() - lastLeft >= timeLeft){
    taskLeft();
    lastLeft = millis();
  }*/
  if(millis() - lastRight >= timeRight){
    taskRight();
    lastRight = millis();
  }
}

void taskCalc(){
  /****************************************************************************
   Calculo do erro, memória de linha e checagem de marcadores que atrapalham calculo do erro
  ****************************************************************************/
  if(memLine == true){ //ultimo contato com a linha foi no sensor direito
    analogWrite(buz, 10);
  }
  else{            //ultimo contato com a linha foi no sensor esquerdo
    analogWrite(buz, 0);
  }
  for(int i = 0; i < 8; i++){     // converte as leituras para uma escala de 0 a 1000.
    nS[i] = constrain(map(mediaPorta(s[i]), minS[i], maxS[i], 1000, 0), 0, 1000);
  }

  valueAll = ((0L*nS[0]) + (1000L*nS[1]) + (2000L*nS[2]) + (3000L*nS[3]) + (4000L*nS[4]) + (5000L*nS[5]) + (6000L*nS[6]) + (7000L*nS[7]))/(nS[0] + nS[1] + nS[2] + nS[3] + nS[4] + nS[5] + nS[6] + nS[7]);
  valueAll = constrain(valueAll, 0, 7000);     
  error = valueAll - 3500;                     //error negativo, linha na direita

  numOnLine = 0;    //variável que armazena número de sensores em cima da linha
  for(int i = 0; i < 8; i++){    //ve quais sensores estão em cima da linha
    if(nS[i] < 200){
      onLine[i] = false;
    }
    else{
      onLine[i] = true;
      numOnLine++;
    }
  }
  
  if(onLine[0] == false && onLine[1] == false && onLine[2] == false && onLine[3] == false && onLine[4] == false && onLine[5] == false && onLine[6] == false && onLine[7] == false){
    curLost = true;  //caso não tenha sensor em cima da linha o robo esta perdido
    if(memLine == true){//se ultimo contato com a linha foi o lado direito, ir para a direita
      error = -3500;
    }
    else{
      error = 3500; 
    }
  }
  else{        //o robo encontrou uma linha, mas pode ser marcador. O robo só encontra marcador como linha por engano quando este estiver na extremidade esquerda. Checar!
    if(curLost == true && error > -2500 && memLine == true && onLine[0] == false){ //se previamente esteve perdido, ultimo contato com a linha no lado direito, sensor extremo direito sem contato com linha
      curLost == true;  //continua perdido(era marcador)
      error = -3500;
    }
    else if(curLost == true && error < 1000 && memLine == false && onLine[7] == false){ //desnecessário, seria caso o marcador estivesse no lado direito. Talvez remover no futuro.
      curLost == true;
      error = 3500;
    }
    else{
      curLost = false;
    }
  }
    
  if(curLost == false && memLine == true && numOnLine > 2 && numOnLine < 7 && (nS[7] == true || nS[6] == true || nS[5] == true)){// se encontrar marcador enquanto está quase saindo da linha, ignorar marcador
    valueAll = ((0L*nS[0]) + (1000L*nS[1]) + (2000L*nS[2]) + (3000L*nS[3]) + (4000L*nS[4]) + (5000L*0) + (6000L*0) + (7000L*0))/(nS[0] + nS[1] + nS[2] + nS[3] + nS[4]);    
    valueAll = constrain(valueAll, 0, 7000);
    error = valueAll - 3500;
    //serial.println(error);
  }
    
  if(error >= 2000 && curLost == false){
    memLine = false;  //tendencia que perca a linha pela esquerda
  }
  else if(error <= -2000 && curLost == false){
    memLine = true;     //tendencia que perca linha pela direita
  }  
}

void taskFollow(){
  andar(potDir, constrain(pwr + (kp*error + kd*(error - lastError) + ki*i), -255, 255));
  andar(potEsq, constrain(pwr - (kp*error + kd*(error - lastError) + ki*i), -255, 255));
  lastError = error;
  i += error;
}

void taskLeft(){
  leftValues[contLeft] = mediaPorta(s[7]);
  if(contLeft == 9) contLeft = 0;
  else{
    contLeft++;
  }
  if(digitalRead(sEsq) == LOW){//encontrou linha lateral. Checar se é encruzilhada ou marcador
    if(checkLeft() == false){ //se não encontrou encruzilhada previamente significa que é marcador
      analogWrite(buz, 100);
    }
  }
  else if(digitalRead(sDir) == HIGH){
    analogWrite(buz, 0);
 }
}

void taskRight(){
  rightValues[contRight] = mediaPorta(s[0]);
  if(contRight == 9) contRight = 0;
  else{
    contRight++;
  }
  if(digitalRead(sDir) == LOW){ //encontrou linha lateral. Checar se é encruzilhada ou marcador
    if(checkRight() == false){ //se não encontrou encruzilhada previamente significa que é marcador
      analogWrite(buz, 100);
    }
  }
  else if(digitalRead(sEsq) == HIGH){
    analogWrite(buz, 0);
  }
}

void taskBlue(){
  Serial.println(digitalRead(sDir));
  if(Serial.available() > 0){
    incomingByte = Serial.read();
    if(incomingByte == 'q'){ //letra para mostrar pot e pid atuais
    Serial.println("Current pot and PID values:");
    Serial.print("pwr:  ");
    Serial.println(pwr);
    Serial.print("P:  ");
    Serial.println(kp);
    Serial.print("I:  ");
    
    Serial.println(ki);
    Serial.print("D:  ");
    Serial.println(kd);
    }
    else if(incomingByte == 'l'){//letra para incrementar p += 0.001
      kp += 0.001;
      Serial.print("P*100:  ");
      Serial.println(kp*100);
    }
    else if(incomingByte == 'o'){//letra para incrementar p -= 0.001
      kp -= 0.001;
      Serial.print("P*100:  ");
      Serial.println(kp*100);
    }
    else if(incomingByte == 'L'){//letra para incrementar p += 0.01
      kp += 0.01;
      Serial.print("P*100:  ");
      Serial.println(kp*100);
    }
    else if(incomingByte == 'O'){//letra para incrementar p -= 0.01
      kp -= 0.01;
      Serial.print("P*100:  ");
      Serial.println(kp*100);
    }
    else if(incomingByte == 's'){//letra para incrementar d += 0.01
      kd -= 0.01;
      Serial.print("D:  ");
      Serial.println(kd);
    }
    else if(incomingByte == 'f'){//letra para incrementar d -= 0.01
      kd += 0.01;
      Serial.print("D:  ");
      Serial.println(kd);
    }
    else if(incomingByte == 'S'){//letra para incrementar d += 0.1
      kd -= 0.1;
      Serial.print("D:  ");
      Serial.println(kd);
    }
    else if(incomingByte == 'F'){//letra para incrementar d -= 0.1
      kd += 0.1;
      Serial.print("D:  ");
      Serial.println(kd);
    }
    else if(incomingByte == 'j'){//letra para incrementar i += 0.00001
      ki += 0.0000001;
      Serial.print("I*1000:  ");
      Serial.println(ki*1000);
    }
    else if(incomingByte == 'u'){//letra para incrementar i -= 0.00001
      ki -= 0.0000001;
      Serial.print("I*10000:  ");
      Serial.println(ki*10000);
    }
    else if(incomingByte == 'J'){//letra para incrementar i += 0.0001
      ki += 0.001;
      Serial.print("I*1000:  ");
      Serial.println(ki*1000);
    }
    else if(incomingByte == 'U'){//letra para incrementar i -= 0.0001
      ki -= 0.001;
      Serial.print("I*100:  ");
      Serial.println(ki*100);
    }
    else if(incomingByte == 'c'){//letra para incrementar pot += 1
      pwr += 1;
      Serial.print("pot:  ");
      Serial.println(pwr);
    }
    else if(incomingByte == 'z'){//letra para incrementar pot -= 1
      pwr -= 1;
      Serial.print("pot:  ");
      Serial.println(pwr);
    }
    else if(incomingByte == 'C'){//letra para incrementar pot += 5
      pwr += 5;
      Serial.print("pot:  ");
      Serial.println(pwr);
    }
    else if(incomingByte == 'Z'){//letra para incrementar pot -= 5
      pwr -= 5;
      Serial.print("pot:  ");
      Serial.println(pwr);
    }
  }
}
