#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver rightBoard = Adafruit_PWMServoDriver(0x40); //Right board
Adafruit_PWMServoDriver leftBoard = Adafruit_PWMServoDriver(0x41); //Left board
#define SERVO_FREQ 60 // Analog servos run at ~50 Hz updates

/* 
 * Array que contiene la información de calibracion y posicion inicial de cada servo:
 * ServoData[NumeroServo][X] = (pulso_min, pulso_max, ang_min, ang_max, ang_inicial)
 * NumeroServo = 0,1,2,3...19 (numero identificador del servo)
 * x = 0(pulso_min), 1(pulso_max), 2(ang_min), 3(ang_max), 4(ang_inicial)
*/

int ServoData[20][5] = {
                            {90,  490,  70, 103,  90  },
                            {90,  470,  0,  180,  90+25  },
                            {93,  470,  64, 180,  180-45 },
                            {84,  466,  10, 180,  90-40  },
                            {98,  495,  90, 180,  90  },
                            {62,  447,  90, 150,  90  },
                            {110, 495,  0,  180,  45  },
                            {139, 536,  0,  180,  0   },
                            {90,  508,  45, 180,  180 },
                            {105, 490,  0,  90,   45  },
                            {93,  485,  77, 110,  90  },
                            {78,  470,  0,  180,  90-25  },
                            {116, 515,  0,  116,  0+45   },
                            {87,  485,  0,  170,  90+40  },
                            {138, 528,  0,  90,   90  },
                            {125, 520, 30,  90,   90  },
                            {104, 504,  0,  180,  135 },
                            {106, 496,  0,  180,  180 },
                            {100, 500,  0,  135,  0   },
                            {136, 534,  0,  90,   45  }
};

// Variables
int desfase = 20; // Tiempo de espera entre cambios de angulo en los servos (en milisegundos)


void setup() {

  Serial.begin(9600);
  
  rightBoard.begin();
  leftBoard.begin();
  rightBoard.setOscillatorFrequency(27000000);
  leftBoard.setOscillatorFrequency(27000000);
  rightBoard.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  leftBoard.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  
  delay(100);
  //poner los servos en su angulo inicial (robot de pie con los brazos abajo)
  ServosCero();
  delay(400);
  
}

void loop() {
    
    /*  El array <rutina>, contiene en cada fila, los angulos que se desplazará cada servo 0 a 19, a partir de su punto inicial
    *  El ultimo valor de cada fila, es la duración del movimiento en milisegundos 
    *  Ejemplo:
    *  int <rutina>[][21] =   {
    *   {1R,   2R,   3R,   4R,   5R,   6R,   7R,   8R,   9R,   10W,   1L,   2L,   3L,   4L,   5L,   6L,   7L,   8L,   9L,   10N,   tiempo},
    *   {...},
    *  };
    */


// Rutina de movimiento: "Sentadillas"
    int sentadilla[][21] = {
      {90,   90+40,   180-90,   90-80,   90,   90,   45+135,   0,   180,   45,   90,   90-40,   0+90,   90+80,   90,   90,   135-135,   180,   0,   45,   2000},
      {90,   90+25,   180-45,   90-40,   90,   90,   45,   0,   180,   45,   90,   90-25,   0+45,   90+40,   90,   90,   135,   180,   0,   45,   2000}
      };
    int filas = sizeof(sentadilla)/sizeof(sentadilla[0]); // Obtiene el numero de filas del array sentadilla[][]
    MoverServos(sentadilla, filas); // Ejecuta la rutina de movimiento
    ServosCero(); // Retorna el robot a su posición inicial
    delay(2000);

// Rutina de movimiento: "Saludo"
    int saludo[][21] = {
      {90,   90+25,   180-45,   90-40,   90,   90,   45,   0,   180,   45,   90,   90-25,   0+45,   90+40,   90,   90,   135,   180,   0,   45,   2000},
      {90,   90+25,   180-45,   90-40,   90,   90,   45,   0,   180,   45,   90,   90-25,   0+45,   90+40,   90,   90,   135,   180,   0,   45,   2000},
      {90,   90+25,   180-45,   90-40,   90,   90,   45,   0,   180,   45,   90,   90-25,   0+45,   90+40,   90,   90,   135,   180,   0,   45,   2000},
      };
    int filas = sizeof(saludo)/sizeof(saludo[0]); // Obtiene el numero de filas del array saludo[][]
    MoverServos(saludo, filas); // Ejecuta la rutina de movimiento
    ServosCero(); // Retorna el robot a su posición inicial
    delay(2000);

// Rutina de movimiento: "Pose"
    int pose[][21] = {
      {90,   90+25,   180-45,   90-40,   90,   90,   45,   0,   180,   45,   90,   90-25,   0+45,   90+40,   90,   90,   135,   180,   0,   45,   2000},
      {90,   90+25,   180-45,   90-40,   90,   90,   45,   0,   180,   45,   90,   90-25,   0+45,   90+40,   90,   90,   135,   180,   0,   45,   2000},
      {90,   90+25,   180-45,   90-40,   90,   90,   45,   0,   180,   45,   90,   90-25,   0+45,   90+40,   90,   90,   135,   180,   0,   45,   2000},
      };
    int filas = sizeof(pose)/sizeof(pose[0]); // Obtiene el numero de filas del array pose[][]
    MoverServos(pose, filas); // Ejecuta la rutina de movimiento
    ServosCero(); // Retorna el robot a su posición inicial
    delay(2000);

  
}

