#define in1 2
#define in2 3
#define in3 4
#define in4 7
#define r_sensor_pin A0
#define l_sensor_pin A1

#define DEBUG

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(r_sensor_pin, INPUT);
  pinMode(l_sensor_pin, INPUT);

  #ifdef DEBUG
    Serial.begin(9600);
    Serial.println("INSTRUCTION");
  #endif
}

void loop() {
  int r_val = digitalRead(r_sensor_pin);
  int l_val = digitalRead(l_sensor_pin);

  #ifdef DEBUG
    Serial.print("Right: ");
    Serial.print(r_val);
    Serial.print("  Left: ");
    Serial.println(l_val);
  #endif

  if (r_val == 1 && l_val == 1) {
    forward();
  } else if (r_val == 0 && l_val == 0) {
    stop();
  } else if (r_val == 1 && l_val == 0) {
    turnRight();
  } else if (r_val == 0 && l_val == 1) {
    turnLeft();
  }
}

void forward() {
  // both motors forward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnLeft() {
  // left motor backward, right motor forward
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnRight() {
  // left motor forward, right motor backward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}