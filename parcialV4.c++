// Bibliotecas
#include <Servo.h>

// Definicion de pines
#define ledVerde 13
#define ledRojo 12
#define SEG_A 10
#define SEG_B 11
#define SEG_F 9
#define SEG_G 8
#define SEG_C 5
#define SEG_D 6
#define SEG_E 7

#define botonSube 4
#define botonPara 3
#define botonBaja 2

#define motorA A5

//Definicion de variables 
int valorFlexor;
int pisoActual = 0;
int tiempoPiso = 3000; // milisegundos
int sensorTemperatura = A4;
int temperatura;

bool flagQuieto = true;

// como va arrancar el elevador
bool sube = false;
bool baja = false;
bool vaParar = true;

bool flagPuerta1 = true;
bool flagPuerta2 = true;

bool puertaAbierta = false;

Servo servito;

// Setup
void setup() {
  // Salidas del 7Segmentos
    pinMode(SEG_A, OUTPUT);
    pinMode(SEG_B, OUTPUT);
    pinMode(SEG_C, OUTPUT);
    pinMode(SEG_D, OUTPUT);
    pinMode(SEG_E, OUTPUT);
    pinMode(SEG_F, OUTPUT);
    pinMode(SEG_G, OUTPUT);
  // Entradas de los botones
    pinMode(botonSube, INPUT_PULLUP);
    pinMode(botonBaja, INPUT_PULLUP);
    pinMode(botonPara, INPUT_PULLUP);
  // Habilito la consola
    Serial.begin(9600);
  // Establezco el elevador en 0
    numeros(1, 1, 1, 1, 1, 1, 0);
    digitalWrite(ledRojo,HIGH);
    
  // Motor aficionado
    pinMode(motorA,OUTPUT);
  // Sensor flexion
  	pinMode(A0,INPUT);
  // Servo
    servito.attach(A1, 400, 2600);
    servito.write(0);
}

void loop() {
  quiereAbrir();
  mostrar_piso(pisoActual);
  setAccion();
  mover_montacarga();
  aireAcondicionado();  
}

void setAccion() {
  //SUBIR
  if (digitalRead(botonSube) == 0 ) {
    sube = true;
    vaParar = false;
    baja = false;
  // DETENER
  }else if (digitalRead(botonPara) == 0) {
    vaParar = true;
    sube = false;
    baja = false;
  //BAJAR
  }else if (digitalRead(botonBaja) == 0) {
    sube = false;
    vaParar = false;
    baja = true;
  }
}
// funcion para establecer los numeros en el 7 segmentos
void numeros(int A, int B, int C, int D, int E, int F, int G) {
  digitalWrite(SEG_A, A);
  digitalWrite(SEG_B, B);
  digitalWrite(SEG_C, C);
  digitalWrite(SEG_G, G);
  digitalWrite(SEG_D, D);
  digitalWrite(SEG_E, E);
  digitalWrite(SEG_F, F);
}

// printeo el piso en el 7 segmentos
void mostrar_piso(int piso) {
  
  switch (piso) {
    case 0:
      numeros(1, 1, 1, 1, 1, 1, 0);//numero 0
      break;
    case 1:
      numeros(0, 1, 1, 0, 0, 0, 0);//numero 1
      break;
    case 2:
      numeros(1, 1, 0, 1, 1, 0, 1);//numero 2
      break;
    case 3:
      numeros(1, 1, 1, 1, 0, 0, 1);//numero 3
      break;
    case 4:
      numeros(0, 1, 1, 0, 0, 1, 1);//numero 4
      break;
    case 5:
      numeros(1, 0, 1, 1, 0, 1, 1);//numero 5
      break;
    case 6:
      numeros(1, 0, 1, 1, 1, 1, 1);//numero 6
      break;
    case 7:
      numeros(1, 1, 1, 0, 0, 0, 0);//numero 7
      break;
    case 8:
      numeros(1, 1, 1, 1, 1, 1, 1);//numero 8
      break;
    case 9:
      numeros(1, 1, 1, 0, 0, 1, 1);//numero 9
      break;
  }
}

