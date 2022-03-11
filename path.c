//Example 2
//0. Slow movement
//1. Smooth turn RIGHT/LEFT
//2. Smooth BACK-LEFT/RIGHT turn

#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

#define LEFT_TRACER_PIN 10
#define RIGHT_TRACER_PIN 11

#define IN1_PIN    1
#define IN2_PIN    4
#define IN3_PIN    5
#define IN4_PIN    6

#define MAX_SPEED 100
#define MIN_SPEED 0

void initLineTacer();
void initDCMotor();
void goForward();
void goBackward();
void smoothLeft();
void smoothRight();
void stopDCMotor();
void slow();
void smoothBackLeft();
int isFinished=0;

int main(void)
{
  if(wiringPiSetup() == -1)
    return 0;

  initDCMotor();
  initLineTacer();
  
    int leftTracer;
    int rightTracer;

    

 while (isFinished==0) {

        leftTracer = digitalRead(LEFT_TRACER_PIN);
        rightTracer = digitalRead(RIGHT_TRACER_PIN);
        while(leftTracer==1 && rightTracer==1)
        {
    goForward();
     leftTracer = digitalRead(LEFT_TRACER_PIN);
        rightTracer = digitalRead(RIGHT_TRACER_PIN);  
      }
      stopDCMotor();
      delay(200);
    leftTracer = digitalRead(LEFT_TRACER_PIN);
        rightTracer = digitalRead(RIGHT_TRACER_PIN);
      while(leftTracer==0)
      {
        smoothRight();
        leftTracer = digitalRead(LEFT_TRACER_PIN);
        rightTracer = digitalRead(RIGHT_TRACER_PIN);
        
      }
      stopDCMotor();
      delay(200);
      while(rightTracer==0){
        smoothLeft();
        leftTracer = digitalRead(LEFT_TRACER_PIN);
        rightTracer = digitalRead(RIGHT_TRACER_PIN);
        }
      stopDCMotor();
      delay(200);
      //isFinished=1;
      //break;
    }
  return 0;
}


void initDCMotor()
{
pinMode(IN1_PIN,SOFT_PWM_OUTPUT);
pinMode(IN2_PIN,SOFT_PWM_OUTPUT);
pinMode(IN3_PIN,SOFT_PWM_OUTPUT);
pinMode(IN4_PIN,SOFT_PWM_OUTPUT);
 
softPwmCreate(IN1_PIN, MIN_SPEED, MAX_SPEED);
softPwmCreate(IN2_PIN, MIN_SPEED, MAX_SPEED);
softPwmCreate(IN3_PIN, MIN_SPEED, MAX_SPEED);
softPwmCreate(IN4_PIN, MIN_SPEED, MAX_SPEED);
}

void slow(){
  
softPwmWrite(IN1_PIN, 30);
softPwmWrite(IN2_PIN, MIN_SPEED);
softPwmWrite(IN3_PIN, 30);
softPwmWrite(IN4_PIN, MIN_SPEED);

  
  }

void goForward()
{  
softPwmWrite(IN1_PIN, MAX_SPEED);
softPwmWrite(IN2_PIN, MIN_SPEED);
softPwmWrite(IN3_PIN, MAX_SPEED);
softPwmWrite(IN4_PIN, MIN_SPEED);
}

void goBackward()  
{
softPwmWrite(IN1_PIN, MIN_SPEED);
softPwmWrite(IN2_PIN, MAX_SPEED);
softPwmWrite(IN3_PIN, MIN_SPEED);
softPwmWrite(IN4_PIN, MAX_SPEED);
}
  
  
void smoothLeft()
{
softPwmWrite(IN1_PIN, MAX_SPEED/2);
softPwmWrite(IN2_PIN, MIN_SPEED);
softPwmWrite(IN3_PIN, MAX_SPEED);
softPwmWrite(IN4_PIN, MIN_SPEED);
}

void smoothRight()
{
softPwmWrite(IN1_PIN, MAX_SPEED);
softPwmWrite(IN2_PIN, MIN_SPEED);
softPwmWrite(IN3_PIN, MAX_SPEED/2);
softPwmWrite(IN4_PIN, MIN_SPEED);
}

void smoothBackLeft(){
softPwmWrite(IN1_PIN, MIN_SPEED);
softPwmWrite(IN2_PIN, MAX_SPEED/2); 
softPwmWrite(IN3_PIN, MIN_SPEED);
softPwmWrite(IN4_PIN, MAX_SPEED);
}

void smoothBackRight(){
softPwmWrite(IN1_PIN, MIN_SPEED);
softPwmWrite(IN2_PIN, MAX_SPEED); 
softPwmWrite(IN3_PIN, MIN_SPEED);
softPwmWrite(IN4_PIN, MAX_SPEED/2);
}

void stopDCMotor()
{
softPwmWrite(IN1_PIN, MIN_SPEED);
softPwmWrite(IN2_PIN, MIN_SPEED);
softPwmWrite(IN3_PIN, MIN_SPEED);
softPwmWrite(IN4_PIN, MIN_SPEED);    
//printf("Stop\n");
}  

void initLineTacer() {
    pinMode(LEFT_TRACER_PIN, INPUT);
    pinMode(RIGHT_TRACER_PIN, INPUT);
}
  



//gcc 2-motor_C_D.c -o motorcd -lwiringPi
// ./motorcd