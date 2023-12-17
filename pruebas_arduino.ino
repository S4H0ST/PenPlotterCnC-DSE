#include <AccelStepper.h>
#include <Math.h>
#include <Servo.h>
#include <MultiStepper.h>
//Coordenadas
struct Coordenada{
  int puntoX;
  int puntoY;
};

//Puntos que componen la matriz
  Coordenada Punto_1;
  Coordenada Punto_2;
  Coordenada Punto_3;
  Coordenada Punto_4;
  Coordenada Punto_5;
  Coordenada Punto_6;
  Coordenada Punto_7;
  Coordenada Punto_8;
  Coordenada Punto_9;
  Coordenada Entre_4_y_5;
  Coordenada Entre_5_y_6;
  Coordenada Entre_4_y_7;
  Coordenada Entre_4_y_1;
  Coordenada Entre_1_y_2;
  Coordenada Entre_2_y_3;

#define DISTANCIA_PUNTOS 50
#define X_STEP_PIN 2
#define X_DIR_PIN 5
#define Y_STEP_PIN 4 //El Z es el servo  y el Y es el verdadero Z
#define Y_DIR_PIN 7
#define Z_STEP_PIN 3
#define Z_DIR_PIN 6
#define X_ENDSTOP_PIN 9 //low
#define Y_ENDSTOP_PIN 10
#define Z_ENDSTOP_PIN 11

bool servo_abajo = true;
Servo miServo; 
int homeY=1000;

String palabra;

