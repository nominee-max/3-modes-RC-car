#include <Servo.h>

// L298N control pins
#define enabA 5
#define enabB 6
#define in1 2
#define in2 3
#define in3 4
#define in4 7

// distance sensor pins
#define trig_pin A5 // analog input 5
#define echo_pin A4 // analog input 4

#define maximum_distance 200

boolean moveFront = false;
boolean goesForward = false;
int distance = 100;

Servo servo_motor; // our servo name

void setup(){

  pinMode(enablePinA, OUTPUT);
  pinMode(RightMotorForward, OUTPUT);
  pinMode(LeftMotorForward, OUTPUT);
  pinMode(LeftMotorBackward, OUTPUT);
  pinMode(RightMotorBackward, OUTPUT);
  pinMode(enablePinB, OUTPUT);

  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);

  servo_motor.attach(3); // our servo pin

  servo_motor.write(115);
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
}

void loop(){

  int speed = 150;     // varying the motor by this value (0-255)
  int distanceRight = 0;
  int distanceLeft = 0;
  delay(50);

  if (distance <= 40){
    moveStop();
    delay(100);
    moveBackward();
    delay(200); // distance covered
    moveStop();
    delay(300);
    distanceRight = lookRight();
    delay(400);
    distanceLeft = lookLeft();
    delay(400);

    // turn toward whichever side has more clearance
    if (distanceRight >= distanceLeft){
      turnRight();
      moveStop();
    }
    else{
      turnLeft();
      moveStop();
    }
  }
  else{
    moveForward();
  }
  distance = readPing();
}

int lookRight(){
  servo_motor.write(50);  // turn right
  delay(500);
  int dist = readPing();
  delay(100);
  servo_motor.write(115);
  return dist;
}

int lookLeft(){
  servo_motor.write(170); // turn left
  delay(500);
  int dist = readPing();
  delay(100);
  servo_motor.write(115);
  return dist;
}

int readPing(){
  delay(70);

  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  long duration = pulseIn(echo_pin, HIGH, 17400); // ~150cm timeout, roughly matches original 200cm cap
  int cm = duration / 58; // convert echo time to centimeters

  if (cm == 0 || cm > maximum_distance){
    cm = 250;
  }
  return cm;
}

void moveStop(){
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
  digitalWrite(LeftMotorBackward, LOW);
}

void moveForward(){
  int speed = 150;
  if (!goesForward){
    goesForward = true;
    // right, left - clockwise
    digitalWrite(LeftMotorForward, HIGH);
    digitalWrite(RightMotorForward, HIGH);

    digitalWrite(LeftMotorBackward, LOW);
    digitalWrite(RightMotorBackward, LOW);

    analogWrite(enablePinA, speed);
    analogWrite(enablePinB, speed);
  }
}

void moveBackward(){
  int speed = 180;
  goesForward = false;
  // right, left - anticlockwise
  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);

  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorForward, LOW);

  analogWrite(enablePinA, speed);
  analogWrite(enablePinB, speed);
}

void turnRight(){
  int speed = 150;
  // right - clock, left - anti
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);
  delay(5);

  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorForward, LOW);
  analogWrite(enablePinA, speed);
  analogWrite(enablePinB, speed);

  delay(250); // by varying this delay value, rotate the robot

  // right - clock, left - clock (straighten out)
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  delay(5);

  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);
  analogWrite(enablePinA, speed);
  analogWrite(enablePinB, speed);

  goesForward = false;
}

void turnLeft(){
  int speed = 150;
  // right - anti, left - clock
  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  delay(5);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
  analogWrite(enablePinA, speed);
  analogWrite(enablePinB, speed);

  delay(250); // by varying this delay value, rotate the robot

  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  delay(5);

  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);

  analogWrite(enablePinA, speed);
  analogWrite(enablePinB, speed);

  goesForward = false;
}