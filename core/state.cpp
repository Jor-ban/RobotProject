#include <stdio.h>
#include "../helpers/constants.h"
#include "../models/motor.cpp"

#ifndef STATE_CPP
#define STATE_CPP

class State
{
private:
    short tracerLeft = TRACER_RESTRICTED;
    short tracerRight = TRACER_RESTRICTED;

    // short irSensorLeft = TRACER_RESTRICTED;
    // short irSensorRight = TRACER_RESTRICTED;

    int ultraSonic = 40;

    short doubleLineCounter = 0;
    short objectDetectionCounter = 0;

public:
    void setTracerLeft(short value)
    {
        this->tracerLeft = value;
    }

    void setTracerRight(short value)
    {
        this->tracerRight = value;
    }

    // void setIRSensorLeft(short value)
    // {
    //     this->irSensorLeft = value;
    // }

    // void setIRSensorRight(short value)
    // {
    //     this->irSensorRight = value;
    // }

    void setUltraSonic(int value)
    {
        this->ultraSonic = value;
    }

    void init()
    {
        while (true)
        {
            if (this->ultraSonic < 24)
            {
                this->objectDetectionCounter++;

                switch (this->objectDetectionCounter)
                {
                case 1:
                    while (this->ultraSonic < 24)
                    {
                        motor->stop();
                        delay(1500);
                    }
                    break;
                case 2:
                    motor->stop(100);
                    this->switchToRightLine();
                    motor->stop(100);
                    break;
                case 3:
                    motor->stop(100);
                    this->switchToLeftLine();
                    motor->stop(100);
                    break;
                }
            }
            else if (this->tracerLeft == TRACER_RESTRICTED && this->tracerRight == TRACER_ALLOWED)
            {
                motor->right(0.5);
            }
            else if (this->tracerRight == TRACER_RESTRICTED && this->tracerLeft == TRACER_ALLOWED)
            {
                motor->left(0.5);
            }
            else if (this->tracerLeft == TRACER_ALLOWED && this->tracerRight == TRACER_ALLOWED)
            {
                this->doubleLineCounter++;

                switch (this->doubleLineCounter)
                {
                case 1:
                    motor->forward();
                    delay(500);
                    break;
                case 2:
                    this->switchToLeftLine();
                    break;
                case 4:
                    this->switchToRightLine();
                    break;
                case 3:
                    motor->stop(100);
                    this->park();
                    motor->stop(100);
                    this->unpark();
                    motor->stop(100);
                    break;
                case 5:
                    motor->forward();
                    delay(200);
                    motor->stop(100);
                    return;
                }
            }
            else
            {
                motor->forward(0.5);
            }
        }
    }

    void switchToRightLine()
    {
        motor->back(0.7);
        delay(50);
        motor->stop(50);

        motor->right(0.7);
        delay(500);
        motor->stop(50);

        motor->forward(0.7);
        delay(500);
        motor->stop(50);

        motor->left(0.7);
        delay(515);
        motor->stop(50);
    }

    void switchToLeftLine()
    {
        motor->back(0.7);
        delay(50);
        motor->stop(50);

        motor->left(0.7);
        delay(500);
        motor->stop(50);

        motor->forward(0.7);
        delay(435);
        motor->stop(50);

        motor->right(0.7);
        delay(550);
        motor->stop(50);
    }

    void passRight(int distance = 1700)
    {
        motor->right(0.7);
        delay(500);
        motor->stop();
        delay(50);
        motor->forward(0.7);
        delay(550);
        motor->stop();
        delay(100);

        motor->left(0.7);
        delay(520);
        motor->forward(0.7);
        delay(1700);
        motor->stop();
        delay(100);

        motor->left(0.7);
        delay(500);
        motor->forward(0.7);
        delay(435);
        motor->right(0.8);
        delay(560);
    }

    void park()
    {
        // motor->forward(0.7);
        // delay(1400);
        motor->back(0.7);
        delay(50);
        motor->left(0.7);
        delay(430);
        motor->forward(0.7);
        delay(850);
        motor->stop();
        delay(100);
    }

    void unpark()
    {
        motor->left();
        delay(650);
        motor->forward(0.7);
        delay(900);
        motor->right(0.7);
        delay(650);
        // motor->forward(0.7);
        // delay(1400);
    }
};

State *state = new State();

#endif