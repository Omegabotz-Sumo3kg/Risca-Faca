// Code made by Nick
// Contact: nicolas_castrosilva@outlook.com

//Servo Libraries
#include <ESP32Servo.h>
#include <Arduino.h>

//PS4 Controller Libraries
#include <ps4.h>
#include <PS4Controller.h>
#include <ps4_int.h>

//Robot states of operation
enum robotStates {
  LOCKED, AUTO, MANUAL
};
robotStates robotState = LOCKED;
bool right = true;
bool desempate = false;

bool optionPressed = false;

//IR Remote library and variables
#include <IRremote.h>
#define irReceiverPin 23
IRrecv irrecv(irReceiverPin);
decode_results results;

//Presence Sensor Pins and Variables on the board
int rightInfSensorPin = 5; //17 5
int leftInfSensorPin = 19; //23 22 
int middleInfSensorPin = 18;
int leftInfSensorRefPin = 99; //INSERIR O NUMERO NO ESP
int rightInfSensorRefPin = 98; //INSERIR O NUMERO NO ESP

const int rightInfSensor = 2;
const int leftInfSensor = 3;
const int middleInfSensor = 4;
const int leftInfSensorRef = 5;
const int rightInfSensorRef = 6;

void sensorTest();
void Status_Verify();
void MotorWrite();
void Controlado();
void IRRead();
void Suicidio();
void Radar();
void fradar();
void Auto();
void ManualControl();
void Teste();
void Movimentacao();
void RadarInverso();

//Auto mode states of operation (1,2,3 TV controll)
enum autoStates {
  READY, RUNNING, STOPPED
};
autoStates autoState = STOPPED;

enum tatics {
   RADAR,
   SUICIDIO,
   MOVIMENTACAO,
   RADAR_INVERSO
   //TESTE
};
tatics tatic = RADAR;

//PS4 LED status variables
unsigned long blinkTimer;
bool ledOn = true;
int ledIntensity;

//Define Motors Pins and Instances
#define leftMotorPin 27
#define rightMotorPin 26

Servo MotorEsquerdo;
Servo MotorDireito;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  irrecv.enableIRIn(); //Enable IR Receiver

  //Start Connection between ESP32 and PS4 Controller
  PS4.begin("44:1c:a8:c6:41:80");
  while (!PS4.isConnected()) {
    Serial.println("WatingConnection");
    delay(250);
  }

  pinMode(rightInfSensor, INPUT);
  pinMode(leftInfSensor, INPUT);
  pinMode(middleInfSensor, INPUT);
  pinMode(leftInfSensorRef, INPUT);
  pinMode(rightInfSensorRef, INPUT);

  PS4.setLed(100, 0, 0);
  PS4.sendToController();

  MotorEsquerdo.attach(leftMotorPin);
  MotorDireito.attach(rightMotorPin);
  MotorEsquerdo.write(90);
  MotorDireito.write(90);

  Serial.println("Ready and LOCKED");
}

void loop() {
  if (PS4.isConnected()) {
    Status_Verify();
  } else {
    MotorEsquerdo.write(90);
    MotorDireito.write(90);
  }
}

void sensorTest() {
  //Serial.println("TestintSensors");
  if (digitalRead(leftInfSensorPin)) {
    Serial.println("Left");
  }
  if (digitalRead(middleInfSensorPin)) {
    Serial.println("Middle");
  }
  if (digitalRead(rightInfSensorPin)) {
    Serial.println("Right");
  }
}

void Status_Verify() {
  if (PS4.Options()) {
    if (!optionPressed) {
      optionPressed = true;
      if (robotState == LOCKED) {
        robotState = AUTO;
        PS4.setLed(0, 100, 0);
        PS4.sendToController();
        MotorEsquerdo.write(90);
        MotorDireito.write(90);
        Serial.println("AUTO");

      } else if (robotState == AUTO) {
        robotState = MANUAL;
        PS4.setLed(0, 0, 100);
        PS4.sendToController();
        MotorEsquerdo.write(90);
        MotorDireito.write(90);
        autoState = STOPPED;
        Serial.println("MANUAL");

      } else if (robotState == MANUAL) {
        robotState = LOCKED;
        autoState = STOPPED;
        PS4.setLed(100, 0, 0);
        PS4.sendToController();
        MotorEsquerdo.write(90);
        MotorDireito.write(90);
        Serial.println("LOCKED");
      }
    }
  } else {
    optionPressed = false;
  }

  if (robotState == MANUAL) {
    ManualControl();
  } else if (robotState == AUTO) {
    Auto();
  }
}