//Función que mapea los valores de angulo (0 a 180 grados) hacia valores de pulso, entre el minimo y maximo definidos
int AnguloAPulso (int numservo, int angulo) {
  int pulso = map(angulo, 0, 180, ServoData[numservo][0], ServoData[numservo][1]); 
  //ServoData[NumeroServo][X] = (pulso_min, pulso_max, ang_min, ang_max, ang_inicial)
  return pulso;  
}

// Función de posiciona todos los servomotores en su posición inicial 
void ServosCero() {          

      for(int i=0; i<20; i++){
        PosicionarServo(i,ServoData[i][4]);
        //Serial.print("\nServo "); Serial.print(i);Serial.print(": Angulo: "); Serial.print(ServoData[i][4]);
        delay(300);
      }
  delay(200);
}

//Función que desplaza el servo a una nueva posición
void PosicionarServo(float numservo, float ang) {

  if(numservo >= 0 && numservo < 10){
    //Driver derecho
    rightBoard.setPWM(15-numservo, 0, AnguloAPulso(numservo,ang)); // rightBoard.setPWM(canal,0,pulso)    
  } else if(numservo >= 10 && numservo < 20){
    //Driver izquierdo
    leftBoard.setPWM(numservo-10, 0, AnguloAPulso(numservo,ang)); // leftBoard.setPWM(canal,0,pulso)
  } else {
    //Serial.println("Error: numservo debe ser entero, entre 0 y 19.");
  }  
  //Serial.print("\nServo: ");Serial.print(numservo);Serial.print(" Angulo: ");Serial.print(ang);
}

// Función que realiza la rutina de movimiento 
void MoverServos(int mov[][21], int filas){
  
  for(int i=0; i < filas; i++){ // recorre todas las filas del array. Todas las etapas del movimiento
    int duracion = mov[i][20];
    int tiempo = 0;
    int cont = 1;

    while (tiempo < duracion)
    {
      for(int j=0; j < 20; j++){ // recorre las primeras 20 columnas, posicionando cada servo
        
        float angulo_inicial;        
        if(i==0){// Si es la primera fila, el angulo inicial es la posición cero del servo
          angulo_inicial = ServoData[j][4];
        } else { // Si es otra fila, el ángulo inicial es la posición de la fila anterior
          angulo_inicial = mov[i-1][j];
        }
        float angulo_final = mov[i][j];
        float angulo_despl = angulo_final - angulo_inicial;

        float paso = angulo_despl * desfase / duracion;
        float angulo = angulo_inicial+paso*cont;

        /* --- Corregir luego la imprecisión debida a desfases que no son sub multiplos de la duración :v */
        if(j==1){
          Serial.print("\nServo 1 ----------");
          Serial.print("\nAngulo inicial:");Serial.print(angulo_inicial);
          Serial.print("\nAngulo final:");Serial.print(angulo_final);
          Serial.print("\nAngulo desplazamiento:");Serial.print(angulo_despl);
          Serial.print("\nAngulo actual:");Serial.print(angulo);
          Serial.print("\nPaso:");Serial.print(paso);
          Serial.print("\nContador:");Serial.print(cont);
          Serial.print("\nTiempo:");Serial.print(tiempo);
        }
        /* - */
               
        PosicionarServo(j,angulo);               

      }      
      cont = cont + 1;
      tiempo = tiempo + desfase;
      delay(desfase);
    }     
    
  }
  
}