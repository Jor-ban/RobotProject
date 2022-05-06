#include <stdio.h>
#include <wiringPi.h>
#include "../../helpers/constants.h"

#ifndef TRACER_CPP
#define TRACER_CPP

class Tracer
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