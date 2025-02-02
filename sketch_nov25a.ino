#include <SPI.h>  
#include <Pixy.h>

Pixy pixy;

float Kp = 0;
float Kd = 0;
float Ki = 0;

int P;
int I;
int D;

int lastError = 0;

int ancho;
int alto;
int area;

int in1 = 6;
int in2 = 7;
int in3 = 8;
int in4 = 9;
int ENA = 10;
int ENB = 5;


unsigned long tiempo = 0;
const long inicio = 2000;
const long tiempo_1 = 3000;
const long tiempo_2 = 4000;
const long tiempo_3 = 5000;

const uint8_t maxspeeda = 70;
const uint8_t maxspeedb = 70;
const uint8_t basespeeda = 100;
const uint8_t basespeedb = 100;

void setup() {
  Serial.begin(9600);
  
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(ENB,OUTPUT);
  
  pixy.init();
}

void loop() {
  PID_control();

//  bloque();
//  //moveCar();
//  tiempo = millis();
//    
//    if((tiempo >= inicio) && (tiempo <= tiempo_1)){ //0 a 2000
//      analogWrite(ENA,100);
//      analogWrite(ENB,115);
//      digitalWrite(in1, HIGH);
//      digitalWrite(in2,LOW);
//      digitalWrite(in3, HIGH);
//      digitalWrite(in4,LOW);
//    }
//    
//    else if((tiempo > tiempo_1) && (tiempo <= tiempo_2)){ //2000 a 3000
//      analogWrite(ENA,0);
//      analogWrite(ENB,0);
//      digitalWrite(in1, LOW);
//      digitalWrite(in2,LOW);
//      digitalWrite(in3, LOW);
//      digitalWrite(in4,LOW);
//    }
////    else if((tiempo > tiempo_2) && (tiempo <= tiempo_3)){ // 3000 a 5000
////
////      analogWrite(ENA,100);
////      analogWrite(ENB,156);
////      digitalWrite(in1, LOW);
////      digitalWrite(in2,HIGH);
////      digitalWrite(in3, LOW);
////      digitalWrite(in4,HIGH);
////    }
////    else{
////      analogWrite(ENA,0);
////      analogWrite(ENB,0);
////      digitalWrite(in1, LOW);
////      digitalWrite(in2,LOW);
////      digitalWrite(in3, LOW);
////      digitalWrite(in4,LOW);
////    }
//  
//}

//void bloque(){
//  static int i = 0;
//  int j;
//  uint16_t blocks;
//  blocks = pixy.getBlocks();
//  char buf[32]; 
//  if (blocks)
//  {
//    i++;
//    if (i%6==0) //50 -> 1 dps   25 -> 2dps   13-> 4dps   6 -> 8dps 
//    {
//      for (j=0; j<blocks; j++)
//      {
//        Serial.print(buf); 
//        pixy.blocks[j].print();
//      }
//    }
//  }
//}


  
}
void foward_backwards(int posa, int posb,int dir, int lado){
  tiempo = millis();

  if(dir == 1){                // Si dir mover hacia adelante
    analogWrite(ENA,posa);
    analogWrite(ENB,posb);
    digitalWrite(in1, HIGH);
    digitalWrite(in2,LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4,LOW);
  }
  else if(dir == 0){                   // else mover hacia atras
    analogWrite(ENA,posa);
    analogWrite(ENB,posb);
    digitalWrite(in1, LOW);
    digitalWrite(in2,HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4,HIGH);
  }
  if (lado == 1){
    analogWrite(ENA,posa);
    analogWrite(ENB,posb);
    digitalWrite(in1, HIGH);
    digitalWrite(in2,LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4,HIGH);
  }
}

void PID_control(){

  int i;
  uint16_t blocks;
  blocks = pixy.getBlocks();
  
  if(blocks){
    for(i=0; i<=blocks;i++){
      uint16_t ancho = pixy.blocks[i].width;
      uint16_t alto = pixy.blocks[i].height;
      area = ancho * alto;
      pixy.blocks[i].print();
    }
  }
  
  
  
  int error = 654 - area;
  int direccion;
  int lados;

  P = error;
  I = I + error;
  D = error - lastError;
  lastError = error;

  int motorspeed = P*Kp + I*Ki + D*Kd;

  int motorspeeda = basespeeda + motorspeed;
  int motorspeedb = basespeedb - motorspeed;


  if ((area >= 400)&& (area < 600)){     //Si se encuentra dentro del rango no mover ni hacia adelante ni hacia atras
    direccion = 0;
    lados = 0;
    motorspeeda = 0;
    motorspeedb = 0;
    
  }
  else if(area >= 600){
    direccion = 1; 
    lados = 0;                //Mover hacia delante
    motorspeeda = maxspeeda;
    motorspeedb = maxspeedb;
    
  }
  else if(area < 400){            //Mover hacia atras
    direccion = 0;
    lados = 0;
    motorspeeda = maxspeeda;
    motorspeedb = maxspeedb;
    
  }
  else{                         //Cuando no detecte el objeto mover hacia los lados
    lados = 1;
    motorspeeda = maxspeeda;
    motorspeedb = maxspeedb;
    
    
  }
  Serial.print(area);
  foward_backwards(motorspeeda, motorspeedb, direccion, lados);

}
