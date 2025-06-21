

#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11); // RX=10, TX=11 (from transmitter module)

char command;
int controlMode = 0;  // 0 = App/Voice mode, 1 = Gesture mode

// Motor pins
int motorInput1 = 5;
int motorInput2 = 6;
int motorInput3 = 3;
int motorInput4 = 9;

void setup() {
  pinMode(motorInput1, OUTPUT);
  pinMode(motorInput2, OUTPUT);
  pinMode(motorInput3, OUTPUT);
  pinMode(motorInput4, OUTPUT);

  stopCar();  // Initially stop the robot

  Serial.begin(38400);      
  BTSerial.begin(9600);      // For communication with the transmitter
}

void loop() {
  // Listen for command from transmitter
  if (BTSerial.available()) {
    command = BTSerial.read();
    Serial.print("Received: "); Serial.println(command);

    // Mode switching
    if (command == 'G') {  // 'G' = Gesture mode
      controlMode = 1;
      Serial.println("Switched to GESTURE mode");
    }
    else if (command == 'A') {  // 'A' = App/Voice mode
      controlMode = 0;
      Serial.println("Switched to APP/VOICE mode");
    }

    // Movement logic (only if command is a movement character)
    if (controlMode == 0 || controlMode == 1) {
      handleMovement(command);
    }
  }
}

// Function to handle robot motion
void handleMovement(char cmd) {
  switch (cmd) {
    case 'F': forward(); break;
    case 'B': reverse(); break;
    case 'L': left(); break;
    case 'R': right(); break;
    case 'S': stopCar(); break;
  }
}

void forward() {
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, HIGH);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, HIGH);
}

void reverse() {
  digitalWrite(motorInput1, HIGH);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, HIGH);
  digitalWrite(motorInput4, LOW);
}

void left() {
  digitalWrite(motorInput1, HIGH);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, HIGH);
}

void right() {
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, HIGH);
  digitalWrite(motorInput3, HIGH);
  digitalWrite(motorInput4, LOW);
}

void stopCar() {
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, LOW);
}



