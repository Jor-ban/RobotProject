#include <wiringPi.h>
#include "../core/state.cpp"
#include "../helpers/constants.h"

#ifndef ULTRA_SONIC_H
#define ULTRA_SONIC_H

class UltraSonic
{
private:
    const int transPin = ULTRA_TRANS_TRACER_PIN;
    const int recvPin = ULTRA_RECV_TRACER_PIN;

public:
    void init()
    {
        pinMode(this->transPin, OUTPUT);
        pinMode(this->recvPin, INPUT);
    }

    int read()
    {
        int start_time = 0, end_time = 0;
        float distance = 0;

        digitalWrite(this->transPin, LOW);
        delay(100);
        digitalWrite(this->transPin, HIGH);
        delay(10);
        digitalWrite(this->transPin, LOW);

        while (digitalRead(this->recvPin) == 0)
            start_time = micros();

        while (digitalRead(this->recvPin) == 1)
            end_time = micros();

        distance = (end_time - start_time) / 29. / 2.;
        
        return (int)distance;
    }
};

UltraSonic *ultraSonic = new UltraSonic();

PI_THREAD(listenUltra)
{
    while (true)
    {
        state->setUltraSonic(ultraSonic->read());
    }
}

#endif