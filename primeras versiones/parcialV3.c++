
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

int valorFlexor;

bool sube = false;
bool baja = false;

bool vaParar = false;

int pisoActual = 0;

int tiempoPiso = 3000; // milisegundos

bool flagPuerta1 = true;
bool flagPuerta2 = true;

bool flagQuieto = true;

bool puertaAbierta = false;

void setup() {
    pinMode(SEG_A, OUTPUT);
    pinMode(SEG_B, OUTPUT);
    pinMode(SEG_C, OUTPUT);
    pinMode(SEG_D, OUTPUT);
    pinMode(SEG_E, OUTPUT);
    pinMode(SEG_F, OUTPUT);
    pinMode(SEG_G, OUTPUT);
    pinMode(botonSube, INPUT_PULLUP);
    pinMode(botonBaja, INPUT_PULLUP);
    pinMode(botonPara, INPUT_PULLUP);
    Serial.begin(9600);
    numeros(1, 1, 1, 1, 1, 1, 0);
    digitalWrite(ledRojo,HIGH);
    
    //MOTOR AFICIONADO
    pinMode(motorA,OUTPUT);
  	//SENSOR DE FLEXION
  	pinMode(A0,INPUT);

}

void loop() {
  
  setAccion();
  mostrar_piso(pisoActual);
  mover_montacarga();
  quiereAbrir();
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

void numeros(int A, int B, int C, int D, int E, int F, int G) {
  digitalWrite(SEG_A, A);
  digitalWrite(SEG_B, B);
  digitalWrite(SEG_C, C);
  digitalWrite(SEG_G, G);
  digitalWrite(SEG_D, D);
  digitalWrite(SEG_E, E);
  digitalWrite(SEG_F, F);
}

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

void mover_montacarga() {
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

void cambio_piso() {
  for (int i = 0; i<tiempoPiso;i++){
    setAccion();
    delay(1);
    quiereAbrir();
    if (puertaAbierta == true ){
      if (flagPuerta1 == true){
        Serial.println("Espere a que el montacargas se detenga para abrirlo");
        flagPuerta1 = false;
      }
    }  
    else{
      flagPuerta1 = true; 
    }
  }
}
void sistemaFuncionando(){
    //primero prendo y apago las leds correspondientes
    digitalWrite(ledRojo,LOW);
    digitalWrite(ledVerde,HIGH);
    //Motor de aficionado
    digitalWrite(motorA,HIGH);
    
}
void sistemaApagado(){

   
    //primero prendo y apago las leds correspondientes
    digitalWrite(ledVerde,LOW);
    digitalWrite(ledRojo,HIGH);
    //Motor de aficionado
    digitalWrite(motorA,LOW);
    quiereAbrir();
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

void quiereAbrir(){
  valorFlexor = analogRead(A0);
  // Serial.println(valorFlexor);
  if (valorFlexor > 990){
    puertaAbierta = true;
  
  }else{
    puertaAbierta = false;
  }
  
}
