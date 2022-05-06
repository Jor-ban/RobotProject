#include <stdio.h>
#include <softPwm.h>
#include <wiringPi.h>

#include "../helpers/constants.h"

#ifndef MOTOR_CPP
#define MOTOR_CPP

#define MOTOR_MAX_SPEED 255

class Motor
{
public:
    void init()
    {
        const int pins[4] = {IN1_PIN, IN2_PIN, IN3_PIN, IN4_PIN};

        for (const int pin : pins)
            pinMode(pin, PWM_OUTPUT);

        for (const int pin : pins)
            softPwmCreate(pin, 0, MOTOR_MAX_SPEED);
    }

    void reset()
    {
        const int pins[4] = {IN1_PIN, IN2_PIN, IN3_PIN, IN4_PIN};

        for (const int pin : pins)
            pinMode(pin, OUTPUT);

        init();
    }

    void _pwmWrite(float pin1Intensity = 1, float pin2Intensity = 1, float pin3Intensity = 1, float pin4intensity = 1)
    {
        softPwmWrite(IN1_PIN, (int)(pin1Intensity * MOTOR_MAX_SPEED));
        softPwmWrite(IN2_PIN, (int)(pin2Intensity * MOTOR_MAX_SPEED));
        softPwmWrite(IN3_PIN, (int)(pin3Intensity * MOTOR_MAX_SPEED));
        softPwmWrite(IN4_PIN, (int)(pin4intensity * MOTOR_MAX_SPEED));
    }

    void forward(float intensity = 1)
    {
        _pwmWrite(intensity, 0, intensity, 0);
    }

    void right(float intensity = 1)
    {
        _pwmWrite(intensity, 0, 0, intensity);
    }

    void left(float intensity = 1)
    {
        _pwmWrite(0, intensity, intensity, 0);
    }

    void back(float intensity = 1)
    {
        _pwmWrite(0, intensity, 0, intensity);
    }

    void stop(int timeout = 0)
    {
        _pwmWrite(0, 0, 0, 0);
        reset();

        if (timeout)
            delay(timeout);
    }
};

Motor *motor = new Motor();

#endif