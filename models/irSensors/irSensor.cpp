#include <stdio.h>
#include <wiringPi.h>
#include "../../helpers/constants.h"

#ifndef IR_SENSOR_CPP
#define IR_SENSOR_CPP

class IRSensor
{
public:
    static void init(short pin)
    {
        pinMode(pin, INPUT);
    }

    static short read(short pin)
    {
        return digitalRead(pin);
    }
};

#endif