void MotorWrite(int ppmDireito, int ppmEsquerdo) {
  MotorDireito.write(ppmDireito);
  MotorEsquerdo.write(ppmEsquerdo);
}

void IRRead() {
  String value;
  if (irrecv.decode(&results))
  {
    Serial.println(results.value);
    value = String(results.value, HEX);
    Serial.println(value);
    irrecv.resume();
  }
  if (value == "10") {
    if (autoState == STOPPED) {
      autoState = READY;
      PS4.setLed(0, 100, 100);
      PS4.sendToController();
      MotorWrite(90, 90);
    }
  } else if (value == "810") {
    if (autoState == READY) {
      autoState = RUNNING;
      PS4.setLed(0, 100, 0);
      PS4.sendToController();
    }
  } else if ( value == "410") {
    if (autoState == RUNNING || autoState == READY) {
      autoState = STOPPED;
      PS4.setLed(100, 100, 0);
      PS4.sendToController();
      MotorWrite(90, 90);
    }
  }
}
void Radar() {
  Serial.println("RadarStart");
  if (!desempate) {
    unsigned int timerStart = millis() + 300;
    while (timerStart > millis()) {
      MotorWrite(120, 120);
    }
  }

  while (autoState == RUNNING) {
    while ((digitalRead(rightInfSensor) && digitalRead(leftInfSensor) && digitalRead(middleInfSensor)) && autoState == RUNNING) {
      //Serial.println("NotFind");
      IRRead();
      //Status_Verify();
      if (right) {
        MotorWrite(95, 110);
      } else {
        MotorWrite(110, 95);
      }
    }
    right = !right;
    while (!digitalRead(middleInfSensor) && autoState == RUNNING) {
      //Serial.println("Find");
      IRRead();
      //Status_Verify();
      MotorWrite(150, 150);
    }
    while (autoState == STOPPED) {   
      IRRead();
      MotorWrite(0, 0);
      }
  }
}
void Movimentacao() {
  static int movimento = 0;
  static bool direcao = true; 
  int cont=0;
  rightInfSensor == digitalRead(rightInfSensor);
  leftInfSensor == digitalRead(leftInfSensor);
  middleInfSensor == digitalRead(middleInfSensor);

  if (digitalRead(middleInfSensor)==true) {
    MotorWrite(130, 130);
  } else if (digitalRead(leftInfSensor)==true) {
    MotorWrite(80, 95);
    delay(100);
  } else if (digitalRead(rightInfSensor)==true) {
    MotorWrite(100, 80);
    delay(100);
  }else if (digitalRead(leftInfSensorRef)) {
    MotorWrite(80, 95);
    delay(100);
  } else if (digitalRead(rightInfSensorRef)) {
    MotorWrite(100, 80);
    delay(100);
  }
   if (direcao) {
      //MotorWrite(-80, 100);
    //  delay(100);
    //} else {
     // MotorWrite(100, -80);
    //  delay(100);
    //}
    direcao = !direcao;
  } else if(digitalRead(middleInfSensor)) {

    
    do{
      if (movimento < 2) {
      MotorWrite(100,100);
      direcao = true;
    } else if (movimento < 1) {
      MotorWrite(80, 80);
      direcao = false;
    } else {
      movimento = 0; 
    }
    movimento++;
    cont++;
    
    } while(cont<=50 || digitalRead(middleInfSensor)==false );
    /*if (movimento < 2) {
      MotorWrite(100,100);
      direcao = true;
    } else if (movimento < 1) {
      MotorWrite(100, 100);
      direcao = false;
    } else {
      movimento = 0; 
    }
    movimento++;*/
  }
}


