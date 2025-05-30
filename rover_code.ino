#include <NewPing.h>           
#include <Servo.h>             
#include <AFMotor.h>           

#define LEFT A5             
#define RIGHT A1            
#define TRIGGER_PIN A2      
#define ECHO_PIN A3         
#define MAX_DISTANCE 50     

unsigned int distance = 0;    
unsigned int Right_Value = 0; 
unsigned int Left_Value = 0; 
int d = 10;                   

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);  

AF_DCMotor Motor1(1, MOTOR12_1KHZ);
AF_DCMotor Motor2(2, MOTOR12_1KHZ);
AF_DCMotor Motor3(3, MOTOR34_1KHZ);
AF_DCMotor Motor4(4, MOTOR34_1KHZ);

Servo myservo;  
int servoPosition = 90;  


int direction = 3; 

void setup() {
  Serial.begin(9600);  
  myservo.attach(10);  

  
  myservo.write(servoPosition);  
  delay(1000);  

  pinMode(RIGHT, INPUT);  
  pinMode(LEFT, INPUT);   
  
  stop(); 

  Serial.println("Robot initialized. Starting loop...");
}

void loop() {
  delay(50);  
  distance = sonar.ping_cm();  
  

  Serial.print("Ultrasonic Sensor Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  Right_Value = !digitalRead(RIGHT);
  Left_Value = !digitalRead(LEFT);
  
 
  Serial.print("Right IR Sensor: ");
  Serial.println(Right_Value);
  Serial.print("Left IR Sensor: ");
  Serial.println(Left_Value);

r
  if (Right_Value == 0 && Left_Value == 0) {
 
    if (distance > 1 && distance < d) {
      direction = 0; 
      moveForward();
      Serial.println("Action: Moving Forward (Ultrasonic)");
    } else {
      direction = 3; 
      stop();
      Serial.println("Action: Stopped (No Obstacle)");
    }
  } else if (Right_Value == 1 && Left_Value == 0) {
   
    direction = 1;
    turnRight();
    Serial.println("Action: Turning Right (IR)");
  } else if (Right_Value == 0 && Left_Value == 1) {
    
    direction = 2;
    turnLeft();
    Serial.println("Action: Turning Left (IR)");
  } else if (Right_Value == 1 && Left_Value == 1) {
    
    direction = 0; 
    moveForward();
    Serial.println("Action: Moving Forward (Both IR)");
  }

  
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

  if (direction == 0) {
    servoPosition = 90;  // Forward
  } else if (direction == 1) {
    servoPosition = 60;  // Right
  } else if (direction == 2) {
    servoPosition = 120; // Left
  }

  myservo.write(servoPosition);
}
