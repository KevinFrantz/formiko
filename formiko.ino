/** 
 * Formikoj (Ameisen) Roboter
 * --------------------------
 * Besteht aus 
 * - zwei Steppermotoren
 * - zwei Linetracking\Distanz IR Sensoren
 * - Einem Ultraschalsensoren
 * - 1 Statusled
 * 
 * @author kf
 * @since 2017-01-16
 **/


//include "Communication.h"
//Bibliothek für Ultraschalsensor
#include <NewPing.h>

// Ultraschallsensor
#define TRIGGER_PIN   3  
#define ECHO_PIN      4  
#define MAX_DISTANCE  400 

// Infarosensoren
#define INFAROT_LEFT  2
#define INFAROT_RIGHT 12

//LED's
#define INTERNAL_LED 13
#define EXTERNAL_LED 10
// Motoren
#define MOTOR_LEFT_DIRECTION 6
#define MOTOR_LEFT_PWM 5
#define MOTOR_RIGHT_DIRECTION 8
#define MOTOR_RIGHT_PWM 7

#define MOTOR_PWM_MIN 10            // arbitrary slow speed PWM duty cycle
#define MOTOR_PWM_MAX 200           // arbitrary fast speed PWM duty cycle
#define MOTOR_DIRECTION_DELAY 200  // brief delay for abrupt motor changes

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

//Dreht ein Rad des Roboters
void turnWheel(int directionPin, int pwmPin, int directionValue=LOW, int pwmValue=MOTOR_PWM_MIN){ 
  digitalWrite(directionPin,directionValue); // direction = forward
  analogWrite(pwmPin, 255-pwmValue);         // PWM speed = fast
}

// Stoppt ein Roboterrad
void stopWheel(int directionPin, int pwmPin){
  digitalWrite(directionPin, LOW );
  digitalWrite(pwmPin, LOW );
}

//Stopt das Roboterrad
void stopMotors(){
  stopWheel(MOTOR_LEFT_DIRECTION,MOTOR_LEFT_PWM);
  stopWheel(MOTOR_RIGHT_DIRECTION,MOTOR_RIGHT_PWM);
  
  //Abwarten bis Roboter gestoppt wurde
  delay(MOTOR_DIRECTION_DELAY);
}

//Fährt den Roboter nach vorne
void driveForward(int motorSpeed=MOTOR_PWM_MIN){
  turnWheel(MOTOR_LEFT_DIRECTION,MOTOR_LEFT_PWM,LOW,motorSpeed);
  turnWheel(MOTOR_RIGHT_DIRECTION,MOTOR_RIGHT_PWM,LOW,motorSpeed);
  
}

//Fährt den Roboter zurück
void driveBackwards(int motorSpeed=MOTOR_PWM_MIN){
  motorSpeed+=255;
  turnWheel(MOTOR_RIGHT_DIRECTION,MOTOR_RIGHT_PWM,HIGH,motorSpeed);
  turnWheel(MOTOR_LEFT_DIRECTION,MOTOR_LEFT_PWM,HIGH,motorSpeed);
  
}

//Dreht den Roboter nach rechts
void turnRight(){
  turnWheel(MOTOR_LEFT_DIRECTION,MOTOR_LEFT_PWM);
}

//Dreht den Roboter nach links
void turnLeft(){
  turnWheel(MOTOR_RIGHT_DIRECTION,MOTOR_RIGHT_PWM);
}

//Gibt die Sensordaten über die serielle Schnittstelle aus
void serialStatus(){
  Serial.print("Ultraschalsensor: ");
  Serial.println(sonar.ping_cm());
  Serial.print("IR-Sensor-Links: ");
  Serial.println(digitalRead(INFAROT_LEFT));
  Serial.print("IR-Sensor-Links: ");
  Serial.println(digitalRead(INFAROT_RIGHT));
}

//Fährt den Roboter zuällig durch den Raum und umgeht Hindernisse
void randomDrive(){
  //Prüfen ob Objekt in Fahrbahn
  if(sonar.ping_cm()<=40 && sonar.ping_cm()){
    if(sonar.ping_cm()<=20){
      //Rückwärtsgang einlegen wenn zu nah an Objekt
      driveBackwards();
    }else{
      //Zufälligerweise in die eine, oder andere Richtung drehen
      if(random(2)){
        turnLeft();
      }else{
        turnRight();
      }
    }
  }else{
    driveForward(MOTOR_PWM_MIN);
  }
  //Zeit für Aktionen gewähren
  delay(MOTOR_DIRECTION_DELAY);
  
  //Motoren nach Aktion stoppen
  stopMotors();
}

//Lässt eine LED blinken
void blinkLED(int led,int count=7, int time=1000){
  for(int i=0;i<count;i++){  
    digitalWrite(led, HIGH);
    delay(time);               
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  }
}

//Stellt ein Serielles Testinterface zur Verfügung
void siMenue(){
  byte c;
  boolean isValidInput;
  
  Serial.println( "-----------------------------" );
  Serial.println( "Formiko - Testmenue " );
  Serial.println( "" );
  Serial.println( "1) Forward ");
  Serial.println( "2) Turn Left ");
  Serial.println( "3) Turn Right ");
  Serial.println( "4) Stop ");
  Serial.println( "5) Show Status ");
  Serial.println( "" );
  Serial.println( "@author kf" );
  Serial.println( "@since 2016-01-16" );
  Serial.println( "-----------------------------" );
    
  do{
    isValidInput = true;
    // get the next character from the serial port
    Serial.print( "?" );
    //Abwarten bis serielles Interface zur Verfügung steht
    //while( !Serial.available() );
    c = Serial.read();
    switch( c )
    {
      case '1':      
        driveForward();
        break;
      case '2':      
        turnLeft();
        break;
      case '3':      
        turnRight();
        break;
      case '4':      
        stopMotors();
        break;
      case '5':      
        serialStatus();
        break;
      default:
        Serial.println("Die gewünschte Funktion steht nicht zur Verfügung!");
        break;
    }
    delay(1500);
  } while(true); 
}
//Initialisierung
void setup(){
  //LED initialisierung
  pinMode(INTERNAL_LED,OUTPUT);
  pinMode(EXTERNAL_LED,OUTPUT);
  
  //Infarotinitialisierung
  pinMode(INFAROT_LEFT,INPUT);
  pinMode(INFAROT_RIGHT,INPUT);
  
  //Initialisierung des linken Motors
  pinMode(MOTOR_LEFT_DIRECTION, OUTPUT );
  pinMode(MOTOR_LEFT_PWM, OUTPUT );
  digitalWrite(MOTOR_LEFT_DIRECTION, LOW );
  digitalWrite(MOTOR_LEFT_PWM, LOW );

  //Initialisierung des rechten Motors
  pinMode(MOTOR_RIGHT_DIRECTION, OUTPUT );
  pinMode(MOTOR_RIGHT_PWM, OUTPUT );
  digitalWrite(MOTOR_RIGHT_DIRECTION, LOW );
  digitalWrite(MOTOR_RIGHT_PWM, LOW );

  //Open serial monitor at 115200 baud to see ping results.
  Serial.begin(115200);

  //Randomwerte zur verfügung stellen
  randomSeed(analogRead(0));
}

//Main-Loop
void loop(){
  //blinkLED(EXTERNAL_LED);
  //digitalWrite(INTERNAL_LED, HIGH);
  //Solange Standartroutinen ausführen bis USB geladen 
  //while( !Serial.available() ){
  //   randomDrive();
  //}
  //Serieles Menue aufrufen
  siMenue();
  //serialStatus();
}
