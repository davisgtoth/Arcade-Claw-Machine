#include <Servo.h>

Servo myservo;

void setup() {
  myservo.attach(9);
  Serial.begin(9600);
}

void loop() {
  for(int num = 180; num >= 60; num--) {
    myservo.write(num);
    Serial.println(num);
    delay(15);
    if(num == 60) {
      delay(3000);
    }
  }
}
