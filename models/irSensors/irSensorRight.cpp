#include "irSensor.cpp"
#include "../../core/state.cpp"

#ifndef IRSENSOR_RIGHT_CPP
#define IRSENSOR_RIGHT_CPP

class IRSensorRight : public IRSensor
{
private:
    const short pin = RIGHT_IR_PIN;

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

IRSensorRight *irSensorRight = new IRSensorRight();

PI_THREAD(listenIRSensorRight)
{
    while (true)
    {
        state->setIRSensorRight(irSensorRight->read());
    }
}

#endif