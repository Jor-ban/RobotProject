#include "tracer.cpp"
#include "../../core/state.cpp"

#ifndef TRACER_LEFT_CPP
#define TRACER_LEFT_CPP

class TracerLeft : public Tracer
{
private:
    const int pin = LEFT_TRACER_PIN;

public:
    void init()
    {
        Tracer::init(this->pin);
    }

    short read()
    {
        return Tracer::read(pin);
    }
};

TracerLeft *tracerLeft = new TracerLeft();

PI_THREAD(listenTracerLeft)
{
    while (true)
    {
        state->setTracerLeft(tracerLeft->read());
    }
}

#endif