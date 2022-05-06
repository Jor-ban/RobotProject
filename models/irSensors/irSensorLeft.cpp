#include "irSensor.cpp"
#include "../../core/state.cpp"

#ifndef IRSENSOR_LEFT_CPP
#define IRSENSOR_LEFT_CPP

class IRSensorLeft : public IRSensor
{
private:
    const short pin = LEFT_IR_PIN;

public:
    void init()
    {
        IRSensor::init(this->pin);
    }

    short read()
    {
        return IRSensor::read(pin);
    }
};

IRSensorLeft *irSensorLeft = new IRSensorLeft();

PI_THREAD(listenIRSensorLeft)
{
    while (true)
    {
        state->setIRSensorLeft(irSensorLeft->read());
    }
}

#endif