AccelStepper stepperX(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepperY(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN);
AccelStepper stepperZ(AccelStepper::DRIVER, Z_STEP_PIN, Z_DIR_PIN);

MultiStepper multimotor;

//Homming para el eje X 
void homingX(AccelStepper &motor,int finalCarrera ){
  pinMode(finalCarrera,INPUT_PULLUP);
  bool no_terminado = true;
  while(no_terminado){
    motor.moveTo(10000); //dirección hacia el boton
    while(digitalRead(finalCarrera)==HIGH && motor.distanceToGo()!=0){
      motor.run();
      if(digitalRead(finalCarrera)==LOW){
        motor.stop();
        no_terminado=false;
        break;
      }
    }
  } 
  motor.setCurrentPosition(0);
  motor.moveTo(-1800); //dirección opuesto al boton
  while(motor.distanceToGo()!=0){
    motor.run();
  }
  motor.setCurrentPosition(0); //Ponemos q nuestra posicion actual es el 0
}

//Homming para el eje Y
void homingY(AccelStepper &motor,int finalCarrera ){
  pinMode(finalCarrera,INPUT_PULLUP);
  bool no_terminado = true;
  while(no_terminado){
    motor.moveTo(-10000);//dirección hacia el boton
    while(digitalRead(finalCarrera)==HIGH && motor.distanceToGo()!=0){
      motor.run();
      if(digitalRead(finalCarrera)==LOW){
        motor.stop();
        no_terminado=false;
        break;
      }
    }
  } 
  motor.setCurrentPosition(0);
  motor.moveTo(homeY); //dirección opuesto al boton
  while(motor.distanceToGo()!=0){
    motor.run();
  }
  homeY=homeY-300;
  motor.setCurrentPosition(0); //Ponemos q nuestra posicion actual es el 0
}

void setup() {
  
  Serial.begin(9600);
  pinMode(X_ENDSTOP_PIN, INPUT_PULLUP);
  pinMode(Y_ENDSTOP_PIN, INPUT_PULLUP);
  pinMode(Z_ENDSTOP_PIN, INPUT_PULLUP);
  stepperX.setMaxSpeed(100);
  stepperX.setAcceleration(100);
  stepperY.setMaxSpeed(100);
  stepperY.setAcceleration(100);
  stepperZ.setMaxSpeed(100); // Velocidad máxima ajustada para el eje Z
  stepperZ.setAcceleration(100); // Aceleración ajustada para el eje Z
  
  miServo.attach(12); // poner el servo en el pin adecuado ( numero 12 : spinEnable )

//Configuración de la posición de los puntos
  Punto_1.puntoX=0;
  Punto_1.puntoY=0;
  
  Punto_2.puntoX=Punto_1.puntoX+DISTANCIA_PUNTOS;
  Punto_2.puntoY=Punto_1.puntoY;
  
  Punto_3.puntoX=Punto_2.puntoX+DISTANCIA_PUNTOS;
  Punto_3.puntoY=Punto_1.puntoY;
  
  Punto_4.puntoX=Punto_1.puntoX;
  Punto_4.puntoY=Punto_1.puntoY-DISTANCIA_PUNTOS;
  
  Punto_5.puntoX=Punto_4.puntoX+DISTANCIA_PUNTOS;
  Punto_5.puntoY=Punto_4.puntoY;
  
  Punto_6.puntoX=Punto_5.puntoX+DISTANCIA_PUNTOS;
  Punto_6.puntoY=Punto_4.puntoY;
  
  Punto_7.puntoX=Punto_1.puntoX;
  Punto_7.puntoY=Punto_4.puntoY-DISTANCIA_PUNTOS;

  Punto_8.puntoX=Punto_7.puntoX+DISTANCIA_PUNTOS;
  Punto_8.puntoY=Punto_7.puntoY;
  
  Punto_9.puntoX=Punto_8.puntoX+DISTANCIA_PUNTOS;
  Punto_9.puntoY=Punto_7.puntoY;

  Entre_4_y_5.puntoX=Punto_1.puntoX+(DISTANCIA_PUNTOS/2);
  Entre_4_y_5.puntoY=Punto_4.puntoY;


  Entre_5_y_6.puntoX=Punto_5.puntoX+(DISTANCIA_PUNTOS/2);
  Entre_5_y_6.puntoY=Punto_5.puntoY;


  Entre_4_y_7.puntoX=Entre_5_y_6.puntoX;
  Entre_4_y_7.puntoY=Punto_4.puntoY-(DISTANCIA_PUNTOS/2);
  
  Entre_4_y_1.puntoX=Punto_2.puntoX+(DISTANCIA_PUNTOS/2);
  Entre_4_y_1.puntoY=Punto_2.puntoY-(DISTANCIA_PUNTOS/2);
  
  Entre_1_y_2.puntoX=Punto_1.puntoX+(DISTANCIA_PUNTOS/2);
  Entre_1_y_2.puntoY=Punto_1.puntoY;
  
  Entre_2_y_3.puntoX=Punto_2.puntoX+(DISTANCIA_PUNTOS/2);
  Entre_2_y_3.puntoY=Punto_2.puntoY;
  
  multimotor.addStepper(stepperX);
  multimotor.addStepper(stepperY);

  
}

  
void Escribir_caracter(char caracter){

  switch (caracter)
  {
  case '0':
    drawNumber0();
    break;
  case '1':
    drawNumber1();
    break;
  case '2':
    drawNumber2();
    break;
  case '3':
    drawNumber3();
    break;
  case '4':
    drawNumber4();
    break;
  case '5':
    drawNumber5();
    break;
  case '6':
    drawNumber6();
    break;
  case '7':
    drawNumber7();
    break;
  case '8':
    drawNumber8();
    break;
  case '9':
    drawNumber9();
    break;
  case 'A':
    drawLetterA();
    break;
  case 'B':
    drawLetterB();
    break;
  case 'C':
    drawLetterC();
    break;
  case 'D':
    drawLetterD();
    break;
  case 'E':
    drawLetterE();
    break;
  case 'F':
    drawLetterF();
    break;
  case 'G':
    drawLetterG();
    break;
  case 'H':
    drawLetterH();
    break;
  case 'I':
    drawLetterI();
    break;
  case 'J':
    drawLetterJ();
    break;
  case 'K':
    drawLetterK();
    break;
  case 'L':
    drawLetterL();
    break;
  case 'M':
    drawLetterM();
    break;
  case 'N':
    drawLetterN();
    break;
  case 'Ñ':
    drawLetterN2();
    break;    
  case 'O':
    drawLetterO();
    break;
  case 'P':
    drawLetterP();
    break;
  case 'Q':
    drawLetterQ();
    break;
  case 'R':
    drawLetterR();
    break;
  case 'S':
    drawLetterS();
    break;
  case 'T':
    drawLetterT();
    break;
  case 'U':
    drawLetterU();
    break;
  case 'V':
    drawLetterV();
    break;
  case 'W':
    drawLetterW();
    break;
  case 'X':
    drawLetterX();
    break;
  case 'Y':
    drawLetterY();
    break;
  case 'Z':
    drawLetterZ();
    break;
  case ' ':
    Espacio_Blanco();
    break; 
  default:
    Serial.println("Caracter No válido");
    break;
  }
}  


void Escribir_palabra(String palabra){
  for(int i=0;i<palabra.length();i++){
    char caracter = palabra.charAt(i);
    Escribir_caracter(caracter);
    delay(1000);
    Espacio_Blanco();
  }
}
void Espacio_Blanco(){
  mover(Punto_1,Entre_1_y_2,false);
}

void loop() {
//Aqui inicia la ejecución del programa
//1. Inicia el Homming  
  miServo.write(30); // soy mario y acabo de meter esto , levanta al servo al principio
  servo_abajo = false;
  delay(3000);
  mover(Punto_1,Punto_1,false);
  delay(1000);
  //homingX(stepperX,X_ENDSTOP_PIN);
  //homingY(stepperY,Y_ENDSTOP_PIN);
 
  
  while(1){
  Serial.println("Ingrese su palabra: ");
  delay(100); //lo he cambiado de 1000 a 100
  if(Serial.available()>0){
    palabra = Serial.readStringUntil('\n');
    palabra.trim(); // Elimina espacios al principio y al final de la cadena

    // Convertir cada carácter a mayúsculas
    for (unsigned int k = 0; k < palabra.length(); k++) {
      palabra.setCharAt(k, toupper(palabra.charAt(k)));
    } 
    Serial.println(palabra);
  
    Escribir_palabra(palabra);
    if(servo_abajo){
      miServo.writeMicroseconds(map(0,0,180,1000,2000));
      servo_abajo=false;
      delay(1000);
    }
  homingX(stepperX,X_ENDSTOP_PIN);
  homingY(stepperY,Y_ENDSTOP_PIN);

    delay(1000);
  
  }else{
    delay(10000);
  }
  
}
}

void mover(Coordenada Origen,Coordenada Destino, boolean tiene_que_pintar){
  //Falso -> NO PINTO (LEVANTO SERVO)
  //True -> PINTO (BAJO SERVO)
  //Pintado lo q quiere q hagamos ahora
  //servo_abajo lo q estamos haciendo
 
  int distanciaX =Destino.puntoX-Origen.puntoX;
  int distanciaY = Destino.puntoY-Origen.puntoY;

  if(tiene_que_pintar==servo_abajo){
      //No hacemos nada ya q queremos q haga lo mismo q ya tenemos
  }else{
    if(tiene_que_pintar && !servo_abajo){
      //Tenemos q bajar el servo para q pinte
      miServo.write(180); // baja el servo
      //miServo.writeMicroseconds(map(180,0,180,1000,2000));
      servo_abajo=true;
      delay(1000);
    }else if(!tiene_que_pintar && servo_abajo){
      //!Pintado && servo_abajo
      //Subir el servo
      miServo.write(30);
      //miServo.writeMicroseconds(map(0,0,180,1000,2000));
      //moverServo(-90);
      servo_abajo=false;
      delay(1000);
    }
  }

    if(distanciaX != 0 && distanciaY==0){
    //Movimiento horizontal
    if(distanciaX<0){
      stepperX.setSpeed(-15); // Velocidad negativa para mover hacia la izquierda
    } 
    else {
      // Movimiento horizontal hacia la derecha
      stepperX.setSpeed(15); // Velocidad positiva para mover hacia la derecha  
    }
    stepperX.moveTo(distanciaX);
    while(stepperX.distanceToGo()!=0){
      stepperX.run();
      if(digitalRead(X_ENDSTOP_PIN)==LOW){
        stepperX.stop();
        homingX(stepperX,X_ENDSTOP_PIN);
        homingY(stepperY,Y_ENDSTOP_PIN);
        break;
      }
    }
    stepperX.setCurrentPosition(0);
    stepperY.setCurrentPosition(0);
    }else if(distanciaX==0 &&  distanciaY!=0){

    //Movimiento vertical
    if(distanciaY<0){
      stepperY.setSpeed(-15);
    } 
    else {
      // Movimiento horizontal hacia la derecha
      stepperY.setSpeed(15);
    }
    stepperY.moveTo(distanciaY);
    while(stepperY.distanceToGo()!=0){
      stepperY.run();
      if(digitalRead(Y_ENDSTOP_PIN)==LOW){
        stepperY.stop();
        homingX(stepperX,X_ENDSTOP_PIN);
        homingY(stepperY,Y_ENDSTOP_PIN);
        break;
      }
    }
    stepperY.setCurrentPosition(0);
    stepperX.setCurrentPosition(0);
    }else{

    // Movimiento diagonal
    long positions[]= {distanciaX,distanciaY};
    multimotor.moveTo(positions);
    while(stepperX.distanceToGo()!= 0 && stepperY.distanceToGo()!= 0){
      multimotor.run();
      if(digitalRead(Y_ENDSTOP_PIN)==LOW || digitalRead(X_ENDSTOP_PIN)==LOW || digitalRead(Z_ENDSTOP_PIN)==LOW){
        stepperX.stop();
        stepperY.stop();
        homingX(stepperX,X_ENDSTOP_PIN);
        homingY(stepperY,Y_ENDSTOP_PIN);
        break;
      }
    }

    // Restaurar posición inicial
    stepperX.setCurrentPosition(0);
    stepperY.setCurrentPosition(0);
    } 
  }

  void moverServo(int grados) {
    miServo.write(grados);  // Utiliza la función write para establecer la posición del servo
    delay(15); 
  }

//------------------------------ FUNCIONES PARA ESCRIBIR TODO EL ABECEDARIO ------------------------------------//
void drawNumber0(){
  mover(Punto_1,Punto_3,true);
  delay(1000);
  mover(Punto_3,Punto_9,true);
  delay(1000);
  mover(Punto_9,Punto_7,true);
  delay(1000);
  mover(Punto_7,Punto_1,true);
  delay(1000);
  mover(Punto_1,Punto_9,true);
  delay(1000);
  mover(Punto_9,Punto_3,true);
}
void drawNumber1(){
  mover(Punto_1,Punto_4,false);
  delay(1000);
  mover(Punto_4,Punto_2,true);
  delay(1000);
  mover(Punto_2,Punto_3,true);
  delay(1000);
  mover(Punto_3,Punto_9,true);
  delay(3000);
  mover(Punto_9,Punto_3,false);
}
void drawNumber2(){
  mover(Punto_1,Punto_3,true);
  delay(1000);
  mover(Punto_3,Punto_6,true);
  delay(1000);
  mover(Punto_6,Punto_4,true);
  delay(1000);
  mover(Punto_4,Punto_7,true);
  delay(1000);
  mover(Punto_7,Punto_9,true);
  delay(1000);
  mover(Punto_9,Punto_3,false);

}
void drawNumber3(){
  mover(Punto_1,Punto_3,true);
  delay(1000);
  mover(Punto_3,Punto_9,true);
  delay(1000);
  mover(Punto_9,Punto_7,true);
  delay(1000);
  mover(Punto_7,Punto_9,true);
  delay(1000);
  mover(Punto_9,Punto_6,true);
  delay(1000);
  mover(Punto_6,Punto_4,true);
  delay(1000);
  mover(Punto_4,Punto_3,false);
}
void drawNumber4(){
  mover(Punto_1,Punto_4,true);
  delay(1000);
  mover(Punto_4,Punto_6,true);
  delay(1000);
  mover(Punto_6,Punto_9,true);
  delay(1000);
  mover(Punto_9,Punto_3,true);
}
void drawNumber5(){
  mover(Punto_1,Punto_3,true);
  delay(1000);
  mover(Punto_3,Punto_1,true);
  delay(1000);
  mover(Punto_1,Punto_4,true);
  delay(1000);
  mover(Punto_4,Punto_6,true);
  delay(1000);
  mover(Punto_6,Punto_9,true);
  delay(1000);
  mover(Punto_9,Punto_7,true);
  delay(1000);
  mover(Punto_7,Punto_3,false);
}

void drawNumber6(){
  mover(Punto_1,Punto_3,true);
  delay(1000);
  mover(Punto_3,Punto_1,true);
  delay(1000);
  mover(Punto_1,Punto_7,true);
  delay(1000);
  mover(Punto_7,Punto_9,true);
  delay(1000);
  mover(Punto_9,Punto_6,true);
  delay(1000);
  mover(Punto_6,Punto_4,true);
  delay(1000);
  mover(Punto_4,Punto_3,false);
}

void drawNumber7(){
  mover(Punto_1,Punto_3,true);
  delay(1000);
  mover(Punto_3,Punto_9,true);
  delay(1000);
  mover(Punto_9,Punto_6,true);
  delay(1000);
  mover(Punto_6,Punto_5,true);
  delay(1000);
  mover(Punto_5,Punto_3,false);
}
void drawNumber8(){
  mover(Punto_1,Punto_3,true);
  delay(1000);
  mover(Punto_3,Punto_9,true);
  delay(1000);
  mover(Punto_9,Punto_7,true);
  delay(1000);
  mover(Punto_7,Punto_1,true);
  delay(1000);
  mover(Punto_1,Punto_4,true);
  delay(1000);
  mover(Punto_4,Punto_6,true);
  delay(1000);
  mover(Punto_6,Punto_3,true);
}

void drawNumber9(){
  mover(Punto_1,Punto_3,true);
  delay(1000);
  mover(Punto_3,Punto_9,true);
  delay(1000);
  mover(Punto_9,Punto_7,true);
  delay(1000);
  mover(Punto_7,Punto_9,true);
  delay(1000);
  mover(Punto_9,Punto_6,true);
  delay(1000);
  mover(Punto_6,Punto_4,true);
  delay(1000);
  mover(Punto_4,Punto_1,true);
  delay(1000);
  mover(Punto_1,Punto_3,true);
}
//FUNCIONES PARA ESCRIBIR TODO EL ABECEDARIO
void drawLetterA(){
  mover(Punto_1,Punto_2,false); //Me muevo sin pintar
  delay(1000);
  mover(Punto_2,Punto_7,true);
  delay(1000);
  mover(Punto_7,Punto_2,true);
  delay(1000);
  mover(Punto_2,Punto_9,true);
  delay(1000);
  mover(Punto_9,Punto_2,true);
  delay(1000);
  mover(Punto_2,Punto_5,false);
  delay(1000);
  mover(Punto_5,Entre_4_y_5,true);
  delay(1000);
  mover(Entre_4_y_5,Punto_5,true);
  delay(1000);
  mover(Punto_5,Entre_5_y_6,true);
  delay(1000);
  mover(Entre_5_y_6,Punto_3,false);
  
}
void drawLetterB(){
  mover(Punto_1,Punto_4,true);
  delay(1000);
  mover(Punto_4,Punto_5,true);
  delay(1000);
  mover(Punto_5,Punto_4,true);
  delay(1000);
  mover(Punto_4,Punto_7,true);
  delay(1000);
  mover(Punto_7,Punto_8,true);
  delay(1000);
  mover(Punto_8,Entre_4_y_7,true);
  delay(1000);
  mover(Entre_4_y_7,Punto_5,true);
  delay(1000);
  mover(Punto_5,Entre_4_y_1,true);
  delay(1000);
  mover(Entre_4_y_1,Punto_2,true);
  delay(1000);
  mover(Punto_2,Punto_1,true);
  delay(1000);
  mover(Punto_1,Punto_3,false);
}
void drawLetterC(){
  mover(Punto_1,Punto_3,true);
  delay(1000);
  mover(Punto_3,Punto_1,true);
  delay(1000);
  mover(Punto_1,Punto_7,true);
  delay(1000);
  mover(Punto_7,Punto_9,true);
  delay(1000);
  mover(Punto_9,Punto_3,false);
}
void drawLetterD(){
  //Mirar como va
  mover(Punto_1,Punto_2,true);
  delay(1000);
  mover(Punto_2,Entre_5_y_6,true);
  delay(1000);
  mover(Entre_5_y_6,Punto_8,true);
  delay(1000);
  mover(Punto_8,Punto_7,true);
  delay(1000);
  mover(Punto_7,Punto_1,true);
  delay(1000);
  mover(Punto_1,Punto_3,true);
  
}
void drawLetterE(){
  mover(Punto_1,Punto_3,true);
  delay(1000);
  mover(Punto_3,Punto_1,true);
  delay(1000);
  mover(Punto_1,Punto_4,true);
  delay(1000);
  mover(Punto_4,Punto_6,true);
  delay(1000);
  mover(Punto_6,Punto_4,true);
  delay(1000);
  mover(Punto_4,Punto_7,true);
  delay(1000);
  mover(Punto_7,Punto_9,true);
  delay(1000);
  mover(Punto_9,Punto_3,false);  
}  
void drawLetterF(){
    mover(Punto_1,Punto_2,true);// linea horizontal
    delay(1000);
    mover(Punto_2,Punto_1,true);//volver origen
    delay(1000);
    mover(Punto_1,Punto_4,true);//linea vertical
    delay(1000);
    mover(Punto_4,Punto_5,true);//linea horizontal
    delay(1000);
    mover(Punto_5,Punto_4,true);//volver atras
    delay(1000);
    mover(Punto_4,Punto_7,true);//linea vertical
    delay(1000);
    mover(Punto_7,Punto_3,false);
    //falta la mitad del cuadrado libre asique no dar espacio entre letras si se usa esta letra primero
    //ya que esta letra ya cuenta con medio cuadrado de espacio
  }
  void drawLetterG(){
    mover(Punto_1,Punto_2,true);// linea horizontal
    delay(1000);
    mover(Punto_2,Punto_3,true);// linea horizontal
    delay(1000);
    //volvemos sobre nuestros pasos
    mover(Punto_3,Punto_2,true);// linea horizontal
    delay(1000);
    mover(Punto_2,Punto_1,true);// linea horizontal
    delay(1000);
    //seguimos con la letra
    mover(Punto_1,Punto_4,true);// linea vertical
    delay(1000);
    mover(Punto_4,Punto_7,true);// linea vertical
    delay(1000);
    mover(Punto_7,Punto_8,true);// linea horizontal
    delay(1000);
    mover(Punto_8,Punto_9,true);// linea horizontal
    delay(1000);
    mover(Punto_9,Punto_6,true);// linea vertical
    delay(1000);
    mover(Punto_6,Punto_5,true);// linea horizontal
    delay(1000);
    mover(Punto_5,Punto_3,false);
  }
 void drawLetterH(){
    mover(Punto_1,Punto_4,true);// linea vertical
    delay(1000);
    mover(Punto_4,Punto_7,true);// linea vertical
    delay(1000);
    mover(Punto_7,Punto_4,true);// volvemos al anterior punto
    delay(1000);
    mover(Punto_4,Punto_5,true);// linea horizontal
    delay(1000);
    mover(Punto_5,Punto_6,true);// linea horizontal
    delay(1000);
    mover(Punto_6,Punto_3,true);// linea vertical
    delay(1000); 
    mover(Punto_3,Punto_6,true);// volvemos al anterior punto
    delay(1000);
    mover(Punto_6,Punto_9,true);// linea vetical
    delay(1000);
    mover(Punto_9,Punto_3,true);
 }
 void drawLetterI(){
    //Suponemos q estamos siempre en el punto 1 al empezar
    //Lineas Horizontales
    mover(Punto_1,Punto_2,true);
    delay(1000);
    mover(Punto_2,Punto_3,true);
    delay(1000);
    mover(Punto_3,Punto_2,true); //Fallo esta aqui
    delay(1000);
    //Lineas Verticales
    mover(Punto_2,Punto_5,true);
    delay(1000);
    mover(Punto_5,Punto_8,true);
    delay(1000);
    //lineas horizontales
    mover(Punto_8,Punto_7,true);
    delay(1000);
    mover(Punto_7,Punto_8,true);//volvemos al punto anterior
    delay(1000);
    mover(Punto_8,Punto_9,true);
    delay(1000);
    mover(Punto_9,Punto_3,false);
  }
  void drawLetterJ(){
    mover(Punto_1,Punto_2,true);// linea horizontal
    delay(1000);
    mover(Punto_2,Punto_3,true);// linea horizontal
    delay(1000);
    mover(Punto_3,Punto_2,true);// volvemos al punto anterior
    delay(1000);
    mover(Punto_2,Punto_5,true);// linea vetical
    delay(1000);
    mover(Punto_5,Punto_8,true);// linea vetical
    delay(1000);
    mover(Punto_8,Punto_7,true);// linea horizontal
    delay(1000);
    mover(Punto_7,Punto_4,true);// linea vetical
    delay(1000);
    mover(Punto_4,Punto_3,false);
  }
  void drawLetterK(){
    mover(Punto_1,Punto_4,true);// linea vertical
    delay(1000);
    mover(Punto_4,Punto_2,true);// linea diagonal acendente
    delay(1000);
    mover(Punto_2,Punto_4,true);// linea diagonal descendente (vuelve al punto de antes)
    delay(1000);
    mover(Punto_4,Punto_8,true);// linea diagonal descendente
    delay(1000);
    mover(Punto_8,Punto_4,true);// linea diagonal ascendente (vuelve al punto de antes)
    delay(1000);
    mover(Punto_4,Punto_7,true);// linea vertical
    delay(1000);
    mover(Punto_7,Punto_3,false);
    //falta la mitad del cuadrado libre asique no dar espacio entre letras si se usa esta letra primero
    //ya que esta letra ya cuenta con medio cuadrado de espacio
  }
  void drawLetterL(){
    mover(Punto_1,Punto_4,true);// linea vertical
    delay(1000);
    mover(Punto_4,Punto_7,true);// linea vertical
    delay(1000);
    mover(Punto_7,Punto_8,true);// linea horizontal
    delay(1000);
    mover(Punto_8,Punto_3,false);
    //falta la mitad del cuadrado libre asique no dar espacio entre letras si se usa esta letra primero
    //ya que esta letra ya cuenta con medio cuadrado de espacio
  }
  void drawLetterM(){
    mover(Punto_1,Punto_4,true);// linea vertical
    delay(1000);
    mover(Punto_4,Punto_7,true);// linea vertical
    delay(1000);
    mover(Punto_7,Punto_4,true);// volvemos al punto anterior 
    delay(1000);
    mover(Punto_4,Punto_1,true);// volvemos al punto anterior
    delay(1000);
    mover(Punto_1,Punto_5,true);// linea diagonal descendente
    delay(1000);
    mover(Punto_5,Punto_3,true);// linea diagonal ascendente
    delay(1000);
    mover(Punto_3,Punto_6,true);// linea vertical
    delay(1000);
    mover(Punto_6,Punto_9,true);// linea vertical
    delay(1000);
    mover(Punto_9,Punto_3,false);
  }
  void drawLetterN(){
    mover(Punto_1,Punto_4,true);// linea vertical
    delay(1000);
    mover(Punto_4,Punto_7,true);// linea vertical
    delay(1000);
    mover(Punto_7,Punto_4,true);// volvemos al punto anterior
    delay(1000);
    mover(Punto_4,Punto_1,true);// volvemos al punto anterior
    delay(1000);
    mover(Punto_1,Punto_5,true);// linea diagonal descendente
    delay(1000);
    mover(Punto_5,Punto_9,true);// linea diagonal descendente
    delay(1000);
    mover(Punto_9,Punto_6,true);// linea vertical ascendente
    delay(1000);
    mover(Punto_6,Punto_3,true);// linea vertical ascendente
    
  }
void drawLetterN2(){ // no me deja poner la Ñ en el nombre de la FUncion , lo sustituyo por su pronunciacion . Ñ = enie
  mover(Punto_1,Punto_7,true);
  delay(1000);
  mover(Punto_7,Punto_1,false);
  delay(1000);
  mover(Punto_1,Punto_9,true);
  delay(1000);
  mover(Punto_9,Punto_3,true);
  delay(1000);
  mover(Punto_3,Entre_1_y_2,false);
  delay(1000);
  mover(Entre_1_y_2,Entre_2_y_3,true);
  delay(1000);
  // modificacion para que acabe en el punto 3
  mover(Entre_2_y_3,Punto_3,false);
}
void drawLetterO(){
  mover(Punto_1,Punto_2,true);
  delay(1000);
  mover(Punto_2,Punto_8,true);
  delay(1000);
  mover(Punto_8,Punto_7,true);
  delay(1000);
  mover(Punto_7,Punto_1,true);
  //modifificacion para qye acabe en el punto 3
  mover(Punto_1,Punto_3,false);
}
void drawLetterP(){
  mover(Punto_1,Punto_2,true);
  delay(1000);
  mover(Punto_2,Punto_5,true);
  delay(1000);
  mover(Punto_5,Punto_4,true);
  delay(1000);
  mover(Punto_4,Punto_7,true);
  delay(1000);
  mover(Punto_7,Punto_1,true);
   delay(1000);
  mover(Punto_1,Punto_3,false);
}
void drawLetterQ(){ // la Q no es bonita , pero es una Q
  mover(Punto_1,Punto_2,false);
  delay(1000);
  mover(Punto_2,Punto_8,true);
  delay(1000);
  mover(Punto_8,Punto_9,true);
  delay(1000);
  mover(Punto_9,Punto_3,true);
  delay(1000);
  mover(Punto_3,Punto_9,true);
  delay(1000);
  mover(Punto_9,Entre_5_y_6,true);
  delay(1000);
  mover(Entre_5_y_6,Punto_3,false);
}
void drawLetterR(){
  mover(Punto_1,Punto_2,true);
  delay(1000);
  mover(Punto_2,Punto_5,true);
  delay(1000);
  mover(Punto_5,Punto_4,true);
  delay(1000);
  mover(Punto_4,Punto_8,true);
  delay(1000);
  mover(Punto_8,Punto_7,false);
  delay(1000);
  mover(Punto_7,Punto_1,true);
  delay(1000);
  mover(Punto_1,Punto_3,false);
}
void drawLetterS(){
  mover(Punto_1,Punto_2,true);
  delay(1000);
  mover(Punto_2,Punto_1,false);
  delay(1000);
  mover(Punto_1,Punto_4,true);
  delay(1000);
  mover(Punto_4,Punto_5,true);
  delay(1000);
  mover(Punto_5,Punto_8,true);
  delay(1000);
  mover(Punto_8,Punto_7,true);
  delay(1000);
  mover(Punto_7,Punto_3,false);
}
void drawLetterT(){
    //Suponemos q estamos siempre en el punto 1 al empezar
    //Lineas Horizontales
    mover(Punto_1,Punto_2,true);
    delay(1000);
    mover(Punto_2,Punto_3,true);
    delay(1000);
    mover(Punto_3,Punto_2,true); //Fallo esta aqui
    delay(1000);
    //Lineas Verticales
    mover(Punto_2,Punto_5,true);
    delay(1000);
    mover(Punto_5,Punto_8,true);
    delay(1000);
    mover(Punto_8,Punto_3,false);
    
}
void drawLetterU(){
  mover(Punto_1,Punto_7,true);
  delay(1000);
  mover(Punto_7,Punto_8,true);
  delay(1000);
  mover(Punto_8,Punto_2,true);
  delay(1000);
  mover(Punto_2,Punto_3,false);
}
void drawLetterV(){
  mover(Punto_1,Punto_8,true);
  delay(1000);
  mover(Punto_8,Punto_3,true);
}
void drawLetterW(){
  mover(Punto_1,Punto_7,true);
  delay(1000);
  mover(Punto_7,Punto_5,true);
  delay(1000);
  mover(Punto_5,Punto_9,true);
  delay(1000);
  mover(Punto_9,Punto_3,false);
}
void drawLetterX(){
  mover(Punto_1,Punto_9,true);
  delay(1000);
  mover(Punto_9,Punto_7,true);
  delay(1000);
  mover(Punto_7,Punto_3,false);
}
void drawLetterY(){
  mover(Punto_1,Punto_5,true);
  delay(1000);
  mover(Punto_5,Punto_8,true);
  delay(1000);
  mover(Punto_8,Punto_5,false);
  delay(1000);
  mover(Punto_5,Punto_3,false);
}
void drawLetterZ(){
  mover(Punto_1,Punto_3,true);
  delay(1000);
  mover(Punto_3,Punto_7,true);
  delay(1000);
  mover(Punto_7,Punto_9,true);
  delay(1000);
  mover(Punto_9,Punto_3,false);
}