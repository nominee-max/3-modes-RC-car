//reciver code 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Uncomment to remove debugging
#define PRINT_DEBUG   
#define SIGNAL_TIMEOUT 500  // Signal timeout in milliseconds.
#define MAX_MOTOR_SPEED 200

const uint64_t pipeIn = 0xF9E8F0F0E1LL;
RF24 radio(8, 9);
unsigned long lastRecvTime = 0;
bool signalLost = false;

struct PacketData {
  byte xAxisValue;    
  byte yAxisValue;
} receiverData;

// Motor pins
int enableRightMotor = 5; 
int rightMotorPin1 = 2;
int rightMotorPin2 = 3;
int enableLeftMotor = 6;
int leftMotorPin1 = 4;
int leftMotorPin2 = 7;

void setup() {
  // Set motor control pins as outputs
  pinMode(enableRightMotor, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  pinMode(enableLeftMotor, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);

  // Stop motors initially
  rotateMotor(0, 0); 
  
  //Initialize radio communication
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, pipeIn);
  radio.startListening(); // Start the radio receiver 

  #ifdef PRINT_DEBUG
    Serial.begin(115200);
    if (!radio.isChipConnected()) {
      Serial.println(F("Warning: radio not detected. check wiring"));
    }
    Serial.println(F("setup conplete. Wating of data..."));
  #endif
}

void loop() {
  bool pktRecieved = false;
  // Check if RF is connected and packet is available 
  rotateMotor(MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);
  lastRecvTime = millis();  

  
  if (radio.available()) {
    radio.read(&receiverData, sizeof(PacketData)); 
    pktRecieved = true;
  }
  if (pktRecieved) {
    // Print received values for debugging
    #ifdef PRINT_DEBUG  
      Serial.print(F("Received X: "));
      Serial.print(receiverData.xAxisValue);
      Serial.print(F(", Y: "));
      Serial.println(receiverData.yAxisValue);      
    #endif

    //motorControls
    if (receiverData.yAxisValue >= 175) { // Move car Forward
    rotateMotor(MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);
    } 
    else if (receiverData.yAxisValue <= 75) { // Move car Backward
      rotateMotor(-MAX_MOTOR_SPEED, -MAX_MOTOR_SPEED);
    } 
    else if (receiverData.xAxisValue >= 175) { // Move car Right
      rotateMotor(-MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);
    } 
    else if (receiverData.xAxisValue <= 75) { // Move car Left
      rotateMotor(MAX_MOTOR_SPEED, -MAX_MOTOR_SPEED);
    } 
    else { // Stop the car
      rotateMotor(0, 0);
    }
  }
  else if (millis() - lastRecvTime > SIGNAL_TIMEOUT) {
    rotateMotor(0, 0);
    #ifdef PRINT_DEBUG
    if (!signalLost) {
      Serial.println(F("Signal lost. Motors stopped."));
    }
    #endif
    signalLost = true;
    }
}


void rotateMotor(int rightMotorSpeed, int leftMotorSpeed) {
  // Control right motor direction
  if (rightMotorSpeed < 0) {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);    
  } 
  else if (rightMotorSpeed > 0) {
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);      
  } 
  else {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, LOW);      
  }

  // Control left motor direction
  if (leftMotorSpeed < 0) {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);    
  } 
  else if (leftMotorSpeed > 0) {
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);      
  } 
  else {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, LOW);      
  }  

  // Set motor speeds
  analogWrite(enableRightMotor, abs(rightMotorSpeed));
  analogWrite(enableLeftMotor, abs(leftMotorSpeed));    
}
