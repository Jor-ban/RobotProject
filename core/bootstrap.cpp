#include <wiringPi.h>

#include "state.cpp"
#include "../models/motor.cpp"
#include "../models/ultra.cpp"
#include "../models/tracers/tracerLeft.cpp"
#include "../models/tracers/tracerRight.cpp"
// #include "../models/irSensors/irSensorLeft.cpp"
// #include "../models/irSensors/irSensorRight.cpp"

class Bootstrap
{
public:
    static void init()
    {
        wiringPiSetup();
        motor->init();
        ultraSonic->init();
        tracerLeft->init();
        tracerRight->init();
        // irSensorLeft->init();
        // irSensorRight->init();

        piThreadCreate (listenUltra);
        piThreadCreate (listenTracerLeft);
        piThreadCreate (listenTracerRight);
        // piThreadCreate (listenIRSensorLeft);
        // piThreadCreate (listenIRSensorRight);
        
        state->init();
    }
};