#include "tracer.cpp"
#include "../../core/state.cpp"

#ifndef TRACER_RIGHT_CPP
#define TRACER_RIGHT_CPP

class TracerRight : public Tracer
{
private:
    const short pin = RIGHT_TRACER_PIN;

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

TracerRight *tracerRight = new TracerRight();

PI_THREAD(listenTracerRight)
{
    while (true)
    {
        state->setTracerRight(tracerRight->read());
    }
}

#endif