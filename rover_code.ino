#include <NewPing.h>           
#include <Servo.h>             
#include <AFMotor.h>           

#define LEFT A5             // Left IR sensor connected to analog pin A5 of Arduino Uno
#define RIGHT A1            // Right IR sensor connected to analog pin A1 of Arduino Uno
#define TRIGGER_PIN A2      // Trigger pin connected to analog pin A2 of Arduino Uno
#define ECHO_PIN A3         // Echo pin connected to analog pin A3 of Arduino Uno
#define MAX_DISTANCE 50     // Maximum ping distance

unsigned int distance = 0;    // Variable to store ultrasonic sensor distance
unsigned int Right_Value = 0; // Variable to store Right IR sensor value
unsigned int Left_Value = 0;  // Variable to store Left IR sensor value
int d = 10;                   // Distance threshold for following

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);  // NewPing setup of pins and max distance

// Create motor objects
AF_DCMotor Motor1(1, MOTOR12_1KHZ);
AF_DCMotor Motor2(2, MOTOR12_1KHZ);
AF_DCMotor Motor3(3, MOTOR34_1KHZ);
AF_DCMotor Motor4(4, MOTOR34_1KHZ);

Servo myservo;  // Create servo object to control the servo
int servoPosition = 90;  // Variable to store the current servo position

// Variable to track the rover's direction (0 = Forward, 1 = Right, 2 = Left, 3 = Stop)
int direction = 3; 

void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 bits per second
  myservo.attach(10);  // Servo attached to pin 10 of Arduino UNO

  // Initialize the servo to center position (facing forward)
  myservo.write(servoPosition);  
  delay(1000);  // Wait 1 second

  pinMode(RIGHT, INPUT);  // Set RIGHT as input (IR sensor)
  pinMode(LEFT, INPUT);   // Set LEFT as input (IR sensor)
  
  stop();  // Stop all motors initially

  Serial.println("Robot initialized. Starting loop...");
}

void loop() {
  delay(50);  // Wait 50ms between pings
  distance = sonar.ping_cm();  // Get distance from ultrasonic sensor
  
  // Print the distance
  Serial.print("Ultrasonic Sensor Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Read IR sensor values
  Right_Value = !digitalRead(RIGHT);
  Left_Value = !digitalRead(LEFT);
  
  // Print the IR sensor values
  Serial.print("Right IR Sensor: ");
  Serial.println(Right_Value);
  Serial.print("Left IR Sensor: ");
  Serial.println(Left_Value);

  // Priority logic: IR sensors take precedence over ultrasonic sensor
  if (Right_Value == 0 && Left_Value == 0) {
    // No IR sensor detection, use ultrasonic sensor
    if (distance > 1 && distance < d) {
      direction = 0; // Move forward
      moveForward();
      Serial.println("Action: Moving Forward (Ultrasonic)");
    } else {
      direction = 3; // Stop
      stop();
      Serial.println("Action: Stopped (No Obstacle)");
    }
  } else if (Right_Value == 1 && Left_Value == 0) {
    // Turn Right
    direction = 1;
    turnRight();
    Serial.println("Action: Turning Right (IR)");
  } else if (Right_Value == 0 && Left_Value == 1) {
    // Turn Left
    direction = 2;
    turnLeft();
    Serial.println("Action: Turning Left (IR)");
  } else if (Right_Value == 1 && Left_Value == 1) {
    // Both IR sensors detect obstacles; stop or move forward as a fallback
    direction = 0; // Move forward cautiously
    moveForward();
    Serial.println("Action: Moving Forward (Both IR)");
  }

  // Update servo position based on the direction of the rover
  updateServoPosition();
}

void moveForward() {
  Motor1.setSpeed(180);
  Motor1.run(FORWARD);
  Motor2.setSpeed(180);
  Motor2.run(FORWARD);
  Motor3.setSpeed(180);
  Motor3.run(FORWARD);
  Motor4.setSpeed(180);
  Motor4.run(FORWARD);
}

void turnRight() {
  Motor1.setSpeed(180);
  Motor1.run(FORWARD);
  Motor2.setSpeed(180);
  Motor2.run(FORWARD);
  Motor3.setSpeed(180);
  Motor3.run(BACKWARD);
  Motor4.setSpeed(180);
  Motor4.run(BACKWARD);
  delay(100);
}

void turnLeft() {
  Motor1.setSpeed(180);
  Motor1.run(BACKWARD);
  Motor2.setSpeed(180);
  Motor2.run(BACKWARD);
  Motor3.setSpeed(180);
  Motor3.run(FORWARD);
  Motor4.setSpeed(180);
  Motor4.run(FORWARD);
  delay(100);
}

void stop() {
  Motor1.setSpeed(0);
  Motor1.run(RELEASE);
  Motor2.setSpeed(0);
  Motor2.run(RELEASE);
  Motor3.setSpeed(0);
  Motor3.run(RELEASE);
  Motor4.setSpeed(0);
  Motor4.run(RELEASE);
}

void updateServoPosition() {
  // Update the servo position based on the rover's movement direction
  if (direction == 0) {
    servoPosition = 90;  // Forward
  } else if (direction == 1) {
    servoPosition = 60;  // Right
  } else if (direction == 2) {
    servoPosition = 120; // Left
  }

  // Update the servo only if its position has changed
  myservo.write(servoPosition);
}
