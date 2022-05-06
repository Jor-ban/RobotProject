#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

//Line tracer
#define LEFT_TRACER_PIN 10
#define RIGHT_TRACER_PIN 11

// Motor Pins
#define IN1_PIN 1
#define IN2_PIN 4
#define IN3_PIN 5
#define IN4_PIN 6

// IR Pins
#define LEFT_IR_PIN 27
#define RIGHT_IR_PIN 26

// Ultrasonic Sensor Pins
#define TRIG_PIN 28
#define ECHO_PIN 29

// Speed
#define MAX_SPEED 50  
#define MIN_SPEED 0

// Init IR Sensors
void initIR();

// Init Ultrasonic Sensor
void initUltrasonic();

// DC Motor
void initDCMotorsoft();
void initDCMotor();
void goForward();
void goBackward();
void goLeft();
void goRight();
void goLeftk();
void goRightk();
void smoothLeft();
void smoothRight();
void slow();
void stopDCMotor();
void stopDCMotorsoft();

// Get distance function
int getDistance();
int dist;
int i = 0;

int main(void)
{

  if (wiringPiSetup() == -1) {
    printf("Add -lwiringPi flag in compilation")
    return 0;
  }


  int LValue, RValue;
  initIR();
  initUltrasonic();
  initDCMotorsoft();
  

  while(i < 3)
  {
        
    dist = getDistance();
  
  
    LValue = digitalRead(LEFT_IR_PIN);
    RValue = digitalRead(RIGHT_IR_PIN);
    int leftTracer = digitalRead(LEFT_TRACER_PIN);
    int rightTracer = digitalRead(RIGHT_TRACER_PIN);

    tracer(leftTracer, rightTracer);

    if(dist <= 15) {
      i++;
      stopDCMotorsoft();
      printf("STOP: %d OBSTACLE IS DETECTED.\n", i);
      obs();
    }
    
    else {
      slow();
    }    
  }

  stopDCMotorsoft();
  return 0;
}


void obs(){
    
  switch (i)
  {
    case 1:
      printf("Case1 ");
      break;
    case 2:
      printf("Case2 ");
      smoothRight();
      delay(500);
      slow();
      delay(500);
      smoothLeft();
      delay(500);
      slow();break
      delay(500);
      smoothLeft();
      delay(500);
      smoothRight();
      delay(500);
      break;

    case 3:
      printf("Case3 ");
      stopDCMotorsoft();
      break;
    default:
      printf("Smth went wrong");
  }
}

void initLineTracer()
{
  pinMode(LEFT_TRACER_PIN, INPUT);
  pinMode(RIGHT_TRACER_PIN, INPUT);
}

void tracer(leftTracer, rightTracer){
  if (leftTracer == 0 && rightTracer == 1) {
    printf("Right\n");
    smoothRight();
    delay(500);if (leftTracer == 0 && rightTracer == 1) {
    printf("Right\n");
    smoothRight();
    delay(500);
  }
  else if (rightTracer == 0 && leftTracer == 1) {
    printf("Left\n");
    smoothLeft();
    delay(500);
  }
  else if (rightTracer == 0 && leftTracer == 0) {
    printf("Stop\n");
    stopDCMotorsoft();
  }
  else if (rightTracer == 1 && leftTracer == 1) {
    printf("Forward\n");
    slow();
    delay(500);
  }
}
  }
  else if (rightTracer == 0 && leftTracer == 1) {
    printf("Left\n");
    smoothLeft();
    delay(500);
  }
  else if (rightTracer == 0 && leftTracer == 0) {
    printf("Stop\n");
    stopDCMotorsoft();
  }
  else if (rightTracer == 1 && leftTracer == 1) {
    printf("Forward\n");
    slow();
    delay(500);
  }
}

void initIR()
{
  pinMode(LEFT_IR_PIN, INPUT);
  pinMode(RIGHT_IR_PIN, INPUT);
}

void initUltrasonic()
{
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

int getDistance()
{
  int   start_time=0, end_time=0;
  float   distance=0;

    digitalWrite(TRIG_PIN, LOW) ;
    delay(500) ;
    digitalWrite(TRIG_PIN, HIGH) ;
    delayMicroseconds(10) ;
    digitalWrite(TRIG_PIN, LOW) ;
    
    while (digitalRead(ECHO_PIN) == 0) ;
    start_time = micros() ;
    
    while (digitalRead(ECHO_PIN) == 1) ;
    end_time = micros() ;
    
    distance = (end_time - start_time) / 29. / 2. ;
  
  return (int)distance;
}
void initDCMotorsoft()
{
  pinMode(IN1_PIN, SOFT_PWM_OUTPUT);
  pinMode(IN2_PIN, SOFT_PWM_OUTPUT);
  pinMode(IN3_PIN, SOFT_PWM_OUTPUT);
  pinMode(IN4_PIN, SOFT_PWM_OUTPUT);

  softPwmCreate(IN1_PIN, MIN_SPEED, MAX_SPEED);
  softPwmCreate(IN2_PIN, MIN_SPEED, MAX_SPEED);
  softPwmCreate(IN3_PIN, MIN_SPEED, MAX_SPEED);
  softPwmCreate(IN4_PIN, MIN_SPEED, MAX_SPEED);
}

void initDCMotor()
{
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);

  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, HIGH);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, HIGH);
}

void goForward()
{
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
  printf("Forward\n");
}

void slow()
{

  softPwmWrite(IN1_PIN, 30);
  softPwmWrite(IN2_PIN, MIN_SPEED);
  softPwmWrite(IN3_PIN, 30);
  softPwmWrite(IN4_PIN, MIN_SPEED);
  printf("Slowly forward\n");
}

void smoothRight()
{
  softPwmWrite(IN1_PIN, MAX_SPEED/8);
  softPwmWrite(IN2_PIN, MIN_SPEED);
  softPwmWrite(IN3_PIN, MAX_SPEED);
  softPwmWrite(IN4_PIN, MIN_SPEED);
  printf("Right\n");
}

void smoothLeft()
{
  softPwmWrite(IN1_PIN, MAX_SPEED);
  softPwmWrite(IN2_PIN, MIN_SPEED);
  softPwmWrite(IN3_PIN, MAX_SPEED/8);
  softPwmWrite(IN4_PIN, MIN_SPEED);
  printf("Left\n");}

void goLeft()
{
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
  printf("Left\n");
}

void goRight()
{
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, HIGH);
  printf("Right\n");
}

void goRightk()
{
  softPwmWrite(IN1_PIN, MAX_SPEED);
  softPwmWrite(IN2_PIN, MIN_SPEED);
  softPwmWrite(IN3_PIN, MIN_SPEED);
  softPwmWrite(IN4_PIN, MAX_SPEED);
  printf("Right\n");
}

void goLeftk()
{
  softPwmWrite(IN1_PIN, MIN_SPEED);
  softPwmWrite(IN2_PIN, MAX_SPEED);
  softPwmWrite(IN3_PIN, MAX_SPEED);
  softPwmWrite(IN4_PIN, MIN_SPEED);
  printf("Left\n");
}

void stopDCMotorsoft()
{
  softPwmWrite(IN1_PIN, MIN_SPEED);
  softPwmWrite(IN2_PIN, MIN_SPEED);
  softPwmWrite(IN3_PIN, MIN_SPEED);
  softPwmWrite(IN4_PIN, MIN_SPEED);
  printf("Stop\n");
}

void stopDCMotor()
{
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
  printf("Stop\n");
}