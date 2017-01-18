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

//Bibliothek für Ultraschalsensor
#include <NewPing.h>

//Bibliothek für Motorsteurung
#include <Motor.h>

// Ultraschallsensor
#define TRIGGER_PIN   3  
#define ECHO_PIN      4  
#define MAX_DISTANCE  200 

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
#define MOTOR_PWM_NORMAL 50
#define MOTOR_DIRECTION_DELAY 50  // brief delay for abrupt motor changes

//Initialisierung des Ultraschalsonars
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

//Initialisierung der Motoren
Motor motorLeft = Motor(MOTOR_LEFT_DIRECTION,MOTOR_LEFT_PWM,HIGH,MOTOR_PWM_NORMAL,MOTOR_DIRECTION_DELAY);
Motor motorRight = Motor(MOTOR_RIGHT_DIRECTION,MOTOR_RIGHT_PWM,HIGH,MOTOR_PWM_NORMAL,MOTOR_DIRECTION_DELAY);

//Stopt die Motoren
void stopMotors(){
  motorLeft.stop();
  motorRight.stop();
}

//Fährt den Roboter nach vorne
void driveForward(){
  motorLeft.forward();
  motorRight.forward();
}

//Fährt den Roboter zurück
void driveBackward(){
  motorLeft.backward();
  motorRight.backward();
}

//Dreht den Roboter nach rechts
void turnRight(){
  motorLeft.forward();
  motorRight.backward();
}

//Dreht den Roboter nach links
void turnLeft(){
  motorLeft.backward();
  motorRight.forward();
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

//Lässt eine LED blinken
void blinkLED(int led,int count=1, int time=1000){
  for(int i=0;i<count;i++){  
    digitalWrite(led, HIGH);
    delay(time);               
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(time);
  }
}

//Fährt den Roboter zuällig durch den Raum und umgeht Hindernisse
void randomDrive(){
  //Prüfen ob Objekt in Fahrbahn
  if(sonar.ping_cm()<=20 && sonar.ping_cm()!=0){
   if(sonar.ping_cm()<=10){  
     driveBackward();
   }else
   //Zufälligerweise in die eine, oder andere Richtung drehen
   if(random(2)){
      turnLeft();
   }else{
     turnRight();
   }
  }else{
    driveForward();
  }
  delay(1000);
  //Motoren nach Aktion stoppen
  stopMotors();
  blinkLED(INTERNAL_LED);
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
  Serial.println( "6) Drive Backwards ");
  Serial.println( "" );
  Serial.println( "@author kf" );
  Serial.println( "@since 2016-01-16" );
  Serial.println( "-----------------------------" );
  Serial.print( "?" );
//while(true){    
  do{
    isValidInput = true;
    //Abwarten bis serielles Interface zur Verfügung steht
    while( !Serial.available() );
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
      case '6':      
        driveBackward();
        break;
      default:
        //isValidInput = false;
        break;
    }
    //delay(1500);
  } while(isValidInput);
//Serial.println("Die gewünschte Funktion steht nicht zur Verfügung!");         
//}
}

//Initialisierung
void setup(){
  //LED initialisierung
  pinMode(INTERNAL_LED,OUTPUT);
  pinMode(EXTERNAL_LED,OUTPUT);
  
  //Infarotinitialisierung
  pinMode(INFAROT_LEFT,INPUT);
  pinMode(INFAROT_RIGHT,INPUT);
  
  //Open serial monitor at 115200 baud to see ping results.
  Serial.begin(115200);

  //Randomwerte zur verfügung stellen
  randomSeed(analogRead(0));
  
}

//Main-Loop
void loop(){
  //Solange Standartroutinen ausführen bis USB geladen 
  while( !Serial.available() ){
     randomDrive();
  }
  //Serieles Menue aufrufen
  siMenue();
}