void RadarInverso() {
  Serial.println("RadarInversoStart");
  if (!desempate) {
    unsigned int timerStart = millis() + 300;
    while (timerStart > millis()) {
      MotorWrite(120, 120);
    }
  }

  while (autoState == RUNNING) {
    // Verifica se os sensores identificam um robô adversário
    if (digitalRead(rightInfSensor) == LOW || digitalRead(leftInfSensor) == LOW || digitalRead(middleInfSensor) == LOW) {
      // Realiza uma volta de ré
      while (digitalRead(middleInfSensor) == LOW && autoState == RUNNING) {
        IRRead();
        MotorWrite(-120, -120); // Marcha ré
      }
      delay(200); // Delay para garantir que a volta de ré foi completada
    }
    
    // Avança para frente
    while ((digitalRead(rightInfSensor) && digitalRead(leftInfSensor) && digitalRead(middleInfSensor)) && autoState == RUNNING) {
      IRRead();
      if (right) {
        MotorWrite(95, 110);
      } else {
        MotorWrite(110, 95);
      }
    }
    right = !right;
    while (!digitalRead(middleInfSensor) && autoState == RUNNING) {
      IRRead();
      MotorWrite(150, 150);
    }
    while (autoState == STOPPED) {   
      IRRead();
      MotorWrite(0, 0);
    }
  }
}

void Suicidio() {
  Serial.println("SuicidioStart");
  if (!desempate) {
    unsigned int timerStart = millis() + 300;
    while (timerStart > millis()) {
      MotorWrite(120, 120);
    }
  }

  while (autoState == RUNNING) {  
      IRRead();
      while(!digitalRead(middleInfSensor)){
        IRRead();
      MotorWrite(150, 150);
      PS4.setLed(100, 0, 0);
      PS4.sendToController();
      }
      }
  while (autoState == STOPPED) {   
      IRRead();
      MotorWrite(0, 0);
      }
}
void Auto() {
  IRRead();
  if (PS4.Square()) {
    tatic = RADAR_INVERSO;
  }
  if (PS4.Circle()) {
    tatic = RADAR;
  }
  if (PS4.Triangle()) {
    tatic = MOVIMENTACAO;
  }
  if (PS4.Cross()) {
    tatic = SUICIDIO;
  }
  if (PS4.Right()) {
    right = true;
  }
  if (PS4.Left()) {
    right = false;
  }
  if (PS4.Up()) {
    desempate = false;
  }
  if (PS4.Down()) {
    desempate = true;
  }

  if (autoState == RUNNING) {
    if (tatic == SUICIDIO) {
      Suicidio();
    } else if (tatic == MOVIMENTACAO){
      Movimentacao();
    } else if (tatic == RADAR) {
      Radar();
    } else if (tatic == RADAR_INVERSO) {
      RadarInverso();
    }
  } else if (autoState == READY) {
    MotorWrite(90, 90);
    if (blinkTimer < millis()) {
      if (ledOn) {
        PS4.setLed(0, 0, 0);
        PS4.sendToController();
      } else {
        PS4.setLed(0, 100, 0);
        PS4.sendToController();
      }
      ledOn = !ledOn;
      blinkTimer = millis() + 200;
    }
  } else if (autoState == STOPPED) {
    MotorWrite(90, 90);
    if (blinkTimer < millis()) {
      if (ledOn) {
        PS4.setLed(0, ledIntensity++, 0);
        PS4.sendToController();
      } else {
        PS4.setLed(0, ledIntensity--, 0);
        PS4.sendToController();
      }
      if (ledIntensity == 0 || ledIntensity == 100) {
        ledOn = !ledOn;
      }
      blinkTimer = millis() + 10;
    }
  }
}

void ManualControl() {
  int forward = PS4.R2Value();
  int steering = PS4.LStickX();
  int backward = PS4.L2Value();

  if (steering < 10 && steering > -10) {
    steering = 0;
  }

  if (forward < 10) {
    forward = 0;
  }

  if (PS4.Cross()) {
    forward = 63;
    if (PS4.Square() ) {
      forward = 180;
    }
  } else if (PS4.Circle()) {
    forward = -63;
  }

  int leftMotorOutput = constrain(map(forward, 0, 255, 90, 150)  - (map(backward, 0, 255, 0, 60) - (map(steering, -127, 127, 70, 90) - 80)), 30, 150);
  int rightMotorOutput = constrain(map(forward, 0, 255, 90, 150) - (map(backward, 0, 255, 0, 60) + (map(steering, -127, 127, 70, 90) - 80)), 30, 150);

  MotorEsquerdo.write(leftMotorOutput);
  MotorDireito.write(rightMotorOutput);
}