// a partir de los botones: SUBE-QUIETO-BAJA
void mover_montacarga() {

  //SUBE
    if (sube == true) {
        //si el piso es menos a 9 puede seguir subiendo 
        if (pisoActual < 9) {
            sistemaFuncionando();
            pisoActual += 1;
            Serial.println("");
            Serial.print("Subiendo al piso: ");
            Serial.println(pisoActual);
            cambio_piso();
        }else {
            Serial.println("El montacarga se encuentra en el 9no piso, no puede subir más.");
            vaParar = true;
            sube = false;
        }
        // vuelvo a setear las banderas para el mensaje
        flagQuieto = true;
    }

    //PARA
    if (vaParar == true) {
        //detengo el montacargas (no aumenta ni disminuye el piso)
        sistemaApagado();
        if (flagQuieto == true){
            Serial.println("");
            Serial.print("El montacarga esta quieto en el piso: ");
            Serial.println(pisoActual);
            flagQuieto = false;
        }
    }

    // BAJA
    if (baja==true) {

        if(pisoActual > 0) {
            sistemaFuncionando();
            pisoActual -= 1;
          
            Serial.println("");
            Serial.print("Bajando al piso: ");
            Serial.println(pisoActual);
            cambio_piso();

        }else {
            Serial.println("");
            Serial.println("El montacarga se encuentra en el piso 0, no puede bajar más.");
            vaParar = true;
            baja = false;
        }
        // vuelvo a setear la bandera para el mensaje
        flagQuieto = true;
  }
}

// Controlo los segundos que tarda subir o bajar de piso
void cambio_piso() {
  for (int i = 0; i<tiempoPiso;i++){
    setAccion();
    delay(1);
    quiereAbrir();
    // deteccion de la puerta del elevador
    if (puertaAbierta == true ){
      if (flagPuerta1 == true){
        Serial.println("");
        Serial.println("Espere a que el montacargas se detenga para abrirlo");
        flagPuerta1 = false;
      }
    }  
    else{
      flagPuerta1 = true; 
    }
  }
}
// Elevador encendido 
void sistemaFuncionando(){
    //primero prendo y apago las leds correspondientes
    digitalWrite(ledRojo,LOW);
    digitalWrite(ledVerde,HIGH);

    //Motor de aficionado
    digitalWrite(motorA,HIGH);
    
}

// Elevador apagado
void sistemaApagado(){

    quiereAbrir();
    //primero prendo y apago las leds correspondientes
    digitalWrite(ledVerde,LOW);
    digitalWrite(ledRojo,HIGH);

    //Motor de aficionado
    digitalWrite(motorA,LOW);

    //Sensor de flexion para detectar la puerta del elevador
    if (puertaAbierta == true){
      if  (flagPuerta2 == true){
        Serial.println("Se abrio la puerta del montacargas");
        flagPuerta2 = false;
      }
    }else{
      if (flagPuerta2 == false)
      {
        Serial.println("Se cerro la puerta del montacargas");
        flagPuerta2 = true;
      }
    }
}
// Detecto el Sensor de Flexion
void quiereAbrir(){
  valorFlexor = analogRead(A0);
  // Serial.println(valorFlexor);
  if (valorFlexor > 990){
    puertaAbierta = true;
  
  }else{
    puertaAbierta = false;
  }
}
// Detecto el sensor de Temperatura
float leerSensorTemp(int sensorTemperatura_1)
{ 
  int numero = analogRead(sensorTemperatura_1);
  float temp = 0.4882 * numero - 49.763;
  return temp;
}
// Establezco la funcion del servo
void aireAcondicionado(){
    temperatura = leerSensorTemp(sensorTemperatura);
    if (temperatura >= 30 )
    {
        servito.write(90);
    }else{
        servito.write(0);
    }
}