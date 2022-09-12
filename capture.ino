#include <Servo.h>

#define VCC_PIN 13
#define TRIGGER_PIN 12    // sonar trigger pin will be attached to Arduino pin 12
#define ECHO_PIN 11       // sonar echo pint will be attached to Arduino pin 11
#define GROUND_PIN 10     //
#define MAX_DISTANCE 15  // maximum distance set to 200 cm
#define MIN_DISTANCE 1

Servo myservo;  // create servo object to control a servo

long duration;
int distance = MAX_DISTANCE;
int open = true;
unsigned long time;

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  pinMode(ECHO_PIN, INPUT) ;  
  pinMode(TRIGGER_PIN, OUTPUT) ;
  pinMode(GROUND_PIN, OUTPUT);    // tell pin 10 it is going to be an output
  pinMode(VCC_PIN, OUTPUT);       // tell pin 13 it is going to be an output
  digitalWrite(GROUND_PIN,LOW);   // tell pin 10 to output LOW (OV, or ground)
  digitalWrite(VCC_PIN, HIGH) ;   // tell pin 13 to output HIGH (+5V)
  Serial.begin(9600);
  myservo.write(0);
}

void loop() {
  digitalWrite(TRIGGER_PIN, LOW);   // Clears the trigPin
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);  // Sets the trigPin on HIGH state for 10 micro seconds
  delayMicroseconds(20);
  digitalWrite(TRIGGER_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance= duration*0.034/2;       // Calculating the distance

  // if the distance measurement becomes extremely high, it is likely an error.  
  // Default to a maximum value of MAX_DISTANCE to identify this condition.
  if (distance > MAX_DISTANCE)   
    {distance = MAX_DISTANCE ;}
  
  if(distance < MIN_DISTANCE) {
    distance = MIN_DISTANCE;
  }

  if(millis() > 1000) {
    if(open == true && distance < 5) {                  // if the claw is open and the sonar
      delay(1000);                                      // reading is close to the ground,
      for(int angle = 0; angle <= 180; angle += 1) {    // turn the servo to close the claw
        myservo.write(angle);
        delay(7);
      }
      delay(2000);                             // delay 2 seconds to account for user error
      time = millis();                         // start a timer
      open = false;                            // let the Arduino know the claw has been closed
    }
    else if(open == false && (distance < 6 || (millis() - time) > 7000)) {   // if the claw is closed and either the sonar
      for(int angle = 180; angle >= 0; angle -= 1) {                         // reading is close to the ground or the
        myservo.write(angle);                                                // timer has been running for more than 7
        delay(7);                                                            // seconds, turn the servo to open the claw
      }                                                   
      delay(2000);                             // delay 2 seconds to account for user error                                       
      open = true;                            // let the Arduino know the claw has been closed
    }
  }
  delay(100);
}
