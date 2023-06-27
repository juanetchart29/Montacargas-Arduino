# Montacargas-Arduino
 # Proyecto de Simulación de Ascensor

En este proyecto de simulación de ascensor, utilizaremos Tinkercad Arduino para crear un sistema que simule el funcionamiento de un ascensor. El proyecto involucra varios componentes que desempeñan funciones específicas para el correcto funcionamiento del ascensor. A continuación, se describen los componentes clave y sus roles en el proyecto:

![Proyecto armado](/src/elevador.png)

## Componentes Necesarios

### 1. Arduino UNO

El Arduino UNO actúa como la plataforma principal para nuestro proyecto. Es el cerebro del sistema y ejecuta el código que controla el funcionamiento del ascensor. Se comunica con otros componentes y coordina sus acciones.

### 2. Pantalla 7Segmentos

La pantalla 7 segmentos es lo que nos permitira conocer el piso en el que se encuentra el elevador.

### 3. Botones

Utilizaremos varios botones para simular la interacción del usuario con el ascensor. Tendremos botones para seleccionar un piso de destino, un botón de subida y un botón de bajada para solicitar que el ascensor se mueva en una dirección específica.

### 4. Motor de CC

El motor de CC es el componente responsable de la acción física del ascensor. Utilizaremos un motor de CC para simular el movimiento hacia arriba y hacia abajo del ascensor. Controlaremos la dirección y velocidad del motor utilizando transistores y señales de salida del Arduino.

### 5. Sensores de Piso

Utilizaremos sensores de piso para detectar en qué piso se encuentra actualmente el ascensor. Estos sensores nos permitirán ajustar el comportamiento del ascensor, como detenerse en el piso correcto y mostrar la información correcta en la pantalla LCD.

### 6. LED

Utilizaremos LEDs para indicar el estado del ascensor. Por ejemplo, un LED rojo puede indicar que el ascensor está detenido, mientras que un LED verde puede indicar que está en movimiento.

### 7. Sensor de Temperatura

Incluiremos un sensor de temperatura para monitorear la temperatura ambiente dentro del ascensor. Esto nos permitirá controlar el sistema de aire acondicionado y ajustar la temperatura según sea necesario para brindar comodidad a los pasajeros.

### 8. Servomotor

Utilizaremos un servomotor para simular la apertura y cierre de las puertas del ascensor. Controlaremos la posición del servomotor para abrir y cerrar las puertas en los momentos adecuados.

### 9. Protoboard(opcional)
En este proyecto el uso de la protoboard no es mas que para ordenar el proyecto y darle un lugar mas estable a los sensores.

### 10. Cables y Resistencias
Los cables y resistencias que requieren cada componente


## Codigo:
### Declaraciones y setup
```c++

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

```
### Funcion Principal:
```c++
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



```
